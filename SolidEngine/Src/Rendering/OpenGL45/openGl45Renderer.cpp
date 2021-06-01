#include "Resources/ressources.hpp"
#include "Resources/graphicalResource.hpp"
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
        InitLinesGL();
        InitSkyboxGL();
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
			Log::Send(infoLog, Log::ELogSeverity::ERROR);
			abort();
		}
		glGetShaderiv(fs, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(fs, 1024, nullptr, infoLog);
			std::string log = infoLog;
			Log::Send(infoLog, Log::ELogSeverity::ERROR);
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
			Log::Send(infoLog, Log::ELogSeverity::ERROR);
			abort();
		}
		glGenBuffers(1,&grid_vbo);
		glGenBuffers(1,&grid_ebo);
		glDeleteShader(vs);
		glDeleteShader(fs);
	}

    void OpenGL45Renderer::InitLinesGL()
    {
        const char* vertexS =  R"GLSL(
		#version 450
        in vec4 aPosition;

        out vec3 vColor;

        uniform mat4 proj;
        uniform mat4 view;
        void main()
        {
            gl_Position = proj * view * aPosition;
            vColor = vec3(0,1,0);

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
	        Log::Send(infoLog, Log::ELogSeverity::ERROR);
            abort();
        }
        glGetShaderiv(fs, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus == GL_FALSE)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(fs, 1024, nullptr, infoLog);
            std::string log = infoLog;
	        Log::Send(infoLog, Log::ELogSeverity::ERROR);
            abort();

        }
        linesProgram = glCreateProgram();
        glAttachShader(linesProgram, vs);
        glAttachShader(linesProgram, fs);
        glLinkProgram(linesProgram);
        GLint linkStatus;
        glGetProgramiv(linesProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE)
        {
            GLchar infoLog[1024];
            glGetProgramInfoLog(linesProgram, ARRAYSIZE(infoLog), nullptr, infoLog);
	        Log::Send(infoLog, Log::ELogSeverity::ERROR);
            abort();
        }
        glGenBuffers(1,&lines_vbo);
        glGenBuffers(1,&lines_ebo);
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    void OpenGL45Renderer::InitSkyboxGL()
    {
        const char* vertexS =  R"GLSL(
		#version 450

        layout(location = 0) in vec3 aPosition;

        out vec3 textCoords;

        uniform mat4 proj;
        uniform mat4 view;
        uniform mat4 model;

        void main()
        {
            textCoords = aPosition;
            vec4 pos = proj * view * model * vec4(aPosition, 1.0);
            gl_Position = pos.xyww;
        }
        )GLSL";
        const char* fragS =  R"GLSL(
		#version 450

		in vec3 textCoords;

        out vec4 fragColor;

        uniform samplerCube skybox;

        void main()
        {
            fragColor = texture(skybox, textCoords);
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
            printf("Shader compilation error: %s\n", infoLog);
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
        skyboxProgram = glCreateProgram();
        glAttachShader(skyboxProgram, vs);
        glAttachShader(skyboxProgram, fs);
        glLinkProgram(skyboxProgram);
        GLint linkStatus;
        glGetProgramiv(skyboxProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE)
        {
            GLchar infoLog[1024];
            glGetProgramInfoLog(skyboxProgram, ARRAYSIZE(infoLog), nullptr, infoLog);
            printf("Program link error: %s", infoLog);
            abort();
        }

        std::array<Vec3,8> pos
        {
                Vec3(1,1,-1),
                Vec3(1,-1,-1),
                Vec3(1,1,1),
                Vec3(1,-1,1),
                Vec3(-1,1,-1),
                Vec3(-1,-1,-1),
                Vec3(-1,1,1),
                Vec3(-1,-1,1)
        };
        std::array<Vec3ui,12> index
        {
                Vec3ui{6,3,2},
                Vec3ui{4,7,6},
                Vec3ui{3,5,1},
                Vec3ui{2,1,0},
                Vec3ui{0,5,4},
                Vec3ui{6,0,4},
                Vec3ui{6,7,3},
                Vec3ui{4,5,7},
                Vec3ui{3,7,5},
                Vec3ui{2,3,1},
                Vec3ui{0,1,5},
                Vec3ui{6,2,0}
        };

        glGenVertexArrays(1,&skybox_vao);

        glGenBuffers(1,&skybox_vbo);
        glGenBuffers(1,&skybox_ebo);

        glBindVertexArray(skybox_vao);

        glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(Vec3ui), index.data(),GL_STATIC_DRAW);

        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vec3), pos.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (const GLvoid*)0);

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
        GLenum error = glGetError();
        while (error != GL_NO_ERROR)
        {
            error = glGetError();
        }
    }

    void OpenGL45Renderer::Clear(const Vec2i& _windowSize) const
    {
        if(_windowSize.x <= 0 || _windowSize.y <= 0)
            return;

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
        if(_framebuffer.size.x <= 0 || _framebuffer.size.y <= 0)
            return;

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
	            Log::Send("Binary Format = %u is not a value recognized by the implementation.\n", Log::ELogSeverity::ERROR);
            }
            if(ErrorCode == GL_INVALID_OPERATION )
            {
	            Log::Send("programID = %u is not the name of an existing program object .\n", Log::ELogSeverity::ERROR);
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

    void OpenGL45Renderer::DrawSkybox(const Camera &_camera) const
    {
        if(_map == nullptr)
            return;

        glUseProgram(skyboxProgram);

        glDepthFunc(GL_LEQUAL);

        Mat4<float> view = _camera.GetView();
        Mat4<float> model;

        view.elements[12] = 0;
        view.elements[13] = 0;
        view.elements[14] = 0;

        glUseProgram(skyboxProgram);
        glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "proj"), 1, GL_FALSE, _camera.GetProjection().elements.data());
        glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_FALSE, view.elements.data());
        glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "model"), 1, GL_FALSE, model.elements.data());

        glBindVertexArray(skybox_vao);
        glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox_ebo);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, ((GL::Cubemap*)_map.get())->cubemapID);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr); // Draw triangle
        glDepthFunc(GL_LESS);
    }

    void OpenGL45Renderer::DrawLines(const Camera& _camera, std::vector<Vec3> _points, std::vector<uint> indices) const
    {
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

        //gen vao
        ////TODO
        glGenVertexArrays(1, &temp_vao);

        glUseProgram(linesProgram);
        //uniforms
        glUniformMatrix4fv(glGetUniformLocation(linesProgram,"proj"),1,GL_FALSE,_camera.GetProjection().elements.data());
        glUniformMatrix4fv(glGetUniformLocation(linesProgram,"view"),1,GL_FALSE,_camera.GetView().elements.data());

        glBindVertexArray(temp_vao);

        glBindBuffer(GL_ARRAY_BUFFER, lines_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lines_ebo);
        uint posAttrib = glGetAttribLocation(linesProgram, "aPosition");
        ////TODO

        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT,   GL_FALSE,  3*sizeof(GLfloat), (GLvoid*)0);



        // Upload vertex/index buffers
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)_points.size() * (int)sizeof(Vec3), (const GLvoid*)_points.data(), GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)indices.size() * (int)sizeof(uint), (const GLvoid*)indices.data(), GL_DYNAMIC_DRAW);

        //Draw
        glLineWidth(1);
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

} //!namespace