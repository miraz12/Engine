#include "config.h"
#include "sceneloader.h"

#include <iostream>


#include  "fstream"
#include <yaml-cpp/eventhandler.h>
#include <yaml-cpp/yaml.h>  // IWYU pragma: keep

#include "core/math/vector3D.h"
#include "render/base/entity.h"
#include "render/managers/entitymanager.h"
#include "render/properties/graphicsproperty.h"

using namespace Toolkit;

SceneLoader* SceneLoader::instance = 0;

void SceneLoader::Init()
{
	entityManager = Managers::EntityManager::GetInstance();
	LoadScene();
	LoadEntities();
}

void operator >> (const YAML::Node& node, vector3D& v)
{
	node[0] >> v[0];
	node[1] >> v[1];
	node[2] >> v[2];
}

void operator >> (const YAML::Node& node, Properties::GraphicsProperty* gProp)
{
	std::string type;
	node["type"] >> type;
	if (type == "GrPr")
	{
		std::string filename;
		vector3D v;
		node["position"] >> v;
		node["filename"] >> filename;
		gProp->model.setPos(v);
		gProp->mesh->LoadMesh(filename);
	}
}

void operator >> (const YAML::Node& node, Toolkit::Scene& scene)
{
	Base::Entity* newEntity = new Base::Entity;
	std::string name;
	node["entity"] >> name;
	const YAML::Node& properties = node["properties"];
	for (unsigned i = 0; i < properties.size(); i++) 
	{
		std::string type;
		properties[i]["type"] >> type;
		if (type == "GrPr")
		{
			Properties::GraphicsProperty* gProp = new Properties::GraphicsProperty();
			properties[i] >> gProp;
			newEntity->AttachProperty(gProp);
		}
	}
	scene.entitys.push_back(newEntity);
}

SceneLoader* SceneLoader::GetInstance()
{
	if (instance == 0)
	{
		instance = new SceneLoader();
	}
	return instance;
}

void SceneLoader::LoadScene()
{
	std::ifstream fin("content/scene/scene.yaml");
	YAML::Parser parser(fin);
	YAML::Node doc;
	parser.GetNextDocument(doc);
	for (unsigned i = 0; i < doc.size(); i++) 
	{
		doc[i] >> scene;
	}
}

void SceneLoader::LoadEntities()
{
	for (int i=0; i < scene.entitys.size(); i++)
	{
		entityManager->AttachEntity(scene.entitys[i]);
	}
	return;
}

