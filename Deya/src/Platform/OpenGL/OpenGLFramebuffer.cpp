#include "dypch.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Deya
{
    static const uint32_t s_MaxFramebufferSize = 8192;
   
    namespace Utils
    {
        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void BindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }

        static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
        {
            glGenTextures(count, outID);

            for (int i = 0; i < count; i++)
            {
                BindTexture(multisampled, outID[i]);
            }
        }

        static void AttachColourTexture(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, int index)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        static bool IsDepthFormat(FramebufferTextureFormat format)
        {
            switch (format)
            {
                case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
                default: break;
            }

            return false;
        }

    }
    
    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) 
        : m_Spec(spec)
    {
        for (auto format : m_Spec.Attachments.Attachments)
        {
            if (!Utils::IsDepthFormat(format.TextureFormat))
                m_ColourAttachmentSpecs.emplace_back(format);
            else
                m_DepthAttachmentSpecs = format;
        }

        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer() 
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(m_ColourAttachments.size(), m_ColourAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate() 
    {
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(m_ColourAttachments.size(), m_ColourAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);

            m_ColourAttachments.clear();
            m_DepthAttachment = 0;
        }
   
        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        bool multisample = m_Spec.Samples > 1;

        // Attachments
        if (m_ColourAttachmentSpecs.size())
        {
            m_ColourAttachments.resize(m_ColourAttachmentSpecs.size());
            Utils::CreateTextures(multisample, m_ColourAttachments.data(), m_ColourAttachments.size());

            for (size_t i = 0; i < m_ColourAttachments.size(); i++)
            {
                Utils::BindTexture(multisample, m_ColourAttachments[i]);
                switch (m_ColourAttachmentSpecs[i].TextureFormat)
                {
                    case FramebufferTextureFormat::RGBA8:
                        Utils::AttachColourTexture(m_ColourAttachments[i], m_Spec.Samples, GL_RGBA8, m_Spec.Width, m_Spec.Height, i);
                        break;
                    default:
                        break;
                }
            }
        }
        
        if (m_DepthAttachmentSpecs.TextureFormat != FramebufferTextureFormat::None)
        {
            Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
            Utils::BindTexture(multisample, m_DepthAttachment);
            switch (m_DepthAttachmentSpecs.TextureFormat)
            {
                case FramebufferTextureFormat::DEPTH24STENCIL8:
                    Utils::AttachDepthTexture(m_DepthAttachment, m_Spec.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Spec.Width, m_Spec.Height);
                    break;
                default:
                    break;
            }
        }

        if (m_ColourAttachments.size())
        {
            DY_CORE_ASSERT(m_ColourAttachments.size() <= 4, "More than 4 colour attachments not supported");
            GLenum buffers[4] { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(m_ColourAttachments.size(), buffers);
        }
        else if (m_ColourAttachments.empty())
        {
            // only depth-pass
            glDrawBuffer(GL_NONE);
        }

        DY_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind() 
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Spec.Width, m_Spec.Height);
    }

    void OpenGLFramebuffer::Unbind() 
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) 
    {
        if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
        {
            DY_CORE_WARN("Attempted to resize framebuffer to {0}x{1}", width, height);
            return;
        }

        m_Spec.Width = width;
        m_Spec.Height = height;
        Invalidate();
    }
}
