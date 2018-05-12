#include "config.h"
#include "entitymanager.h"
#include "render/camera.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
    for (unsigned int i = 0; i < m_objList.size(); i++)
    {
        delete m_objList[i];
    }
}

void EntityManager::AddNewEntity(const char* filename)
{
    GraphicsNode* newEntity = new GraphicsNode();
    newEntity->getMesh()->LoadMesh(filename);
    newEntity->setLight(lNode);
    m_objList.push_back(newEntity);
}


void EntityManager::Init(std::shared_ptr<LightManager> lnode)
{
    lNode = lnode;
}

void EntityManager::DrawEntitys()
{
    Camera* cam = Camera::GetInstance();

    cam->view = cam->view.LookAtRH(cam->position, cam->position + cam->camFront, cam->headUp);

    for (unsigned int i = 0; i < m_objList.size(); i++)
    {
        m_objList[i]->camera = cam->position;
        matrix4D modelM = m_objList[i]->getMesh()->getMM();
        //modelM.scale(0.3, 0.3, 0.3);

        m_objList[i]->drawOBJ(cam->projection, cam->view, modelM);
    }
}

void EntityManager::ReloadShaders()
{
    for (unsigned int i = 0; i < m_objList.size(); i++)
    {
        m_objList[i]->getShader()->ReloadShader();
    }
}
