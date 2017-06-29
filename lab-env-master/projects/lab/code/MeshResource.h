#pragma once
#include "GL/glew.h"
#include "vector3D.h"
#include <vector>
#include <iostream>
#include <iosfwd>
#include "matrix4D.h"

class Ray;

class MeshResource 
	{
	public:
		/// constructor
		MeshResource();
		/// destructor
		~MeshResource();

		void Setup();
		void DrawCube();
		void DrawOBJ();
		bool LoadObjAssimp(const char * path);
		bool loadOBJ(const char* filename);
		void DrawQuad();
		void DrawLine();
		void DrawPoint(vector3D p);
	    void DrawBox();
        void CollisionBox(Ray* r);
        void CollisionMeshPlanes(Ray* r);
		void PointOnMesh(Ray* r);
		void MoveMesh(matrix4D model);
		void DrawLine(vector3D v, vector3D p);

		std::vector<vector3D> vertices;
		std::vector<vector3D> uvs;
		std::vector<vector3D> normals;

        vector3D hitDir;
        vector3D hitPoint;

		std::vector<vector3D> vertColl;

		Ray* ra;
        bool hitByRay = false;
		bool collision = false;
		matrix4D model;

private:
	GLuint vbo, vao, uvb, ebo ,nb;
	int vertexAttributes, vertexBuffer, indexBuffer;
};
