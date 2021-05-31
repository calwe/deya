#include "dypch.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Deya
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case Deya::ShaderDataType::Float:		return GL_FLOAT;
            case Deya::ShaderDataType::Float2:		return GL_FLOAT;
            case Deya::ShaderDataType::Float3:		return GL_FLOAT;
            case Deya::ShaderDataType::Float4:		return GL_FLOAT;
            case Deya::ShaderDataType::Int:			return GL_INT;
            case Deya::ShaderDataType::Int2:		return GL_INT;
            case Deya::ShaderDataType::Int3:		return GL_INT;
            case Deya::ShaderDataType::Int4:		return GL_INT;
            case Deya::ShaderDataType::Mat3:		return GL_FLOAT;
            case Deya::ShaderDataType::Mat4:		return GL_FLOAT;
            case Deya::ShaderDataType::Bool:		return GL_BOOL;

            case Deya::ShaderDataType::None:		return 0;
        }

		DY_CORE_ASSERT_STRING(false, "Unknown ShaderDataType");
		return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        DY_PROFILE_FUNCTION();
#ifdef DY_PLATFORM_OPENGL_4_5
        glCreateVertexArrays(1, &m_RendererID);
#else
        glGenVertexArrays(1, &m_RendererID);
        glBindVertexArray(m_RendererID);
#endif
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        DY_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        DY_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
    }

    
    void OpenGLVertexArray::Unbind() const
    {
        DY_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        DY_CORE_ASSERT_STRING(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();
        
        uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            switch (element.Type)
            {
                case Deya::ShaderDataType::Float: 
                case Deya::ShaderDataType::Float2:
                case Deya::ShaderDataType::Float3:
                case Deya::ShaderDataType::Float4:
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(
                            index,
                            element.GetComponentCount(),
                            ShaderDataTypeToOpenGLBaseType(element.Type),
                            element.Normalized ? GL_TRUE : GL_FALSE,
                            layout.GetStride(),
                            (const void*) element.Offset
                            );                
                    break;
                }
                case Deya::ShaderDataType::Int:
                case Deya::ShaderDataType::Int2:
                case Deya::ShaderDataType::Int3:
                case Deya::ShaderDataType::Int4:
                case Deya::ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribIPointer(
                            index,
                            element.GetComponentCount(),
                            ShaderDataTypeToOpenGLBaseType(element.Type),
                            layout.GetStride(),
                            (const void*) element.Offset
                            );
                    break;
                }
                // TODO: ShaderDataType::Mat3/4 cases
                case Deya::ShaderDataType::Mat3:
                case Deya::ShaderDataType::Mat4:
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(
                            index,
                            element.GetComponentCount(),
                            ShaderDataTypeToOpenGLBaseType(element.Type),
                            element.Normalized ? GL_TRUE : GL_FALSE,
                            layout.GetStride(),
                            (const void*) element.Offset
                            );
                    break;
                }
                case Deya::ShaderDataType::None: break;
            }
           index++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}
