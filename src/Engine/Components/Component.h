/* =============================================================================
 * Component
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/Time/GameTime.h>
#include <Engine/Graphics/SpriteBatch.h>
#include <Engine/Content/ContentMgr.h>
#include <Engine/Input/InputMgr.h>
#include <Engine/Components/ComponentList.h>
#include <Engine/Graphics/GraphicsDeviceMgr.h>

namespace SDG
{
    class Component
    {
        friend class ComponentList;
        friend class Game;

        // Attribute bit flags
        enum Flags : unsigned char
        {
            Updatable   = 1u << 0u,
            Drawable    = 1u << 1u,
            Active      = 1u << 2u,
            ToBeRemoved = 1u << 3u,
            WasInit     = 1u << 4u
        };

    // ===== User-implemented Events =====
    protected:
        // User-implemented initialization logic. Use to connect to other components and game resources.
        virtual void Init() { }
        // User-implemented update logic.
        virtual void Update() { }
        // User-implemented postupdate logic. Occurs after Update().
        virtual void PostUpdate() { }
        // User-implemented rendering logic. Use the sprite batch to draw images and shapes.
        virtual void Draw() { }
        // User-implemented clean up logic.
        virtual void Close() { }

    // ===== Constructors =====
    public:
        Component(bool isUpdatable, bool isDrawable): attr_{}, owner_{}
        {
            if (isUpdatable) attr_ |= Updatable;
            if (isDrawable) attr_  |= Drawable;
            attr_ |= Active;
        }

        // Cannot copy
        Component(const Component &) = delete;
        Component &operator=(const Component &) = delete;
        virtual ~Component() = default;

    private:
        // Functions intended for control by friend classes (e.g. class Game/ComponentList)
        // ===== Service Provider Functions =====
        static void Provide(SpriteBatch *spriteBatch) { spriteBatch_ = spriteBatch; }
        static void Provide(const GameTime *gameTime) { gameTime_ = gameTime; }
        static void Provide(ContentMgr *content) { contentMgr_ = content; }
        static void Provide(InputMgr *inputMgr) { inputMgr_ = inputMgr; }
        static void Provide(GraphicsDeviceMgr *graphics) { graphics_ = graphics; }

        // Initializes the component only if it has not been initialized yet.
        void DoInit()
        {
            if (!WasInitialized())
            {
                ForceInit();
            }
        }

        // Initializes the component regardless if it had been initialized or not.
        void ForceInit()
        {
            Init();
            attr_ |= WasInit;
        }

    public:
        // ===== Attributes / Properties =====
        [[nodiscard]]
        bool IsDrawable() const  { return attr_ & Drawable; }
        [[nodiscard]]
        bool IsUpdatable() const { return attr_ & Updatable; }
        [[nodiscard]]
        bool IsActive() const    { return attr_ & Active; }
        [[nodiscard]]
        bool IsRemoving() const  { return attr_ & ToBeRemoved; }
        [[nodiscard]]
        bool WasInitialized() const { return attr_ & WasInit; }

        // Gets owning ComponentList this Component is a member of, or nullptr if there is none.
        [[nodiscard]]
        ComponentList *Owner() const { return owner_; }

        // Convenience function to get owning Entity, or nullptr if there is none. Component must be in a ComponentList
        // in order to have an owning Entity. Otherwise can be retrieved via Owner()->GetEntity();
        [[nodiscard]]
        Entity *GetEntity()
        {
            return (owner_ && owner_->GetEntity()) ? owner_->GetEntity() : nullptr;
        }

        void SetActive(bool active);
        void SetRemoving(bool removing);

    protected:
        // ===== Convenience Getters for Subclasses =====

        // Gets a component from the owning ComponentList that is exactly the same type as template arg or
        // nullptr if there are none.
        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]]
        T *GetComponent() const
        {
            SDG_ASSERT(Owner());
            return Owner()->Get<T>();
        }

        // Gets a component from the owning ComponentList that is either the same type or a child of arg or
        // nullptr if there are none.
        template <typename T> requires std::is_base_of_v<Component, T>
        [[nodiscard]]
        T *GetTypeof() const
        {
            SDG_ASSERT(Owner());
            return Owner()->GetTypeof<T>();
        }

        template <typename T> requires std::is_base_of_v<Component, T>
        void RemoveComponent()
        {
            SDG_ASSERT(Owner());
            Owner()->Remove<T>();
        }

        [[nodiscard]]
        static SpriteBatch *GetSpriteBatch()
        {
            SDG_ASSERT(spriteBatch_);
            return spriteBatch_;
        }

        [[nodiscard]]
        static const GameTime *GetTime()
        {
            SDG_ASSERT(gameTime_);
            return gameTime_;
        }

        [[nodiscard]]
        static ContentMgr *GetContent()
        {
            SDG_ASSERT(contentMgr_);
            return contentMgr_;
        }

        [[nodiscard]]
        static InputMgr *GetInput()
        {
            SDG_ASSERT(inputMgr_);
            return inputMgr_;
        }

        [[nodiscard]]
        static GraphicsDeviceMgr *GetGraphicsDeviceMgr()
        {
            SDG_ASSERT(graphics_);
            return graphics_;
        }

    private:
        // Members
        ubyte attr_;
        ComponentList *owner_;

        // Game-provided services
        static SpriteBatch *spriteBatch_;
        static const GameTime *gameTime_;
        static ContentMgr *contentMgr_;
        static InputMgr *inputMgr_;
        static GraphicsDeviceMgr *graphics_;
    };
}