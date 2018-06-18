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

    void Property::SetupDefaultAttributes()
    {
    }

    void Property::SetupExternalAttributes()
    {
    }

    void Property::SetupCallbacks()
    {
    }

    void Property::OnActivate()
    {
    }

    void Property::OnDeactivate()
    {
    }

    bool Property::IsActive() const
    {
        return active;
    }

    void Property::OnLoad()
    {
    }

    void Property::OnStart()
    {
    }

    void Property::OnSave()
    {
    }

    void Property::OnBeginFrame()
    {

    }

    void Property::OnMoveBefore()
    {
    }

    void Property::OnMoveAfter()
    {
    }

    void Property::OnRender()
    {
    }

    void Property::OnRenderDebug()
    {
    }

    void Property::OnLoseActivity()
    {
    }

    void Property::OnGainActivity()
    {
    }

    void Property::OnHandleDeferred()
    {
    }

    void Property::HandleDeferredMessages()
    {
    }

    void Property::SetEntity(Entity* v)
    {
    }

    void Property::ClearEntity()
    {
    }
}
