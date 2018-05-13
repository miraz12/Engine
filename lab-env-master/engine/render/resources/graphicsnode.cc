#include "config.h"
#include "graphicsnode.h"
#include "lighttypes.h"

namespace Resources
{

    GraphicsNode::GraphicsNode()
    {
        mesh = std::make_shared<MeshResource>();
        light = std::make_shared<Managers::LightManager>();

        camera.setValues(0, -5, 5);
        origin.setValues(0.0f, 0.0f, 0.0f);
        headUp.setValues(0.0f, 1, 0.0f);
    }

    GraphicsNode::~GraphicsNode()
    {

    }

    void GraphicsNode::drawOBJ(matrix4D projection, matrix4D view, matrix4D model)
    {
        mesh->shader->bind();

        light->Setup(mesh->shader);

        mesh->shader->modVector3f("cameraPos", vector3D(camera.x(), camera.y(), camera.z()));
        mesh->shader->modVector3f("worldPos", vector3D(model.getPosition()));
        mesh->shader->modMatrix4fv("projection", projection);
        mesh->shader->modMatrix4fv("view", view);
        mesh->shader->modMatrix4fv("model", model);
        mesh->Render();
    }

    std::shared_ptr<MeshResource> GraphicsNode::getMesh()
    {
        return mesh;
    }

    void GraphicsNode::setMesh(std::shared_ptr<MeshResource> m)
    {
        mesh = m;
    }

    std::shared_ptr<ShaderObject> GraphicsNode::getShader()
    {
        return mesh->shader;
    }

    void GraphicsNode::setProjec(const matrix4D &projec) {
        GraphicsNode::projec = projec;
    }

    void GraphicsNode::setMod(const matrix4D &mod) {
        GraphicsNode::mod = mod;
    }
    const matrix4D &GraphicsNode::getProjec() const {
        return projec;
    }

    matrix4D &GraphicsNode::getMod() {
        return mod;
    }

    void GraphicsNode::setLight(std::shared_ptr<Managers::LightManager> s)
    {
        light = s;
    }
}
