#include "dypch.h"

#include "Deya/Renderer/Renderer2D.h"

#include "Deya/Renderer/VertexArray.h"
#include "Deya/Renderer/Shader.h"
#include "Deya/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Deya
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init()
    {
        DY_PROFILE_FUNCTION();

        s_Data = new Renderer2DStorage();

        s_Data->QuadVertexArray = VertexArray::Create(); 

        float squareVertices[4 * 5] =	// 4 verts * (3 dimensions + 2 tex coords)
        {
            // POS                  TEX COORDS					
             0.5f,  0.5f, 0.0f,     1.0f, 1.0f,	// top right
            -0.5f,  0.5f, 0.0f,     0.0f, 1.0f,	// top left
             0.5f, -0.5f, 0.0f,     1.0f, 0.0f,	// bottom right
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f 	// bottom left
        };

        uint32_t squareIndices[6]
        {
            3, 0, 1,
            3, 2, 0
        };

        BufferLayout layout =
        {
            { ShaderDataType::Float3, "a_Position"},
            { ShaderDataType::Float2, "a_TexCoords"}
        };

        Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout(layout);
        s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

        Ref<IndexBuffer> sqaureIB;
        sqaureIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_Data->QuadVertexArray->SetIndexBuffer(sqaureIB);

        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff; // pure white (same as #ffffffff)
        s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_Data->TextureShader = Shader::Create("assets/shaders/TextureShader.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetInt("u_Texture", 0);
    }

    void Renderer2D::Shutdown()
    {
        DY_PROFILE_FUNCTION();


        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        DY_PROFILE_FUNCTION();


        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {
        DY_PROFILE_FUNCTION();
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, colour);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& colour)
    {
        DY_PROFILE_FUNCTION();

        s_Data->TextureShader->SetFloat4("u_Colour", colour);
        s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_Data->WhiteTexture->Bind();

        glm::mat4 transform = // !TRS: transform * rotation * scale 
            glm::translate(glm::mat4(1.0f), position) /* * rotation */ * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->TextureShader->SetMat4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColour)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColour);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColour)
    {
        DY_PROFILE_FUNCTION();

        s_Data->TextureShader->SetFloat4("u_Colour", tintColour);
        s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        texture->Bind();

        glm::mat4 transform = // !TRS: transform * rotation * scale 
            glm::translate(glm::mat4(1.0f), position) /* * rotation */ * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->TextureShader->SetMat4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }


    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& colour)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, rotation, size, colour);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& colour)
    {
        DY_PROFILE_FUNCTION();

        s_Data->TextureShader->SetFloat4("u_Colour", colour);
        s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_Data->WhiteTexture->Bind();

        glm::mat4 transform = // !TRS: transform * rotation * scale 
            glm::translate(glm::mat4(1.0f), position)
             * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
             * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->TextureShader->SetMat4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColour)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, rotation, size, texture, tilingFactor, tintColour);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColour)
    {
        DY_PROFILE_FUNCTION();

        s_Data->TextureShader->SetFloat4("u_Colour", tintColour);
        s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        texture->Bind();

        glm::mat4 transform = // !TRS: transform * rotation * scale 
            glm::translate(glm::mat4(1.0f), position) 
             * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
             * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->TextureShader->SetMat4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}