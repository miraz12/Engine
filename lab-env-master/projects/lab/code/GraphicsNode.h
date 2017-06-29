#pragma once
#include "GL/glew.h"
#include "ShaderObject.h"
#include "TextureResource.h"
#include "MeshResource.h"
#include "LightNode.h"
#include "physicsnode.h"
#include <memory>
#include "Mesh.h"

struct CollisionInfo
{
	double depth;
	vector3D normal;
	vector3D point;
	void* B;
};

class GraphicsNode
{
public:


	/// constructor
	GraphicsNode();
	GraphicsNode(std::shared_ptr<MeshResource> me, std::shared_ptr<TextureResource> tex, std::shared_ptr<ShaderObject> sha);
	/// destructor
	~GraphicsNode();

	std::shared_ptr<MeshResource> getMesh();
    std::shared_ptr<MeshResource> getMeshConst() const { return mesh; }
	void setMesh(std::shared_ptr<MeshResource>);

	std::shared_ptr<TextureResource> getTexture();
	void setTexture(std::shared_ptr<TextureResource>);

	std::shared_ptr<ShaderObject> getShader();
	void setShader(std::shared_ptr<ShaderObject>);

	std::shared_ptr<PhysicsNode> getPhysicsNode();
	void setPhysicsNode(std::shared_ptr<PhysicsNode>);

	std::shared_ptr<LightNode> getLightNode() { return light; };
    void setLight(std::shared_ptr<LightNode> s);



    void draw(matrix4D, matrix4D);
	void drawLines(matrix4D, matrix4D);
	void drawOBJ(matrix4D projection, matrix4D model);
	void drawOBJ(matrix4D projection, matrix4D view, matrix4D model);
	void drawOBJPhysics(matrix4D projection, float force);



private:
		std::shared_ptr<Mesh> meshA;


		std::shared_ptr<MeshResource> mesh;
		std::shared_ptr<TextureResource> texture;
		std::shared_ptr<ShaderObject> shader;
		std::shared_ptr<LightNode> light;
		std::shared_ptr<PhysicsNode> physics;

		matrix4D projec, mod;

public:
	const matrix4D &getProjec() const;

	matrix4D& getMod();
	vector3D camera;
	vector3D origin;
	vector3D headUp;
	std::vector<CollisionInfo> collInfVec;

	void setProjec(const matrix4D &projec);

	void setMod(const matrix4D &mod);




};

