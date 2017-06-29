#pragma once
#include "GL/glew.h"
#include "vector3D.h"
#include <vector>
#include <iostream>
#include <iosfwd>
#include "plane.h"
#include "AABB.h"
#include "matrix4D.h"
#include "cell.h"

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
	    void DrawBox(AABB box);
        void CollisionBox(Ray* r);
        void CollisionMeshPlanes(Ray* r);
		void PointOnMesh(Ray* r);
		void MoveMesh(matrix4D model);
		void DrawLine(vector3D v, vector3D p);
		void DrawArrows(Cell* c);
        Plane* CreateQuadPlane(vector3D p1, vector3D p2, vector3D p3, vector3D p4);

		std::vector<vector3D> vertices;
		std::vector<vector3D> uvs;
		std::vector<vector3D> normals;

        vector3D hitDir;
        vector3D hitPoint;

		std::vector<vector3D> vertColl;

		Ray* ra;
		Plane* pla;
        std::vector<Plane*> meshPlanes;
        AABB* box;
        bool hitByRay = false;
		bool collision = false;
		matrix4D model;

private:
	GLuint vbo, vao, uvb, ebo ,nb;
	int vertexAttributes, vertexBuffer, indexBuffer;
};
