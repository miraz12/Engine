#include "config.h"
#include "entity.h"

namespace Base
{
    unsigned int Entity::uniqueIdCounter = 0;


    Entity::Entity() :
        uniqueId(++uniqueIdCounter),
        activated(false),
        isInOnActivate(false),
        isInOnDeactivate(false)
    {

    }

    Entity::~Entity()
    {
    }

    const std::string& Entity::GetCategory() const
    {
        return category;
    }

    void Entity::OnActivate()
    {
    }

    void Entity::OnDeactivate()
    {
    }

    void Entity::OnBeginFrame()
    {
    }

    void Entity::OnMoveBefore()
    {
    }

    void Entity::OnMoveAfter()
    {
    }

    void Entity::OnRender()
    {
        std::vector<Property*> props = callbackProperties[Property::BeginFrame];

        for (int i = 0; i < props.size(); ++i)
        {
            props[i]->BeginFrame;
        }


    }

    void Entity::OnRenderDebug()
    {
    }

    void Entity::OnLoseActivity()
    {
    }

    void Entity::OnGainActivity()
    {
    }

    void Entity::OnHandleDeferred()
    {
    }

    void Entity::OnLoad()
    {
    }

    void Entity::OnStart()
    {
    }

    void Entity::OnSave()
    {
    }

    Entity::EntityId Entity::GetUniqueId() const
    {
        return this->uniqueId;
    }

    bool Entity::IsActive() const
    {
        return activated;
    }
}
