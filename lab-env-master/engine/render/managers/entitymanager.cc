#include "config.h"
#include "entitymanager.h"

namespace Managers
{
    EntityManager::EntityManager()
    {
    }

    EntityManager::~EntityManager()
    {
    }

    void EntityManager::AttachEntity(Base::Entity* entity)
    {

        this->activeEntities.push_back(entity);
        //this->untriggeredEntities.Append(entity);
        this->entityRegistry.insert({ entity->GetUniqueId(), entity });
        // activate entity
        entity->OnActivate();
    }

    void EntityManager::RemoveEntity(Base::Entity* entity)
    {
    }

    void EntityManager::DeleteEntity(Base::Entity* entity)
    {
    }

    void EntityManager::OnDeactivate()
    {
    }

    void EntityManager::OnStart()
    {
        for (size_t i = 0; i < this->activeEntities.size(); i++)
        {
            activeEntities[i]->OnStart();
        }
    }

    void EntityManager::OnBeginFrame()
    {
        for (size_t i = 0; i < this->activeEntities.size(); i++)
        {
            activeEntities[i]->OnBeginFrame();
        }
    }

    

    void EntityManager::OnEndFrame()
    {
    }

    void EntityManager::OnLoad()
    {
    }

    void EntityManager::OnSave()
    {
    }

    void EntityManager::OnRenderDebug()
    {
    }

    void EntityManager::Cleanup()
    {
    }

    bool EntityManager::HasActiveEntities() const
    {
        return this->activeEntities.size() > 0;
    }

    const std::vector<Base::Entity*>& EntityManager::GetEntities() const
    {
        return this->GetEntities();
    }
}
