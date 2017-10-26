#include "GraphicsNode.h"
#include "config.h"
#include <ctime>




GraphicsNode::GraphicsNode()
{
	mesh = std::make_shared<Mesh>();
	//shader = std::make_shared<ShaderObject>("VertexShader.vs", "FragmentShader.fs");
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

	mesh->shader->modVector3f("in_lightPos", light->getPos());
	mesh->shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));
	mesh->shader->modVector3f("activate_normal", vector3D(float(activateNormal), 0.0f, 0.0f));


	mesh->shader->modMatrix4fv("projection", projection);
	mesh->shader->modMatrix4fv("view", view);
	mesh->shader->modMatrix4fv("model", model);

	mesh->Render();

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

void GraphicsNode::setupLight()
{

}




