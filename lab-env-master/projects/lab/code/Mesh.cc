#include "Mesh.h"

#define INVALID_OGL_VALUE 0xffffffff

Mesh::MeshEntry::MeshEntry()
{
	VB = INVALID_OGL_VALUE;
	IB = INVALID_OGL_VALUE;
	NumIndices = 0;
	MaterialIndex = INVALID_MATERIAL;
};

Mesh::MeshEntry::~MeshEntry()
{
	if (VB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &VB);
	}

	if (IB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &IB);
	}
}

void Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices,	const std::vector<unsigned int>& Indices)
{
	NumIndices = Indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* NumIndices, &Indices[0], GL_STATIC_DRAW);

}

Mesh::Mesh()
{
	shader = std::make_shared<ShaderObject>("content/Shader/VertexShader.vs", "content/Shader/lighting.fs");

	defaultDiff = new TextureResource("content/test.png");
	defaultDiff->Load();

	defaulNormal = new TextureResource("content/DefaultNormalMap.png");
	defaulNormal->Load();
}


Mesh::~Mesh()
{
	Clear();
}

void Mesh::Clear()
{

}

bool Mesh::LoadMesh(const std::string& Filename)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	printf("Loading: '%s'\n", Filename.c_str());

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (pScene) {
		Ret = InitFromScene(pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}

	return Ret;
}


bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);
	m_Normals.resize(pScene->mNumMaterials);
	m_Masks.resize(pScene->mNumMaterials);
	m_Spec.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return InitMaterials(pScene, Filename);
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
		Vertex v(vector3D(pPos->x, pPos->y, pPos->z),
				vector2D(pTexCoord->x, pTexCoord->y),
				vector3D(pNormal->x, pNormal->y, pNormal->z));

		Vertices.push_back(v);
	}


	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	computeTangentBasis(Vertices, Indices);
	m_Entries[Index].Init(Vertices, Indices);


}

bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;
	


	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = nullptr;
		m_Normals[i] = nullptr;
		m_Masks[i] = nullptr;
		m_Spec[i] = nullptr;


		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				m_Textures[i] = new TextureResource(FullPath.c_str());

				if (!m_Textures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
		}
		if (pMaterial->GetTextureCount(aiTextureType_HEIGHT) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_HEIGHT, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				m_Normals[i] = new TextureResource(FullPath.c_str());

				if (!m_Normals[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Normals[i];
					m_Normals[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded NormalMapp '%s'\n", FullPath.c_str());
				}
			}
		}
		if (pMaterial->GetTextureCount(aiTextureType_OPACITY) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_OPACITY, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				m_Masks[i] = new TextureResource(FullPath.c_str());

				if (!m_Masks[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Masks[i];
					m_Masks[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded Mask '%s'\n", FullPath.c_str());
				}
			}
		}
		if (pMaterial->GetTextureCount(aiTextureType_REFLECTION) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_REFLECTION, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				m_Spec[i] = new TextureResource(FullPath.c_str());

				if (!m_Spec[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Spec[i];
					m_Spec[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded Spec '%s'\n", FullPath.c_str());
				}
			}
		}
	}
	
	return Ret;
}

void Mesh::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//Indices
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12); //UVs
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //Normals
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);	//Tangents

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex < m_Textures.size()) {
			if (m_Textures[MaterialIndex])
			{
				m_Textures[MaterialIndex]->bind(GL_TEXTURE0, glGetUniformLocation(shader->getProgram(), "DiffuseTextureSampler"), 0);
			}
			else
			{
				defaultDiff->bind(GL_TEXTURE0, glGetUniformLocation(shader->getProgram(), "DiffuseTextureSampler"), 0);
			}
		}
		if (MaterialIndex < m_Normals.size()) {
			if (m_Normals[MaterialIndex] != nullptr)
			{
				m_Normals[MaterialIndex]->bind(GL_TEXTURE1, glGetUniformLocation(shader->getProgram(), "NormalTextureSampler"), 1);
			}
			else
			{
				defaulNormal->bind(GL_TEXTURE1, glGetUniformLocation(shader->getProgram(), "NormalTextureSampler"), 1);
			}
		}
		//if (MaterialIndex < m_masks.size()) {
		//	if (m_masks[MaterialIndex] != nullptr)
		//	{
		//		m_masks[MaterialIndex]->bind(GL_TEXTURE1, glGetUniformLocation(shader->getProgram(), "Masks"), 1);
		//	}

		//}
		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

