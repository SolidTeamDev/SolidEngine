#pragma once

#include <vector>
#include <mutex>
#include "Resources/ressources.hpp"
#include "Resources/graphicalResourceInterface.hpp"
#include "Build/solidAPI.hpp"

#include "Rendering/framebuffer.hpp"
#include "ECS/Components/transform.hpp"
#include "ECS/Components/camera.hpp"

namespace Solid
{

    enum class ERendererType
    {
        OpenGl45,
        NONE,
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
    protected:
        static Renderer* pInstance;
        ERendererType type = ERendererType::NONE;
        Renderer() = default;
        virtual ~Renderer() = default;
        Framebuffer* currentFB = nullptr;
    public:
        std::shared_ptr<ICubemap> _map = nullptr;
        Renderer(Renderer&) = delete;
        void operator=(const Renderer&) = delete;

        static Renderer* GetInstance() ;

        virtual void Clear(const Vec2i& _windowSize) const = 0;
        virtual void ClearColor(const Vec4& _clearColor) const = 0;
        virtual Framebuffer CreateFramebuffer(const Vec2i& _size) const = 0;
        virtual void UpdateFramebuffer(const Framebuffer& _framebuffer) const = 0;
        virtual void BeginFramebuffer(Framebuffer &_framebuffer) = 0;
        virtual void EndFramebuffer() = 0;
        virtual void DrawSolidGrid(const Camera& _camera, float _gridSize, Vec3 _color, float _thickness) const = 0;
        virtual void DrawSkybox(const Camera &_camera) const = 0;
        virtual void DrawLines(const Camera& _camera, std::vector<Vec3> _points, std::vector<uint> indices) const = 0;

        virtual ShaderBinary GetShaderBinary(uint _PID) const = 0;
        virtual uint CreateShaderFromBinary(ShaderBinary _binary) const = 0;
        virtual void SetRendererType(ERendererType _type) final{ if(type==ERendererType::NONE) type = _type; }
        virtual ERendererType GetRenderType() final {return type;}
        virtual void viewport(Vec2 pos, Vec2 Ratio) = 0;
        virtual Framebuffer* GetCurrentBuffer() ;

    };

} //!namespace



