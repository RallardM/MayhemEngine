/*
 * Copyright 2023 TheCherno/Hazel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 // Tutorial : https://youtu.be/qEfohFgQ1-I?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#include "MayhemPrecompiledHeaders.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace Mayhem
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) :
		m_width(width), 
		m_height(height)
	{
		MAYHEM_PROFILE_FUNCTION();

		m_internalFormat = GL_RGBA;
		m_dataFormat = GL_RGBA;

		MAYHEM_ENGINE_ASSERT(m_internalFormat & m_dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		glTextureStorage2D(m_rendererId, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) :
		m_path(path)
	{
		MAYHEM_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;

		{
			MAYHEM_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		MAYHEM_ENGINE_ASSERT(data, "Failed to load image!");
		m_width = width;
		m_height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;

		MAYHEM_ENGINE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		glTextureStorage2D(m_rendererId, 1, internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		MAYHEM_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_rendererId);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		MAYHEM_PROFILE_FUNCTION();

		uint32_t bytesPerPixels = m_dataFormat == GL_RGBA ? 4 : 3;
		MAYHEM_ENGINE_ASSERT(size == m_width * m_height * bytesPerPixels, "Data must be entire texture!")
		glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		MAYHEM_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_rendererId);
	}
}