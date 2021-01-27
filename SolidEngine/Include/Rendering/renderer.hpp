#pragma once

#include "Window/window.hpp"

namespace Solid
{

    struct SOLID_API RendererParams
    {
        WindowParams windowParams;
    };

    /**
     * @brief Renderer virtual class. Interface with rendering API (RHI)
     */
    class SOLID_API Renderer
    {
    protected:

        Window* window = nullptr;

        virtual void Release() = 0;

    public:
        virtual ~Renderer() = default;

        virtual void Initialize(const RendererParams& _rParams) = 0;

        //TODO: Wrapping functions

        [[nodiscard]]
        Window* GetWindow() const;
    };

} //!namespace



