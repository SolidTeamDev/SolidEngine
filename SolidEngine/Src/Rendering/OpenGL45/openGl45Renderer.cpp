#include "Resources/ressources.hpp"

#include "Rendering/OpenGL45/openGl45Renderer.hpp"

#include <GLFW/glfw3.h>

#include "Core/Debug/debug.hpp"

namespace Solid
{
#ifndef ARRAYSIZE
#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

    Renderer * OpenGL45Renderer::InitRenderer()
    {
        if(pInstance != nullptr)
        {
            throw ThrowError("Renderer already initialized", ESolidErrorCode::S_INIT_ERROR);
        }
        pInstance = new OpenGL45Renderer();
        pInstance->SetRendererType(ERendererType::OpenGl45);
        return pInstance;
    }
    OpenGL45Renderer::OpenGL45Renderer()
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            throw ThrowError("Failed to load glad !",ESolidErrorCode::S_INIT_ERROR);

        if (GLAD_GL_KHR_debug)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
            glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
        }
        InitGridGL();
    }

	void OpenGL45Renderer::InitGridGL()
	{
		const char* vertexS =  R"GLSL(
		#version 450
        in vec2 aPosition; // X and Z
        in vec3 aColor;

        out vec3 vColor;

        uniform mat4 proj;
        uniform mat4 view;
        void main()
        {
            gl_Position = proj * view * vec4(aPosition.x, 0.0 , aPosition.y, 1.0);
            vColor = aColor;

        }
        )GLSL";
		const char* fragS =  R"GLSL(
		#version 450

		in vec3 vColor;
		out vec4 fragColor;
        void main()
        {
			fragColor = vec4(vColor,1.0);
        }
        )GLSL";
		uint vs = glCreateShader(GL_VERTEX_SHADER);
		uint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vs, 1, &vertexS, nullptr);
		glShaderSource(fs, 1, &fragS, nullptr);
		glCompileShader(vs);
		glCompileShader(fs);
		GLint compileStatus;
		glGetShaderiv(vs, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(vs, 1024, nullptr, infoLog);
			std::string log = infoLog;
			printf("Shader compilation error: %s", infoLog);
			abort();
		}
		glGetShaderiv(fs, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(fs, 1024, nullptr, infoLog);
			std::string log = infoLog;
			printf("Shader compilation error: %s", infoLog);
			abort();

		}
		gridProgram = glCreateProgram();
		glAttachShader(gridProgram, vs);
		glAttachShader(gridProgram, fs);
		glLinkProgram(gridProgram);
		GLint linkStatus;
		glGetProgramiv(gridProgram, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE)
		{
			GLchar infoLog[1024];
			glGetProgramInfoLog(gridProgram, ARRAYSIZE(infoLog), nullptr, infoLog);
			printf("Program link error: %s", infoLog);
			abort();
		}
		glGenBuffers(1,&grid_vbo);
		glGenBuffers(1,&grid_ebo);
		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	void OpenGL45Renderer::GLDebugCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length,
                                           const GLchar *_message, const void *_userParam)
    {
        Log::ELogSeverity logSeverity = Log::ELogSeverity::DEBUG;
        switch (_severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                logSeverity = Log::ELogSeverity::ERROR;
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                logSeverity = Log::ELogSeverity::WARNING;
                break;
            case GL_DEBUG_SEVERITY_LOW:
                logSeverity = Log::ELogSeverity::INFO;
                break;
        }

        Log::Send("OpenGL Debug : " + std::string(_message), logSeverity);
    }

    void OpenGL45Renderer::Clear(const Vec2i& _windowSize) const
    {
        glViewport(0,0,_windowSize.x,_windowSize.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGL45Renderer::ClearColor(const Vec4& _clearColor) const
    {
        glClearColor(_clearColor.x,_clearColor.y,_clearColor.z,_clearColor.w);
    }

    Framebuffer OpenGL45Renderer::CreateFramebuffer(const Vec2i& _size) const
    {
        Framebuffer framebuffer;

        //Framebuffer
        framebuffer.size = _size;


        //Texture
        glGenTextures(1,&framebuffer.texture);
        glBindTexture(GL_TEXTURE_2D, framebuffer.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebuffer.size.x, framebuffer.size.y, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        //Depth buffer
        glGenRenderbuffers(1,&framebuffer.depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER,framebuffer.depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer.size.x, framebuffer.size.y);
        glBindRenderbuffer(GL_RENDERBUFFER,0);

        //Attachment
        glGenFramebuffers(1, &framebuffer.id);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer.texture, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer.depthBuffer);
        GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Log::Send("Framebuffer is not complete",Log::ELogSeverity::ERROR);

        glBindFramebuffer(GL_FRAMEBUFFER,0);

        return framebuffer;
    }

    void OpenGL45Renderer::UpdateFramebuffer(const Framebuffer& _framebuffer) const
    {
        glBindTexture(GL_TEXTURE_2D, _framebuffer.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _framebuffer.size.x, _framebuffer.size.y, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     nullptr);
        glBindRenderbuffer(GL_RENDERBUFFER, _framebuffer.depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _framebuffer.size.x, _framebuffer.size.y);
        glBindTexture(GL_TEXTURE_2D,0);
        glBindRenderbuffer(GL_RENDERBUFFER,0);
    }

    void OpenGL45Renderer::BeginFramebuffer(const Framebuffer &_framebuffer) const
    {
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Log::Send("Framebuffer is not complete",Log::ELogSeverity::ERROR);

        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer.id);
    }

    void OpenGL45Renderer::EndFramebuffer() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


/*




//



    Renderer::CShader OpenGL45Renderer::CreateComputeProgram(std::vector<char*>& _sources) const
    {

        CShader compute = CreateShader(GL_COMPUTE_SHADER, 1, _sources);
        if(compute.error)
            return compute;
        compute.pID = glCreateProgram();
        glAttachShader(compute.pID, compute.sID);
        glLinkProgram(compute.pID);
        GLint linkStatus;
        glGetProgramiv(compute.pID, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE)
        {

            GLchar infoLog[1024];
            glGetProgramInfoLog(compute.pID, ARRAYSIZE(infoLog), nullptr, infoLog);
            printf("Program link error: %s", infoLog);
            //TODO : Cleanup at return
            compute.error = true;
        }
        return compute;
    }

    Renderer::VFShader OpenGL45Renderer::CreateVertFragProgram(std::vector<char*>& _VertexSources, std::vector<char*>& _fragSources) const
    {

    }*/

    ShaderBinary OpenGL45Renderer::GetShaderBinary(uint _PID) const
    {

        int pSize = 0;
        glGetProgramiv(_PID, GL_PROGRAM_BINARY_LENGTH, &pSize);
        char* binary = new char[pSize];
        GLenum BFormat = 0;
        glGetProgramBinary(_PID, pSize, nullptr, &BFormat, (void*)binary);
        return {.size=(uint)pSize,.format=BFormat, .b=binary};
    }

    uint OpenGL45Renderer::CreateShaderFromBinary(ShaderBinary _binary) const
    {
        uint ID = glCreateProgram();
        glProgramBinary(ID, _binary.format, (void*)_binary.b, _binary.size);
        GLenum ErrorCode = glGetError();
        while (ErrorCode != GL_NO_ERROR)
        {

            if(ErrorCode == GL_INVALID_ENUM)
            {

                printf("Binary Format = %u is not a value recognized by the implementation.\n", _binary.format);
            }
            if(ErrorCode == GL_INVALID_OPERATION )
            {
                printf("programID = %u is not the name of an existing program object .\n", ID);
            }
            ErrorCode = glGetError();
        }
        return ID;
    }

	void OpenGL45Renderer::DrawSolidGrid(const Camera &_camera, float _gridSize, Vec3 _color, float _thickness) const
	{
		//draw GL_LINES
		uint temp_vao = 0;
		GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
		glActiveTexture(GL_TEXTURE0);
		GLuint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&last_program);
		GLuint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&last_texture);
		GLuint last_sampler; { glGetIntegerv(GL_SAMPLER_BINDING, (GLint*)&last_sampler); }
		GLuint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&last_array_buffer);
		GLuint last_vertex_array_object; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&last_vertex_array_object);
		GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
		GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
		GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
		GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
		GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
		GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
		GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
		GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
		GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
		GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
		GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
		GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
		GLboolean last_enable_stencil_test = glIsEnabled(GL_STENCIL_TEST);
		GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
		glGenVertexArrays(1, &temp_vao);

		glUseProgram(gridProgram);
		//uniforms
		glUniformMatrix4fv(glGetUniformLocation(gridProgram,"proj"),1,GL_FALSE,_camera.GetProjection().elements.data());
		glUniformMatrix4fv(glGetUniformLocation(gridProgram,"view"),1,GL_FALSE,_camera.GetView().elements.data());

		glBindVertexArray(temp_vao);

		glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid_ebo);
		uint posAttrib = glGetAttribLocation(gridProgram, "aPosition");
		uint colorAttrib = glGetAttribLocation(gridProgram, "aColor");

		glEnableVertexAttribArray(posAttrib);
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(posAttrib,   2, GL_FLOAT,   GL_FALSE, 5* sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(colorAttrib, 3, GL_FLOAT,   GL_FALSE,  5*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));

		struct vertex2D
		{
			Vec2 pos;
			Vec3 Color;
		};
		std::vector<vertex2D> vertices;
		vertices.reserve(_gridSize * 3);
		std::vector<uint> indices;
		indices.reserve(_gridSize * 3);
		int camPosX = -_camera.position.x;
		int camPosZ = -_camera.position.z;

		for(int y = -_gridSize-camPosZ; y <= _gridSize-camPosZ; ++y)
		{
			vertices.push_back({.pos={-_gridSize-camPosX,(float)y}, .Color=_color});
			indices.push_back(vertices.size()-1);
			vertices.push_back({.pos={_gridSize-camPosX,(float)y}, .Color=_color});
			indices.push_back(vertices.size()-1);
		}
		indices.push_back(0);
		indices.push_back(vertices.size()-2);

		indices.push_back(1);

		indices.push_back(vertices.size()-1);

		for(int x = -_gridSize+1-camPosX; x <= _gridSize-1-camPosX; ++x)
		{
			vertices.push_back({.pos={(float)x,-_gridSize-camPosZ}, .Color=_color});
			indices.push_back(vertices.size()-1);
			vertices.push_back({.pos={(float)x,_gridSize-camPosZ}, .Color=_color});
			indices.push_back(vertices.size()-1);

		}


		// Upload vertex/index buffers
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vertices.size() * (int)sizeof(vertex2D), (const GLvoid*)vertices.data(), GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)indices.size() * (int)sizeof(uint), (const GLvoid*)indices.data(), GL_DYNAMIC_DRAW);

		//Draw
		glLineWidth(_thickness);
		glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);



		glDeleteVertexArrays(1,&temp_vao);
		glUseProgram(last_program);
		glBindTexture(GL_TEXTURE_2D, last_texture);
        glBindSampler(0, last_sampler);
		glActiveTexture(last_active_texture);
		glBindVertexArray(last_vertex_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
		glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
		glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
		if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
		if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
		if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
		if (last_enable_stencil_test) glEnable(GL_STENCIL_TEST); else glDisable(GL_STENCIL_TEST);
		if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
		glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
		glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
		glLineWidth(1);

	}

    void OpenGL45Renderer::DrawSkybox(const Camera& _camera) const
    {

    }

} //!namespace