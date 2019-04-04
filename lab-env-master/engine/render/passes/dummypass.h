#pragma once
#include "render/base/framepass.h"

namespace Passes
{
	///Template for creating new passes
    class Dummy : public Base::FramePass
    {
    public:
		Dummy();
        ~Dummy();
        void Execute();

    private:


    };

}
