#include "dypch.h"

#include "Deya/Renderer/Renderer2D.h"

#include "Deya/Renderer/VertexArray.h"
#include "Deya/Renderer/Shader.h"
#include "Deya/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Deya
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Colour;
        glm::vec2 TexCoord;
        // TODO: texid
    };

    struct Renderer2DData
    {
        const uint32_t MaxQuads = 10000;
        const uint32_t MaxVertices = MaxQuads * 4;
        const uint32_t MaxIndices = MaxQuads * 6;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;
    };

    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        DY_PROFILE_FUNCTION();

        s_Data.QuadVertexArray = VertexArray::Create(); 
        
        BufferLayout layout =
        {
            { ShaderDataType::Float3, "a_Position"},
            { ShaderDataType::Float4, "a_Colour"},
            { ShaderDataType::Float2, "a_TexCoords"}
        };

        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout(layout);
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff; // pure white (same as #ffffffff)
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_Data.TextureShader = Shader::Create("assets/shaders/TextureShader.glsl");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetInt("u_Texture", 0);
    }

    void Renderer2D::Shutdown()
    {
        DY_PROFILE_FUNCTION();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        DY_PROFILE_FUNCTION();

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::EndScene()
    {
        DY_PROFILE_FUNCTION();

        uint32_t dataSize = (uint8_t*) s_Data.QuadVertexBufferPtr - (uint8_t*) s_Data.QuadVertexBufferBase;
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        Flush();
    }

    void Renderer2D::Flush()
    {
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, colour);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& colour)
    {
        DY_PROFILE_FUNCTION();

        s_Data.QuadVertexBufferPtr->Position = position;
        s_Data.QuadVertexBufferPtr->Colour = colour;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z };
        s_Data.QuadVertexBufferPtr->Colour = colour;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
        s_Data.QuadVertexBufferPtr->Colour = colour;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };
        s_Data.QuadVertexBufferPtr->Colour = colour;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;

        // s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
        // s_Data.WhiteTexture->Bind();

        // glm::mat4 transform = // !TRS: transform * rotation * scale 
        //     glm::translate(glm::mat4(1.0f), position) /* * rotation */ * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        // s_Data.TextureShader->SetMat4("u_Transform", transform);

        // s_Data.QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColour)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColour);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColour)
    {
        DY_PROFILE_FUNCTION();

        s_Data.TextureShader->SetFloat4("u_Colour", tintColour);
        s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        texture->Bind();

        glm::mat4 transform = // !TRS: transform * rotation * scale 
            glm::translate(glm::mat4(1.0f), position) /* * rotation */ * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data.TextureShader->SetMat4("u_Transform", transform);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }


    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& colour)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, rotation, size, colour);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& colour)
    {
        DY_PROFILE_FUNCTION();

        s_Data.TextureShader->SetFloat4("u_Colour", colour);
        s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
        s_Data.WhiteTexture->Bind();

        glm::mat4 transform = // !TRS: transform * rotation * scale 
            glm::translate(glm::mat4(1.0f), position)
             * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
             * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data.TextureShader->SetMat4("u_Transform", transform);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColour)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, rotation, size, texture, tilingFactor, tintColour);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColour)
    {
        DY_PROFILE_FUNCTION();

        s_Data.TextureShader->SetFloat4("u_Colour", tintColour);
        s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        texture->Bind();

        glm::mat4 transform = // !TRS: transform * rotation * scale 
            glm::translate(glm::mat4(1.0f), position) 
             * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
             * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data.TextureShader->SetMat4("u_Transform", transform);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }
}