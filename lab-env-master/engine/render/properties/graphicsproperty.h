#pragma once
#include "render/base/property.h"
#include <memory>
#include "core/math/vector3D.h"
#include "core/math/matrix3D.h"
#include "render/managers/lightmanager.h"
#include "render/resources/meshresource.h"

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


        void setLight(std::shared_ptr<Managers::LightManager> s);

        std::shared_ptr<Resources::MeshResource> mesh;
        std::shared_ptr<Managers::LightManager> light;

        float rad(float d)  //Degrees to rad
        {
            float rad = (PI / 180.f) * d;
            return rad;
        }

    public:
        const matrix4D &getProjec() const;

        matrix4D& getMod();

        void setProjec(const matrix4D &projec);
        void setMod(const matrix4D &mod);

    };
}

