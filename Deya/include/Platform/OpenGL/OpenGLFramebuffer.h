#pragma once

#include "Deya/Renderer/Framebuffer.h"

namespace Deya
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;

        virtual uint32_t GetColourAttachmentRendererID() const override { return m_ColourAttachment; };

        virtual const FramebufferSpecification& GetSpecification() const override { return m_Spec; };
    private:
        uint32_t m_RendererID = 0;
        uint32_t m_ColourAttachment = 0;
        uint32_t m_DepthAttachment = 0;
        FramebufferSpecification m_Spec;
    };
}
