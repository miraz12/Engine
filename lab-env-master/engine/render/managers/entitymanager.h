#pragma once
#include "render/resources/graphicsnode.h"
#include "lightmanager.h"
#include "render/base/manager.h"
#include <unordered_map>
#include "render/base/entity.h"

namespace Managers
{
    class EntityManager : Base::Manager
    {
    public:
        EntityManager();
        ~EntityManager();



        /// called when removed from game server
        virtual void OnDeactivate();
        /// called on start by game server
        void OnStart();
        /// called per-frame by game server
        virtual void OnBeginFrame();
        /// called per-frame by game server
        virtual void OnEndFrame();
        /// called after level loaded
        virtual void OnLoad();
        /// called before saving game state
        virtual void OnSave();
        /// called if a render debug visulalization is requested
        void OnRenderDebug();
        /// cleanup the entity manager, removes/deactivates all entities
        void Cleanup();
        /// return true if at least one active entity exists in the world
        bool HasActiveEntities() const;
        /// get currently active entities
        const std::vector<Base::Entity*>& GetEntities() const;


    private:


        std::unordered_map<Base::Entity::EntityId, Base::Entity*> entityRegistry;  // map unique ids to entities
        std::vector<Base::Entity*> activeEntities;                                  // currently active entities

    };
}
