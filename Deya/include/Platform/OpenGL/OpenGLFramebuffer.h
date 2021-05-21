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

        virtual uint32_t GetColourAttachmentRendererID() const override { return m_ColourAttachment; };

        virtual const FramebufferSpecification& GetSpecification() const override { return m_Spec; };
    private:
        uint32_t m_RendererID;
        uint32_t m_ColourAttachment;
        uint32_t m_DepthAttachment;
        FramebufferSpecification m_Spec;
    };
}
