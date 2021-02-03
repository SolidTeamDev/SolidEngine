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
        virtual void Clear(const Float4& _clearColor = {0,0,0,1}) const = 0;

        [[nodiscard]]
        Window* GetWindow() const;
    };

} //!namespace



