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
        callbackProperties.reserve(8);

    }

    Entity::~Entity()
    {
    }

    void Entity::AttachProperty(Property* prop)
    {
        this->properties.push_back(prop);
        prop->SetEntity(this);
    }

    void Entity::RemoveProperty(Property* prop)
    {

    }


    void Entity::RegisterPropertyCallback(Property* prop, Property::CallbackType callback)
    {
        this->callbackProperties[callback].push_back(prop);
    }

    const std::string& Entity::GetCategory() const
    {
        return category;
    }

    void Entity::OnActivate()
    {
        // activate all properties
        this->ActivateProperties();
    }

    void Entity::OnDeactivate()
    {
    }

    void Entity::OnBeginFrame()
    {
        std::vector<Property*> props = callbackProperties[Property::BeginFrame];

        for (size_t i = 0; i < props.size(); ++i)
        {
            props[i]->OnBeginFrame();
        }
    }

    void Entity::OnMoveBefore()
    {
    }

    void Entity::OnMoveAfter()
    {
    }

    void Entity::OnRender()
    {

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

    void Entity::ActivateProperties()
    {
        for (size_t i = 0; i < this->properties.size(); i++)
        {
            Property* prop = this->properties[i];
        
            prop->SetupCallbacks();

            // activate property
            prop->OnActivate();

        }

    }

    void Entity::DeactivateProperties()
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
