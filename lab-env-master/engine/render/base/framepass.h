#pragma once
#include "core/toolkit/userinterface.h"

namespace Base
{
    class FramePass
    {
    public:
        FramePass();

        void BindFrameBuffer();

        virtual ~FramePass();
        virtual void Setup();
        virtual void Execute();
		void RenderQuad();
          
        //virtual void UpdateResolution();

    protected:
        GLuint frameBufferObject;
        GLuint textureBuffer;

    };
}
