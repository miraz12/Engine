#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <vector>
#include "GL/glew.h"
#include "vector3D.h"
#include "vector2D.h"
#include "TextureResource.h"

#define INVALID_MATERIAL 0xFFFFFFFF


struct Vertex
{
	vector3D m_pos;
	vector2D m_tex;
	vector3D m_normal;

	Vertex() {}

	Vertex(const vector3D& pos, const vector2D& tex, const vector3D& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

class Mesh 
	{
	public:
		/// constructor
		Mesh();
		/// destructor
		~Mesh();

		bool LoadMesh(const std::string& Filename);

		void Render();

	private:
		bool InitFromScene(const aiScene* pScene, const std::string& Filename);
		void InitMesh(unsigned int Index, const aiMesh* paiMesh);
		bool InitMaterials(const aiScene* pScene, const std::string& Filename);
		void Clear();


		struct MeshEntry {
			MeshEntry();

			~MeshEntry();

			void Init(const std::vector<Vertex>& Vertices,
				const std::vector<unsigned int>& Indices);

			GLuint VB;
			GLuint IB;
			unsigned int NumIndices;
			unsigned int MaterialIndex;
		};

		std::vector<MeshEntry> m_Entries;
		std::vector<TextureResource*> m_Textures;
};
