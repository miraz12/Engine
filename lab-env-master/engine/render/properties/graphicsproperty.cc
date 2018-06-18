#include "config.h"
#include "graphicsproperty.h"
#include "render/base/entity.h"
#include "render/camera.h"


namespace  Properties
{
    GraphicsProperty::GraphicsProperty()
    {
        mesh = std::make_shared<Resources::MeshResource>();
        light = std::make_shared<Managers::LightManager>();

    }

    GraphicsProperty::~GraphicsProperty()
    {
    }

    void GraphicsProperty::OnBeginFrame()
    {
        mesh->shader->bind();

        Managers::LightManager::GetInstance()->Setup(mesh->shader);

        Display::Camera* cam = Display::Camera::GetInstance();
        cam->view = cam->view.LookAtRH(cam->position, cam->position + cam->camFront, cam->headUp);

        mesh->shader->modVector3f("cameraPos", vector3D(cam->position.x(), cam->position.y(), cam->position.z()));
        mesh->shader->modVector3f("worldPos", vector3D(mesh->model.getPosition()));
        mesh->shader->modMatrix4fv("projection", cam->projection);
        mesh->shader->modMatrix4fv("view", cam->view);
        mesh->shader->modMatrix4fv("model", mesh->model);
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


