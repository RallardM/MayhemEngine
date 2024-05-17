#pragma once

#include "Mayhem/Renderer/Framebuffer.h"

namespace Mayhem
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_colorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_specification; }

	private:
		uint32_t m_rendererId;
		uint32_t m_colorAttachment, m_depthAttachment;
		FramebufferSpecification m_specification;

	};

}