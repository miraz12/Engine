#include "GraphicsNode.h"
#include "config.h"
#include <ctime>




GraphicsNode::GraphicsNode()
{
	mesh = std::make_shared<Mesh>();
	light = std::make_shared<LightNode>();
	
    camera.setValues(0, -5, 5);
    origin.setValues(0.0f, 0.0f, 0.0f);
    headUp.setValues(0.0f, 1, 0.0f);
}

GraphicsNode::~GraphicsNode()
{
	
}

void GraphicsNode::drawOBJ(matrix4D projection, matrix4D view, matrix4D model)
{
	mesh->shader->bind();



	mesh->shader->mod1i("gNumPointLights", (light->m_pLights.size()));
	mesh->shader->mod1i("gNumSpotLights", (light->m_sLights.size()));
	setupLights();

	mesh->shader->modVector3f("in_lightPos", light->getPos());
	mesh->shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));
	mesh->shader->modVector3f("activate_normal", vector3D(float(activateNormal), 0.0f, 0.0f));


	mesh->shader->modVector3f("cameraPos", vector3D(camera.x(), camera.y(), camera.z()));
	mesh->shader->modVector3f("worldPos", vector3D(model.getPosition()));
	mesh->shader->modMatrix4fv("projection", projection);
	mesh->shader->modMatrix4fv("view", view);
	mesh->shader->modMatrix4fv("model", model);

	mesh->Render();

}

void GraphicsNode::drawTerrain(matrix4D projection, matrix4D view, matrix4D model)
{
	mesh->shader->bind();

	mesh->shader->mod1i("gNumPointLights", (light->m_pLights.size()));
	mesh->shader->mod1i("gNumSpotLights", (light->m_sLights.size()));
	setupLights();

	mesh->shader->modVector3f("in_lightPos", light->getPos());
	mesh->shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));
	mesh->shader->modVector3f("activate_normal", vector3D(float(activateNormal), 0.0f, 0.0f));


	mesh->shader->modVector3f("cameraPos", vector3D(camera.x(), camera.y(), camera.z()));
	mesh->shader->modVector3f("worldPos", vector3D(model.getPosition()));
	mesh->shader->modMatrix4fv("projection", projection);
	mesh->shader->modMatrix4fv("view", view);
	mesh->shader->modMatrix4fv("model", model);

	mesh->RenderTerrain();
}


std::shared_ptr<Mesh> GraphicsNode::getMesh()
{
	return mesh;
}

void GraphicsNode::setMesh(std::shared_ptr<Mesh> m)
{
	mesh = m;
}

std::shared_ptr<ShaderObject> GraphicsNode::getShader()
{
	return mesh->shader;
}

//void GraphicsNode::setShader(std::shared_ptr<ShaderObject> s)
//{
//	shader = s;
//}

void GraphicsNode::setProjec(const matrix4D &projec) {
	GraphicsNode::projec = projec;
}

void GraphicsNode::setMod(const matrix4D &mod) {
	GraphicsNode::mod = mod;
}
const matrix4D &GraphicsNode::getProjec() const {
	return projec;
}

matrix4D &GraphicsNode::getMod() {
	return mod;
}

void GraphicsNode::setLight(std::shared_ptr<LightNode> s)
{
    light = s;
}

void GraphicsNode::setupLights()
{

	for (int i = 0; i < light->m_dLights.size(); i++)
	{
		mesh->shader->modVector3f("gDirectionalLight.Base.Color", vector3D(light->m_dLights[i].Color.x(), light->m_dLights[i].Color.y(), light->m_dLights[i].Color.z()));
		mesh->shader->mod1f("gDirectionalLight.Base.AmbientIntensity", light->m_dLights[i].AmbientIntensity);
		vector3D Direction = light->m_dLights[i].Direction;
		Direction.normalize();
		mesh->shader->modVector3f("gDirectionalLight.Direction", vector3D(Direction.x(), Direction.y(), Direction.z()));
		mesh->shader->mod1f("gDirectionalLight.Base.DiffuseIntensity", light->m_dLights[i].DiffuseIntensity);
	}

	for (int i = 0; i < light->m_pLights.size(); ++i)
	{

		char Name[128];
		memset(Name, 0, sizeof(Name));
		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
		mesh->shader->modVector3f(Name, vector3D(light->m_pLights[i].Color.x(), light->m_pLights[i].Color.y(), light->m_pLights[i].Color.z()));

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
		mesh->shader->mod1f(Name, light->m_pLights[i].AmbientIntensity);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Position", i);
		mesh->shader->modVector3f(Name, vector3D(light->m_pLights[i].Position.x(), light->m_pLights[i].Position.y(), light->m_pLights[i].Position.z()));

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
		mesh->shader->mod1f(Name, light->m_pLights[i].DiffuseIntensity);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
		mesh->shader->mod1f(Name, light->m_pLights[i].Attenuation.Constant);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
		mesh->shader->mod1f(Name, light->m_pLights[i].Attenuation.Linear);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
		mesh->shader->mod1f(Name, light->m_pLights[i].Attenuation.Exp);
	}

	for (int i = 0; i < light->m_sLights.size(); ++i)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
		mesh->shader->modVector3f(Name, vector3D(light->m_sLights[i].Color.x(), light->m_sLights[i].Color.y(), light->m_sLights[i].Color.z()));

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
		mesh->shader->mod1f(Name, light->m_sLights[i].AmbientIntensity);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
		mesh->shader->modVector3f(Name, vector3D(light->m_sLights[i].Position.x(), light->m_sLights[i].Position.y(), light->m_sLights[i].Position.z()));

		vector3D Direction = light->m_sLights[i].Direction;
		Direction.normalize();

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
		mesh->shader->modVector3f(Name, vector3D(Direction.x(), Direction.y(), Direction.z()));

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
		mesh->shader->mod1f(Name, cosf(rad(light->m_sLights[i].Cutoff)));

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
		mesh->shader->mod1f(Name, light->m_sLights[i].DiffuseIntensity);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
		mesh->shader->mod1f(Name, light->m_sLights[i].Attenuation.Constant);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
		mesh->shader->mod1f(Name, light->m_sLights[i].Attenuation.Linear);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
		mesh->shader->mod1f(Name, light->m_sLights[i].Attenuation.Exp);

	}

	mesh->shader->mod1f("gMatSpecularIntensity", light->specIntensity);
	mesh->shader->mod1f("gSpecularPower", light->specPower);
}

void GraphicsNode::setupLight()
{

}




