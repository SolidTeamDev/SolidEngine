#include "Rendering/renderer.hpp"
#include "Core/Debug/throwError.hpp"
namespace Solid
{
    Renderer* Renderer::pInstance{nullptr};
    std::mutex Renderer::mutex;

    Renderer * Renderer::GetInstance()
    {
        std::lock_guard<std::mutex>lck(mutex); // RC: What is this mutex for?

        if(pInstance == nullptr)
        {
            ThrowError("Renderer not initialized", ESolidErrorCode::S_INIT_ERROR);
        }
        return pInstance;
    }

} //!namespace