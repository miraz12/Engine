#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <vector>
#include "GL/glew.h"
#include "core/math/vector3D.h"
#include "core/math/vector2D.h"
#include "render/resources/textureresource.h"
#include "core/math/matrix4D.h"
#include "render/resources/shaderobject.h"
#include <memory>

#define INVALID_MATERIAL 0xFFFFFFFF

struct Vertex
{
	vector3D m_pos;
	vector2D m_tex;
	vector3D m_normal;
	vector3D m_tangents;
	vector3D m_bitangents;

	Vertex() {}

	Vertex(const vector3D& pos, const vector2D& tex, const vector3D& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

namespace Resources
{

    class MeshResource
    {
    public:
        /// constructor
        MeshResource();
        /// destructor
        ~MeshResource();

        bool LoadMesh(const std::string& Filename);
        void Render();
        matrix4D getMM() { return modelMatrix; };
        void setMM(matrix4D m) { modelMatrix = m; };

        std::shared_ptr<ShaderObject> shader;

        matrix4D model;


    private:
        bool InitFromScene(const aiScene* pScene, const std::string& Filename);
        void InitMesh(unsigned int Index, const aiMesh* paiMesh);
        bool InitMaterials(const aiScene* pScene, const std::string& Filename);
        void Clear();
        void computeTangentBasis(std::vector<Vertex>& vertices, std::vector<unsigned int>& ind);


        struct MeshEntry {
            MeshEntry();

            ~MeshEntry();

            void Init(const std::vector<Vertex>& Vertices,
                const std::vector<unsigned int>& Indices);

            GLuint VB;
            GLuint IB;
            //GLuint TB;
            //GLuint BTB;
            unsigned int NumIndices;
            unsigned int MaterialIndex;
        };

        std::vector<MeshEntry> m_Entries;
        std::vector<TextureResource*> m_Textures;
        std::vector<TextureResource*> m_Normals;
        std::vector<TextureResource*> m_Masks;
        std::vector<TextureResource*> m_Spec;
        matrix4D modelMatrix;
        TextureResource* defaulNormal;
        TextureResource* defaultDiff;
        TextureResource* defaulMask;

    };

}
