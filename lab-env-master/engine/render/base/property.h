#pragma once

namespace Base
{
    class Entity;

    class Property
    {
    public:
        /// callback types
        enum CallbackType
        {
            BeginFrame = 0,             // please call Property::OnBeginFrame()
            MoveBefore,                 // please call Property::OnMoveBefore()
            MoveAfter,                  // please call Property::OnMoveAfter()
            Render,                     // please call Property::OnRender()
            RenderDebug,                // please call Property::OnRenderDebug()
            LoseActivity,               // please call Property::OnLoseActivity()
            GainActivity,               // please call Property::OnGainActivity()
            HandleDeferred,				// please call Property::OnHandleDeferred()

            NumCallbackTypes,           // KEEP AT END
        };


        /// constructor
        Property();
        /// destructor
        virtual ~Property();

        /// get entity this property is attached to
        const Entity& GetEntity() const;
        /// return true if entity pointer is valid
        bool HasEntity() const;

        /// setup callbacks for this property, call by entity in OnActivate()
        virtual void SetupCallbacks();

        /// called from Entity::ActivateProperties()
        virtual void OnActivate();
        /// called from Entity::DeactivateProperties()
        virtual void OnDeactivate();
        /// return true if property is currently active
        bool IsActive() const;

        /// called from within Entity::Load() after attributes are loaded
        virtual void OnLoad();
        /// called from within Entity::OnStart() after OnLoad when the complete world exist
        virtual void OnStart();

        /// called on begin of frame
        virtual void OnBeginFrame();
        /// called before rendering happens
        virtual void OnRender();

        friend class Entity;
        /// Set entity, this is attached to, to `v'.
        void SetEntity(Entity* v);
        /// Remove entity.
        void ClearEntity();

        Entity* entity;
        bool active;
    };
}