#include "GraphicsNode.h"
#include "config.h"
#include <ctime>




GraphicsNode::GraphicsNode()
{
	meshA = std::make_shared<Mesh>();

	mesh = std::make_shared<MeshResource>();
	//texture = std::make_shared<TextureResource>("cat_diff.tga");
	shader = std::make_shared<ShaderObject>("VertexShader.vs", "FragmentShader.fs");
	light = std::make_shared<LightNode>();
	
	meshA->LoadMesh("crytek_sponza/sponza.obj");
	//texture->Load();
	//mesh->loadOBJ("cat.obj");

    camera.setValues(0, -5, 5);
    origin.setValues(0.0f, 0.0f, 0.0f);
    headUp.setValues(0.0f, 1, 0.0f);
}

GraphicsNode::GraphicsNode(std::shared_ptr<MeshResource> me, std::shared_ptr<TextureResource> tex, std::shared_ptr<ShaderObject> sha)
{
    mesh = me;
    texture = tex;
    shader = sha;
    light = std::make_shared<LightNode>();

	mesh->loadOBJ("cube.obj");

    camera.setValues(0, -5, 5);
    origin.setValues(0.0f, 0.0f, 0.0f);
    headUp.setValues(0.0f, 1, 0.0f);
}

GraphicsNode::~GraphicsNode()
{
	
}

std::shared_ptr<MeshResource> GraphicsNode::getMesh()
{
	return mesh;
}

void GraphicsNode::setMesh(std::shared_ptr<MeshResource> m)
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


void GraphicsNode::drawLines(matrix4D projection, matrix4D model)
{

	texture->bind();
	shader->bind();

	shader->modVector3f("in_lightPos", light->getPos());
	shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));

	shader->modMatrix4fv("projectionView", projection);
	shader->modMatrix4fv("model", model);
}

void GraphicsNode::draw(matrix4D projection, matrix4D model)
{

	texture->bind();
	shader->bind();

    if (mesh->hitByRay)
    {
        shader->modVector3f("planeHit", vector3D(1.0f, 0.0f, 1.0f));
    } else
    {
        shader->modVector3f("planeHit", vector3D(1.0f, 1.0f, 1.0f));

    }

	shader->modVector3f("in_lightPos", light->getPos());
	shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));
	
	shader->modMatrix4fv("projectionView", projection);
	shader->modMatrix4fv("model", model);

	mesh->DrawQuad();

}

void GraphicsNode::drawOBJ(matrix4D projection, matrix4D model)
{

	texture->bind();
	shader->bind();

	shader->modVector3f("in_lightPos", light->getPos());
	shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));

	shader->modMatrix4fv("projectionView", projection);
	shader->modMatrix4fv("model", model);

	mesh->DrawOBJ();

}

void GraphicsNode::drawOBJ(matrix4D projection, matrix4D view, matrix4D model)
{
	//texture->bind();
	shader->bind();

	shader->modVector3f("in_lightPos", light->getPos());
	shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));

	shader->modMatrix4fv("projection", projection);
	shader->modMatrix4fv("view", view);
	shader->modMatrix4fv("model", model);


	meshA->Render();
	//mesh->DrawOBJ();

}

void GraphicsNode::drawOBJPhysics(matrix4D projection, float force)
{
    texture->bind();
    shader->bind();
    vector3D color = vector3D(0.1f, 0.1f, 0.1f);

    if (mesh->collision)
    {
        //color = color + vector3D(0.0f, 0.0f, 1.0f);
        mesh->collision = false;
    }
    if (mesh->hitByRay)
    {
        physics->addForceAtPoint(force, mesh->hitDir, mesh->hitPoint);
        //physics->addForce(force, mesh->hitDir);
        mesh->hitByRay = false;
        //color = color + vector3D(1.0f, 0.0f, 1.0f);
    }
    //light->setColor(color);

    matrix4D model = this->physics->transformMatrix;
    this->mesh->MoveMesh(model);
    shader->modVector3f("planeHit", vector3D(1.0f, 1.0f, 1.0f));
    shader->modVector3f("in_lightPos", light->getPos());
    shader->modVector3f("in_color", (light->getColor() * light->getIntensity()));

    shader->modMatrix4fv("projectionView", projection);
    shader->modMatrix4fv("model", model);

    mesh->DrawOBJ();
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

std::shared_ptr<PhysicsNode> GraphicsNode::getPhysicsNode()
{
    return this->physics;
}

void GraphicsNode::setPhysicsNode(std::shared_ptr<PhysicsNode> p )
{
    this->physics = p;
}

void GraphicsNode::setLight(std::shared_ptr<LightNode> s)
{
    light = s;
}



