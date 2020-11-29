/* =============================================================================
 * EntityMgr
 * 
 * 
 * ===========================================================================*/
#include "EntityMgr.h"
#include <algorithm>

namespace SDG
{
    EntityMgr::EntityMgr(size_t initPoolSize) : entities_(initPoolSize)
    {
    }

    EntityMgr::~EntityMgr()
    {
        Close();
    }

    Entity &EntityMgr::CreateEntity(std::string tag = "")
    {
        return CreateEntity([&tag](Entity &e)->void {
           e.SetTag(tag);
        });
    }

    Entity &EntityMgr::CreateEntity(const std::function<void(Entity &)>&factory)
    {
        // Initialization
        auto *entity = entities_.CheckOut();
        if (factory)
            factory(*entity);

        entity->Init();

        // Put into entity list
        active_.emplace_back(entity);

        // Put into the correct tag bank
        std::string tag = entity->GetTag();
        if (!tag.empty())
        {
            if (!tagLists_.contains(tag))
            {
                tagLists_[tag] = std::vector<Entity *>();
            }

            tagLists_[tag].emplace_back(entity);
        }

        return *entity;
    }

    void EntityMgr::DestroyEntity(Entity &entity)
    {
        entity.toDestroy_ = true;
        this->toDestroy_ = true;
    }

    void EntityMgr::ProcessRemovals()
    {
        if (toDestroy_)
        {
            // Remove-erase all entities marked for destruction.
            active_.erase(
                    std::remove_if(active_.begin(), active_.end(),
                                   [this](Entity *entity) {
                                       bool destroyThisEntity = entity->toDestroy_;
                                       if (destroyThisEntity)
                                       {
                                           // Remove from tagList if there is a tag
                                           if (!entity->GetTag().empty())
                                           {
                                                auto &tagList = tagLists_[entity->GetTag()];
                                                for (auto it = tagList.begin(), end = tagList.end();
                                                    it != end; ++it)
                                                {
                                                    if (entity == *it)
                                                    {
                                                        tagList.erase(it);
                                                        break;
                                                    }
                                                }
                                           }

                                           // Clean up entity
                                           entity->Close();
                                           entities_.Return(entity);
                                       }

                                       return destroyThisEntity;
                                   }),
                    active_.end());

            // Reset destroy flag.
            toDestroy_ = false;
        }
    }
}