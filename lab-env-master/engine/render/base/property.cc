#include "config.h"
#include "property.h"

namespace Base
{
    Property::Property() : active(false)
    {
    }

    Property::~Property()
    {
    }

    const Entity& Property::GetEntity() const
    {
        return *entity;
    }

    bool Property::HasEntity() const
    {
        return entity == NULL;
    }

    void Property::SetupCallbacks()
    {
        // empty
    }

    void Property::OnActivate()
    {
        this->active = true;
    }

    void Property::OnDeactivate()
    {
        this->active = false;
    }

    bool Property::IsActive() const
    {
        return active;
    }

    void Property::OnLoad()
    {
        // empty
    }

    void Property::OnStart()
    {
        // empty
    }

    void Property::OnBeginFrame()
    {
        // empty
    }

    void Property::OnRender()
    {
        // empty
    }

    void Property::SetEntity(Entity* v)
    {
        this->entity = v;
    }

    void Property::ClearEntity()
    {
        this->entity = 0;
    }
}
