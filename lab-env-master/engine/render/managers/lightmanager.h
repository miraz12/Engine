#pragma once
#include "core/math/vector3D.h"
#include "render/resources/lighttypes.h"
#include "core/math/matrix3D.h"
#include "GL/glew.h"
#include <vector>

namespace Managers
{
    class LightManager
    {

    public:
        static const unsigned int MAX_POINT_LIGHTS = 2;
        static const unsigned int MAX_SPOT_LIGHTS = 2;

        LightManager();
        static LightManager* GetInstance();
        LightManager(const LightManager&) = delete;
        void operator=(const LightManager) = delete;

        void Setup(std::shared_ptr<Resources::ShaderObject> s);

        void AddDirectionalLight(vector3D color, float ambientintensity, float diffuseintensity, vector3D direction);
        void AddPointLight(vector3D color, float diffuseintensity, vector3D position, float attenuationConst,
            float attenuationLin, float attenuationExp);
        void AddSpotLight(vector3D color, float diffuseintensity, vector3D position, vector3D direction,
            float attenuationConst, float attenuationLin, float attenuationExp, float cutoff);

        void AddDirectionalLight(DirectionalLight* dirLight);
        void AddPointLight(PointLight* pLight);
        void AddSpotLight(SpotLight* sLight);


        ~LightManager();

        vector3D getPos();
        void setPos(vector3D);

        vector3D getColor();
        void setColor(vector3D);

        float getIntensity();
        void setIntensity(float);

        matrix3D m_matSpecularIntensityLocation;
        matrix3D m_matSpecularPowerLocation;
        int m_numPointLightsLocation;
        int m_numSpotLightsLocation;

        DirectionalLight* m_dLight;

        std::vector<DirectionalLight*> m_dLights;
        std::vector<SpotLight*> m_sLights;
        std::vector<PointLight*> m_pLights;


        float specIntensity = 0.0f;
        float specPower = 0.0f;

    private:

        vector3D color;
        vector3D pos;
        float intens;

        static LightManager* instance;

    };
}
