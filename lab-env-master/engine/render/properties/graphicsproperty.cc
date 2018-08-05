#include "config.h"
#include "graphicsproperty.h"
#include "render/base/entity.h"
#include "render/camera.h"


namespace  Properties
{
    GraphicsProperty::GraphicsProperty()
    {
        mesh = std::make_shared<Resources::MeshResource>();

    }

    GraphicsProperty::~GraphicsProperty()
    {
    }

    void GraphicsProperty::OnBeginFrame()
    {

        mesh->getShader()->bind();

        Display::Camera* cam = Display::Camera::GetInstance();
        cam->view = cam->view.LookAtRH(cam->position, cam->position + cam->camFront, cam->headUp);
        mesh->getShader()->modVector3f("worldPos", vector3D(model.getPosition()));
        mesh->getShader()->modMatrix4fv("projection", cam->projection);
        mesh->getShader()->modMatrix4fv("view", cam->view);
        mesh->getShader()->modMatrix4fv("model", model);
        mesh->Render();
    }

    void GraphicsProperty::OnActivate()
    {
    }

    void GraphicsProperty::OnDeactivate()
    {
    }

    void GraphicsProperty::SetupCallbacks()
    {
        this->entity->RegisterPropertyCallback(this, BeginFrame);
    }

}


