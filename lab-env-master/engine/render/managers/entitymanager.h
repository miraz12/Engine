
#include "render/resources/graphicsnode.h"

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();
    void Init(std::shared_ptr<LightManager> lnode);
    void AddNewEntity(const char* filename);
    void DrawEntitys();
    void ReloadShaders();


private:

    std::vector<GraphicsNode*> m_objList;
    matrix4D world;
    std::shared_ptr<LightManager> lNode;

};
