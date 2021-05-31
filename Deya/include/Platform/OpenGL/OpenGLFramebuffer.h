#pragma once

#include "Deya/Renderer/Framebuffer.h"

namespace Deya
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer() override;

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

        virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

        virtual uint64_t GetColourAttachmentRendererID(uint32_t index = 0) const override 
        {
            DY_CORE_ASSERT_STRING(index < m_ColourAttachments.size(), "Index larger than total colour attachments");
            return m_ColourAttachments[index]; 
        };

        virtual const FramebufferSpecification& GetSpecification() const override { return m_Spec; };
    private:
        uint32_t m_RendererID = 0;
        FramebufferSpecification m_Spec;

        std::vector<FramebufferTextureSpecification> m_ColourAttachmentSpecs;
        FramebufferTextureSpecification m_DepthAttachmentSpecs = FramebufferTextureFormat::None;

        std::vector<uint32_t> m_ColourAttachments;
        uint32_t m_DepthAttachment = 0;
    };
}
