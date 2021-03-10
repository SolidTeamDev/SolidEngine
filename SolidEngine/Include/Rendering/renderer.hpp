#pragma once

#include <vector>
#include <mutex>
#include "Resources/ressources.hpp"
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
    public:
        Renderer(Renderer&) = delete;
        void operator=(const Renderer&) = delete;

        static Renderer* GetInstance() ;

        struct CShader
        {
            bool error = false;
            uint sID = 0;
            uint pID = 0;
        };
        struct VFShader
        {
            bool error = false;
            uint vID = 0;
            uint fID = 0;
            uint pID = 0;
        };
        /*struct ShaderBinary
        {
            uint size = 0;
            GLenum format;
            char* b = nullptr;
        };*/
        //TODO: Wrapping functions
        virtual void Clear(const Int2& _windowSize) const = 0;
        virtual void ClearColor(const Float4& _clearColor) const = 0;
        virtual Framebuffer CreateFramebuffer(const Int2& _size) const = 0;
        virtual void UpdateFramebuffer(const Framebuffer& _framebuffer) const = 0;
        virtual void BeginFramebuffer(const Framebuffer& _framebuffer) const = 0;
        virtual void EndFramebuffer() const = 0;


        virtual ShaderBinary GetShaderBinary(uint _PID) const = 0;
        virtual uint CreateShaderFromBinary(ShaderBinary _binary) const = 0;
        virtual void SetRendererType(ERendererType _type) final{ if(type==ERendererType::NONE) type = _type; }
        virtual ERendererType GetRenderType() final {return type;}
    };

} //!namespace



