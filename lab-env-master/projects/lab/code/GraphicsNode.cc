#include "GraphicsNode.h"
#include "config.h"
#include <ctime>




GraphicsNode::GraphicsNode()
{
	mesh = std::make_shared<Mesh>();
	shader = std::make_shared<ShaderObject>("VertexShader.vs", "FragmentShader.fs");
	light = std::make_shared<LightNode>();
	
	mesh->LoadMesh("crytek_sponza/sponza.obj");
	//mesh->LoadMesh("crytek_sponza/banner.obj");

    camera.setValues(0, -5, 5);
    origin.setValues(0.0f, 0.0f, 0.0f);
    headUp.setValues(0.0f, 1, 0.0f);
}

GraphicsNode::~GraphicsNode()
{
	
}

std::shared_ptr<Mesh> GraphicsNode::getMesh()
{
	return mesh;
}

void GraphicsNode::setMesh(std::shared_ptr<Mesh> m)
{
	mesh = m;
}

std::shared_ptr<TextureResource> GraphicsNode::getTexture()
{
	return texture;
}

void GraphicsNode::setTexture(std::shared_ptr<TextureResource> t)
{
	texture = t;
}

std::shared_ptr<ShaderObject> GraphicsNode::getShader()
{
	return shader;
}

void GraphicsNode::setShader(std::shared_ptr<ShaderObject> s)
{
	shader = s;
}

void GraphicsNode::drawOBJ(matrix4D projection, matrix4D view, matrix4D model)
{
	shader->bind();

	shader->modVector3f("in_lightPos", light->getPos());
	shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));

	shader->modMatrix4fv("projection", projection);
	shader->modMatrix4fv("view", view);
	shader->modMatrix4fv("model", model);


	mesh->Render();
	//mesh->DrawOBJ();

}

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



