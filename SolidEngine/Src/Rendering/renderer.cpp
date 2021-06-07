#include "Rendering/renderer.hpp"
#include "Core/Debug/throwError.hpp"
namespace Solid
{
    Renderer* Renderer::pInstance{nullptr};


    Renderer * Renderer::GetInstance()
    {


        if(pInstance == nullptr)
        {
            ThrowError("Renderer not initialized", ESolidErrorCode::S_INIT_ERROR);
        }
        return pInstance;
    }

	Framebuffer *Renderer::GetCurrentBuffer()
	{
		return currentFB;
	}

} //!namespace