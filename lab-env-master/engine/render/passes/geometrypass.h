#pragma once
#include "render/base/framepass.h"
#include "core/toolkit/userinterface.h"
#include "render/managers/entitymanager.h"

namespace Passes
{
    class GeometryPass : public Base::FramePass
    {
    public:
        GeometryPass();
        ~GeometryPass();

        void Execute();
		std::shared_ptr<Resources::ShaderObject> shader;

    private:

        Managers::EntityManager* eManager;

    };

}
