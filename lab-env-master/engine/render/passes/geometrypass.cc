#include "config.h"
#include "geometrypass.h"
#include "render/managers/entitymanager.h"

Passes::GeometryPass::GeometryPass()
{
}

Passes::GeometryPass::~GeometryPass()
{
}

void Passes::GeometryPass::Setup()
{
    eManager = Managers::EntityManager::GetInstance();
}

void Passes::GeometryPass::Execute()
{
    //Could possably be moved out into renderserver..
    eManager->OnBeginFrame();

}
