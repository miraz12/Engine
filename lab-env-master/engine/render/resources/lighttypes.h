#pragma once
#include "core/math/vector3D.h"
#include "shaderobject.h"
#include <memory>
#include <string.h>

#ifdef __WIN32__
#define snprintf _snprintf_s
#endif

struct BaseLight
{
	vector3D Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight()
	{
		Color = vector3D(0.0f, 0.0f, 0.0f);
		AmbientIntensity = 0.0f;
		DiffuseIntensity = 0.0f;
	}

    virtual void Setup(std::shared_ptr<Resources::ShaderObject>, int i = -1) = 0;
};

struct DirectionalLight : public BaseLight
{
    vector3D Direction;

    DirectionalLight()
    {
        Direction = vector3D(0.0f, 0.0f, 0.0f);

    }
    ~DirectionalLight()
    {
        
    }

    void Setup(std::shared_ptr<Resources::ShaderObject> s, int i = -1)
	{
		i;
        s->modVector3f("gDirectionalLight.Base.Color", vector3D(this->Color.x(), this->Color.y(), this->Color.z()));
        s->mod1f("gDirectionalLight.Base.AmbientIntensity", this->AmbientIntensity);
        vector3D dir = Direction;
		dir.normalize();
        s->modVector3f("gDirectionalLight.Direction", vector3D(dir.x(), dir.y(), dir.z()));
        s->mod1f("gDirectionalLight.Base.DiffuseIntensity", this->DiffuseIntensity);
	}
};

struct PointLight : public BaseLight
{
	vector3D Position;

	struct
	{
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight()
	{
		Position = vector3D(0.0f, 0.0f, 0.0f);
		Attenuation.Constant = 1.0f;
		Attenuation.Linear = 0.0f;
		Attenuation.Exp = 0.0f;
	}
    void Setup(std::shared_ptr<Resources::ShaderObject> s, int i)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
        s->modVector3f(Name, vector3D(this->Color.x(), this->Color.y(), this->Color.z()));

        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
        s->mod1f(Name, this->AmbientIntensity);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Position", i);
        s->modVector3f(Name, vector3D(this->Position.x(), this->Position.y(), this->Position.z()));

        snprintf(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
        s->mod1f(Name, this->DiffuseIntensity);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
        s->mod1f(Name, this->Attenuation.Constant);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
        s->mod1f(Name, this->Attenuation.Linear);

        snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
        s->mod1f(Name, this->Attenuation.Exp);
    }
};

struct SpotLight : public PointLight
{
	vector3D Direction;
	float Cutoff;

	SpotLight()
	{
		Direction = vector3D(0.0f, 0.0f, 0.0f);
		Cutoff = 0.0f;
	}
    void Setup(std::shared_ptr<Resources::ShaderObject> s, int i)
    {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
        s->modVector3f(Name, vector3D(this->Color.x(), this->Color.y(), this->Color.z()));

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
        s->mod1f(Name, this->AmbientIntensity);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
        s->modVector3f(Name, vector3D(this->Position.x(), this->Position.y(), this->Position.z()));

        vector3D dir = this->Direction;
        dir.normalize();

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
        s->modVector3f(Name, vector3D(dir.x(), dir.y(), dir.z()));

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
        s->mod1f(Name, cosf((PI / 180.f) * (this->Cutoff))); //Convert to radians

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
        s->mod1f(Name, this->DiffuseIntensity);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
        s->mod1f(Name, this->Attenuation.Constant);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
        s->mod1f(Name, this->Attenuation.Linear);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
        s->mod1f(Name, this->Attenuation.Exp);
    }
};

