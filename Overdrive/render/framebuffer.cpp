#include "stdafx.h"
#include "framebuffer.h"
#include "gltypes.h"

namespace overdrive {
	namespace render {
		FrameBuffer::FrameBuffer():
			mHandle(0)
		{
			glGenFramebuffers(1, &mHandle);
		}

		FrameBuffer::~FrameBuffer() {
			if (mHandle)
				glDeleteFramebuffers(1, &mHandle);
		}

		GLuint FrameBuffer::getHandle() const {
			return mHandle;
		}

		void FrameBuffer::attachColor(GLuint unit, const Texture2D& texture) {
			glBindFramebuffer(GL_FRAMEBUFFER, mHandle);

			GLenum attachment = asColorAttachment(unit);

			glFramebufferTexture2D(
				GL_FRAMEBUFFER, 
				attachment, 
				GL_TEXTURE_2D, 
				texture.getHandle(), 
				0
			);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void FrameBuffer::attachDepth(const Texture2D& texture) {
			glBindFramebuffer(GL_FRAMEBUFFER, mHandle);

			switch (texture.getFormatType()) {
			case eTextureFormatType::DEPTH:
				glFramebufferTexture2D(
					GL_FRAMEBUFFER,
					GL_DEPTH_ATTACHMENT,
					GL_TEXTURE_2D,
					texture.getHandle(),
					0 // mip level
				);
				break;

			case eTextureFormatType::DEPTH_STENCIL:
				glFramebufferTexture2D(
					GL_FRAMEBUFFER,
					GL_DEPTH_STENCIL_ATTACHMENT,
					GL_TEXTURE_2D,
					texture.getHandle(),
					0	// mip level
				);
				break;

			default:
				throw std::runtime_error("Unsupported depth format");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void FrameBuffer::bind(eFramebufferBind options) {
			glBindFramebuffer(static_cast<GLenum>(options), mHandle);
		}

		void FrameBuffer::unbind(eFramebufferBind options) {
			glBindFramebuffer(static_cast<GLenum>(options), 0);
		}

		void FrameBuffer::drawAttachment(
			GLuint index_0
		) {
			GLenum attachments[] = {
				asColorAttachment(index_0) 
			};

			glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
			glDrawBuffers(sizeof(attachments) / sizeof(attachments[0]), attachments);
		}

		void FrameBuffer::drawAttachment(
			GLuint index_0, 
			GLuint index_1
		) {
			GLenum attachments[] = {
				asColorAttachment(index_0),
				asColorAttachment(index_1)
			};

			glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
			glDrawBuffers(sizeof(attachments) / sizeof(attachments[0]), attachments);
		}

		void FrameBuffer::drawAttachment(
			GLuint index_0, 
			GLuint index_1, 
			GLuint index_2
		) {
			GLenum attachments[] = {
				asColorAttachment(index_0),
				asColorAttachment(index_1),
				asColorAttachment(index_2)
			};

			glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
			glDrawBuffers(sizeof(attachments) / sizeof(attachments[0]), attachments);
		}

		void FrameBuffer::drawAttachment(
			GLuint index_0, 
			GLuint index_1, 
			GLuint index_2,
			GLuint index_3
		) {
			GLenum attachments[] = {
				asColorAttachment(index_0),
				asColorAttachment(index_1),
				asColorAttachment(index_2),
				asColorAttachment(index_3),
			};

			glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
			glDrawBuffers(sizeof(attachments) / sizeof(attachments[0]), attachments);
		}

		std::ostream& operator << (std::ostream& os, const eFramebufferBind& value) {
			switch (value) {
			case eFramebufferBind::READ:	os << "READ";	break;
			case eFramebufferBind::WRITE:	os << "WRITE";	break;
			case eFramebufferBind::READ_WRITE:	os << "READ_WRITE"; break;
			default:
				os << "Unknown framebuffer binding option: " << static_cast<std::underlying_type_t<eFramebufferBind>>(value);
			}

			return os; 
		}
	}
}