#include "dypch.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Deya
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW); // TODO: <OpenGL 4.5 support
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
    {
        glNamedBufferSubData(m_RendererID, offset, size, data);
    }
}
