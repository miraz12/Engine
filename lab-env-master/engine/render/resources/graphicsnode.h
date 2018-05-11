#pragma once
#include "shaderobject.h"
#include "lightresource.h"
#include <memory>
#include "meshresource.h"

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

	std::shared_ptr<ShaderObject> getShader();
	void drawOBJ(matrix4D projection, matrix4D view, matrix4D model);

	std::shared_ptr<LightResource> getLightNode() { return light; };
    void setLight(std::shared_ptr<LightResource> s);


	void setupLights();
	void setupLight();

private:

		std::shared_ptr<MeshResource> mesh;
		std::shared_ptr<LightResource> light;

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

};

