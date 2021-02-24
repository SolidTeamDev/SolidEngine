#pragma once

#include "Build/solidAPI.hpp"

#include "Rendering/framebuffer.hpp"
#include "ECS/Components/transform.hpp"
#include "ECS/Components/camera.hpp"

namespace Solid
{

    enum class ERendererType
    {
        OpenGl45
    };

    struct SOLID_API RendererParams
    {
        ERendererType rendererType;
    };

    /**
     * @brief Renderer virtual class. Interface with rendering API (RHI)
     */
    class SOLID_API Renderer
    {
    private:
    public:
        virtual ~Renderer() = default;

        //TODO: Wrapping functions
        virtual void Clear(const Int2& _windowSize) const = 0;
        virtual void ClearColor(const Float4& _clearColor) const = 0;
        virtual Framebuffer CreateFramebuffer(const Int2& _size) const = 0;
        virtual void UpdateFramebuffer(const Framebuffer& _framebuffer) const = 0;
        virtual void BeginFramebuffer(const Framebuffer& _framebuffer) const = 0;
        virtual void EndFramebuffer() const = 0;
        virtual void InitMesh(class MeshResource* m)const = 0;
        virtual void DrawMesh(const class MeshResource* _mesh)const = 0;
        virtual void SetShaderMVP(class ShaderResource* _shader, Transform& _model, Camera& _camera)const = 0;
    };

} //!namespace



