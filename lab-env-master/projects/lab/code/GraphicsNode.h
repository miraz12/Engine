#pragma once
#include "ShaderObject.h"
#include "LightNode.h"
#include <memory>
#include "Mesh.h"

class GraphicsNode
{
public:


	/// constructor
	GraphicsNode();
	GraphicsNode(std::shared_ptr<Mesh> me, std::shared_ptr<TextureResource> tex, std::shared_ptr<ShaderObject> sha);
	/// destructor
	~GraphicsNode();

	std::shared_ptr<Mesh> getMesh();
	std::shared_ptr<Mesh> getMeshConst() const { return mesh; }
	void setMesh(std::shared_ptr<Mesh>);

	std::shared_ptr<ShaderObject> getShader();
	void setShader(std::shared_ptr<ShaderObject>);
	void drawOBJ(matrix4D projection, matrix4D view, matrix4D model);

	std::shared_ptr<LightNode> getLightNode() { return light; };
    void setLight(std::shared_ptr<LightNode> s);


	void setupLights();

	void setupLight();
	void setupText();

private:

		std::shared_ptr<Mesh> mesh;
		//std::shared_ptr<TextureResource> texture;
		//std::shared_ptr<ShaderObject> shader;
		std::shared_ptr<LightNode> light;

		matrix4D projec, mod;

		float rad(float d)  //Degrees to rad
		{
			float rad = (PI / 180.f) * d;
			return rad;
		}

public:
	const matrix4D &getProjec() const;

	matrix4D& getMod();
	vector3D camera;
	vector3D origin;
	vector3D headUp;

	void setProjec(const matrix4D &projec);
	void setMod(const matrix4D &mod);


	//TODO:
	int activateNormal = 1;

};

