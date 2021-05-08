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
        }

		DY_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    
    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        DY_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();
        
        uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer( 								//*Tell OpenGL what our data means
				index,											// Index of attribute
				element.GetComponentCount(),					// How many components there are in the attribute (vec2 = 2, vec3 = 3, ...)
				ShaderDataTypeToOpenGLBaseType(element.Type), 	// The type of attribute this is (float, int, ...)
				element.Normalized ? GL_TRUE : GL_FALSE, 		// Is the data normalized?
				layout.GetStride(),								// The stride between the attributes
				(const void*) element.Offset); 					// Offset between the start of the data, and the attribute			
            index++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}