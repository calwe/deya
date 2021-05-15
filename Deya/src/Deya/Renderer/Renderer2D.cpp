#include "dypch.h"

#include "Deya/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Deya/Renderer/VertexArray.h"
#include "Deya/Renderer/Shader.h"
#include "Deya/Renderer/RenderCommand.h"

namespace Deya
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> FlatColourShader;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init()
    {
        s_Data = new Renderer2DStorage();

        s_Data->QuadVertexArray = VertexArray::Create(); 

        float squareVertices[4 * 3] =	// 4 verts * (3 dimensions)
        {
            // POS					
            0.5f,  0.5f, 0.0f, 	// top right
            -0.5f,  0.5f, 0.0f, 	// top left
            0.5f, -0.5f, 0.0f, 	// bottom right
            -0.5f, -0.5f, 0.0f,  	// bottom left
        };

        uint32_t squareIndices[6]
        {
            3, 0, 1,
            3, 2, 0
        };

        BufferLayout layout =
        {
            { ShaderDataType::Float3, "a_Position"}
        };

        Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout(layout);
        s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

        Ref<IndexBuffer> sqaureIB;
        sqaureIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_Data->QuadVertexArray->SetIndexBuffer(sqaureIB);

        s_Data->FlatColourShader = Shader::Create("Sandbox/assets/shaders/FlatColourShader.glsl");
    }

    void Renderer2D::Shutdown()
    {
        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColourShader)->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColourShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColourShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
    }

    void Renderer2D::EndScene()
    {

    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, colour);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& colour)
    {
        std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColourShader)->Bind();
        std::dynamic_pointer_cast<Deya::OpenGLShader>(s_Data->FlatColourShader)->UploadUniformFloat4("u_Colour", colour);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

}