bool Mesh::GenerateTerrain(int widhti, int heighti, float freq, int oct, int seed)
{
	//setup buffers

	if(terrainGen != nullptr)
	{
		delete terrainGen;
	}
	terrainGen = new TerrainGenerator();

	if(!terrainGen->GenerateHeigthMap(widhti, heighti, freq, oct, seed))
	{
		return false;
	}

	printf("Loaded Terrain\n");

	long int index;;
	vector3D left, right, up, down, pos, normal;
	
	terrainVertices.resize((terrainGen->width) * (terrainGen->height) * 2);
	terrainIndices.resize(((terrainGen->width) * (terrainGen->height) * 2) * 3);
	
	int inIndex = 0;
	int width = terrainGen->width;
	int height = terrainGen->height;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			index = (width * i) + j;

			Vertex v;

			float x = float(j) -width * 0.5;
			float y = float(terrainGen->imageData[i*width + j]) * 0.25f;
			float z = float(i) -height * 0.5;
			pos = vector3D(x, y, z);

			left = terrainGen->GetNeighbourVertex(pos, -1, 0);
			right = terrainGen->GetNeighbourVertex(pos, +1, 0);
			up = terrainGen->GetNeighbourVertex(pos, 0, -1);
			down = terrainGen->GetNeighbourVertex(pos, 0, +1);
			normal = (left + right + up + down).normalizeRe();
			
			v.m_pos = pos;
			v.m_normal = normal;

			terrainVertices[index] = v;

			if((i < height-1) && (j < width-1))
			{
				// Top triangle (T0)
				terrainIndices[inIndex++] = index;               // V0
				terrainIndices[inIndex++] = index + width + 1;   // V3
				terrainIndices[inIndex++] = index + 1;           // V1
				// Bottom triangle (T1)
				terrainIndices[inIndex++] = index;               // V0
				terrainIndices[inIndex++] = index + width;       // V2
				terrainIndices[inIndex++] = index + width + 1;   // V3
			}
			else
			{
				terrainIndices[inIndex++] = index;   // V0
				terrainIndices[inIndex++] = index;   // V3
				terrainIndices[inIndex++] = index;   // V1
				// Bottom triangle (T1)
				terrainIndices[inIndex++] = index;   // V0
				terrainIndices[inIndex++] = index;   // V2
				terrainIndices[inIndex++] = index;   // V3
			}
		}
	}

	computeTangentBasis(terrainVertices, terrainIndices);

	glGenBuffers(1, &terrainVertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* terrainVertices.size(), &terrainVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->terrainIndexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->terrainIndexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* terrainIndices.size(), &terrainIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);  // Vertex position.
	glEnableVertexAttribArray(1); // Vertex color
	glEnableVertexAttribArray(2); // Vertex normal
	glEnableVertexAttribArray(3); // Tangents

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);

	return true; 
}


void Mesh::RenderTerrain()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//Indices
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12); //UVs
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //Normals
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);	//Tangents

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIndexBufferId);

	glDrawElements(GL_TRIANGLES, terrainIndices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	
	/*
	int ind1, ind2;
	glColor3f(0.5f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	//glBegin(GL_TRIANGLE_STRIP);
	for (int i = 1; i < terrainGen->height-1; i++)
	{
		for (int j = 1; j <  terrainGen->width-1; j++)
		{
			ind1 = (terrainGen->width * i) + j;
			ind2 = (terrainGen->width * (i + 1)) + j - 1;
			glColor3f(0.5f, 0.0f, 1.0f);
			glVertex3f( terrainVertices[ind1].m_pos[0], terrainVertices[ind1].m_pos[1], terrainVertices[ind1].m_pos[2]);
			glVertex3f(terrainVertices[ind2].m_pos[0], terrainVertices[ind2].m_pos[1], terrainVertices[ind2].m_pos[2]);
		
			glVertex3f( terrainVertices[ind1].m_pos[0], terrainVertices[ind1].m_pos[1], terrainVertices[ind1].m_pos[2]);
			glVertex3f( terrainVertices[ind1 + 1].m_pos[0], terrainVertices[ind1 + 1].m_pos[1], terrainVertices[ind1 + 1].m_pos[2]);

			ind2 = (terrainGen->width * (i - 1) + j + 1);
			glVertex3f( terrainVertices[ind1].m_pos[0], terrainVertices[ind1].m_pos[1], terrainVertices[ind1].m_pos[2]);
			glVertex3f( terrainVertices[ind2].m_pos[0], terrainVertices[ind2].m_pos[1], terrainVertices[ind2].m_pos[2]);
		}
	}
	glEnd();
	*/
}

void Mesh::computeTangentBasis(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	for (unsigned int i = 0; i < indices.size(); i += 3) {
		Vertex& v0 = vertices[indices[i]];
		Vertex& v1 = vertices[indices[i + 1]];
		Vertex& v2 = vertices[indices[i + 2]];

		vector3D Edge1 = v1.m_pos - v0.m_pos;
		vector3D Edge2 = v2.m_pos - v0.m_pos;

		float DeltaU1 = v1.m_tex.x() - v0.m_tex.x();
		float DeltaV1 = v1.m_tex.y() - v0.m_tex.y();
		float DeltaU2 = v2.m_tex.x() - v0.m_tex.x();
		float DeltaV2 = v2.m_tex.y() - v0.m_tex.y();

		float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		vector3D Tangent, Bitangent;

		Tangent[0] = f * (DeltaV2 * Edge1.x() - DeltaV1 * Edge2.x());
		Tangent[1] = f * (DeltaV2 * Edge1.y() - DeltaV1 * Edge2.y());
		Tangent[2] = f * (DeltaV2 * Edge1.z() - DeltaV1 * Edge2.z());

		Bitangent[0] = f * (-DeltaU2 * Edge1.x() - DeltaU1 * Edge2.x());
		Bitangent[1] = f * (-DeltaU2 * Edge1.y() - DeltaU1 * Edge2.y());
		Bitangent[2] = f * (-DeltaU2 * Edge1.z() - DeltaU1 * Edge2.z());

		v0.m_tangents += Tangent;
		v1.m_tangents += Tangent;
		v2.m_tangents += Tangent;

	}

	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i].m_tangents.normalize();
	}
}


