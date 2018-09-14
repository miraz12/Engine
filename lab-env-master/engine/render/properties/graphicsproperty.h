#pragma once
#include "render/base/property.h"
#include <memory>
#include "core/math/matrix3D.h"
#include "render/managers/lightmanager.h"
#include "render/resources/meshresource.h"
#include "render/camera.h"

class matrix4D;

namespace Properties
{
    class GraphicsProperty : public Base::Property
    {
    public:

        /// constructor
        GraphicsProperty();
        /// destructor
        virtual ~GraphicsProperty();

        /// called on begin of frame
        virtual void OnBeginFrame();
        /// called from Entity::ActivateProperties()
        virtual void OnActivate();
        virtual void OnDeactivate();
        /// setup callbacks for this property, call by entity in OnActivate()
        virtual void SetupCallbacks();

        std::shared_ptr<Resources::MeshResource> mesh;

        float rad(float d)  //Degrees to rad
        {
            float rad = (PI / 180.f) * d;
            return rad;
        }

    private:
        matrix4D model;
        Display::Camera* cam;
    };
}

