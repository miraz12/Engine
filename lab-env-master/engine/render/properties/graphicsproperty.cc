#include "config.h"
#include "graphicsproperty.h"
#include "render/base/entity.h"


namespace  Properties
{
    GraphicsProperty::GraphicsProperty()
    {
        mesh = std::make_shared<Resources::MeshResource>();
        cam = Display::Camera::GetInstance();
        this->RTTI = "GrPr";
    }

    GraphicsProperty::~GraphicsProperty()
    {
    }

    void GraphicsProperty::OnBeginFrame()
    {

        mesh->getShader()->bind();
        mesh->getShader()->modVector3f("worldPos", vector3D(model.getPosition()));
        mesh->getShader()->modMatrix4fv("projection", cam->projection);
        mesh->getShader()->modMatrix4fv("view", cam->view);
        mesh->getShader()->modMatrix4fv("model", model);

		mesh->getShader()->mod1f("inDistToFocus", Display::Camera::GetInstance()->distToFocus);
		mesh->getShader()->mod1f("inFocalLen", Display::Camera::GetInstance()->focalLen);
		mesh->getShader()->mod1f("inAperture", Display::Camera::GetInstance()->aperture);

        mesh->Render();
        glUseProgram(0);
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


