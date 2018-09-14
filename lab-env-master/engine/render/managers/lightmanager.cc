#include "config.h"
#include "lightmanager.h"
#include "render/camera.h"


namespace Managers
{
    LightManager* LightManager::instance = 0;


    LightManager::LightManager()
    {
        color = vector3D(0.1f, 0.1f, 0.1f);
        pos = vector3D(1.0, 1.0, 1.0);
        intens = 1.0f;

        specIntensity = 0.5f;
        specPower = 15.0f;

        m_dLights = std::vector<DirectionalLight>();

    }

    void LightManager::Setup(std::shared_ptr<Resources::ShaderObject> s)
    {
        Display::Camera* cam = Display::Camera::GetInstance();
        s->modVector3f("cameraPos", vector3D(cam->position.x(), cam->position.y(), cam->position.z()));
        s->mod1i("gNumPointLights", (int(m_pLights.size())));
        s->mod1i("gNumSpotLights", (int(m_sLights.size())));

        for (unsigned int i = 0; i < m_dLights.size(); i++)
        {
            m_dLights[i].Setup(s, i);
        }

        for (unsigned int i = 0; i < m_pLights.size(); ++i)
        {
            m_pLights[i].Setup(s, i);
        }

        for (unsigned int i = 0; i < m_sLights.size(); ++i)
        {
            m_sLights[i].Setup(s, i);
        }

        s->mod1f("gMatSpecularIntensity", specIntensity);
        s->mod1f("gSpecularPower", specPower);

        s->modVector3f("in_lightPos", pos);
        s->modVector3f("in_color", (color * intens));

    }

    void LightManager::AddDirectionalLight(vector3D color, float ambientintensity, float diffuseintensity,
        vector3D direction)
    {
        DirectionalLight dirLight = DirectionalLight();
        dirLight.Color = color;
        dirLight.AmbientIntensity = ambientintensity;
        dirLight.DiffuseIntensity = diffuseintensity;
        dirLight.Direction = direction;
        this->m_dLights.push_back(dirLight);
    }

    void LightManager::AddPointLight(vector3D color, float diffuseintensity, vector3D position, float attenuationConst,
        float attenuationLin, float attenuationExp)
    {
        PointLight pLight = PointLight();
        pLight.DiffuseIntensity = diffuseintensity; // 0.25f;
        pLight.Color = color;
        pLight.Position = position;
        pLight.Attenuation.Constant = attenuationConst;
        pLight.Attenuation.Linear = attenuationLin;
        pLight.Attenuation.Exp = attenuationExp;
        this->m_pLights.push_back(pLight);
    }

    void LightManager::AddSpotLight(vector3D color, float diffuseintensity, vector3D position, vector3D direction, float attenuationConst,
        float attenuationLin, float attenuationExp, float cutoff)
    {
        SpotLight sLight = SpotLight();
        sLight.DiffuseIntensity = diffuseintensity;
        sLight.Color = color;
        sLight.Position = position;
        sLight.Direction = direction;
        sLight.Attenuation.Constant = attenuationConst;
        sLight.Attenuation.Linear = attenuationLin;
        sLight.Attenuation.Exp = attenuationExp;
        sLight.Cutoff = cutoff;
    }


    LightManager::~LightManager()
    {
    }

    vector3D LightManager::getPos()
    {
        return pos;
    }

    void LightManager::setPos(vector3D p)
    {
        pos = p;
    }

    vector3D LightManager::getColor()
    {
        return color;
    }

    void LightManager::setColor(vector3D c)
    {
        color = c;
    }

    float LightManager::getIntensity()
    {
        return intens;
    }

    void LightManager::setIntensity(float i)
    {
        intens = i;
    }

    LightManager* LightManager::GetInstance()
    {
        if (instance == 0)
        {
            instance = new LightManager();
        }
        return instance;
    }
}
