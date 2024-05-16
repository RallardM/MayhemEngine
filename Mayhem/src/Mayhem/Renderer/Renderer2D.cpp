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

 // Tutorial : https://youtu.be/r_5fF1AxgpU?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT
 // Tutorial : https://youtu.be/biGF6oLxgtQ?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#include "MayhemPrecompiledHeaders.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Mayhem
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_data;

	void Renderer2D::Init()
	{
		MAYHEM_PROFILE_FUNCTION();

		s_data.QuadVertexArray = VertexArray::Create();

		s_data.QuadVertexBuffer = VertexBuffer::Create(s_data.MaxVertices * sizeof(QuadVertex));
		s_data.QuadVertexBuffer->SetLayout({
			{ E_ShaderDataType::Float3, "a_Position" },
			{ E_ShaderDataType::Float4, "a_Color" },
			{ E_ShaderDataType::Float2, "a_TexCoord" },
			{ E_ShaderDataType::Float, "a_TexIndex" },
			{ E_ShaderDataType::Float, "a_TilingFactor" }
			});
		s_data.QuadVertexArray->AddVertexBuffer(s_data.QuadVertexBuffer);

		s_data.QuadVertexBufferBase = new QuadVertex[s_data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_data.MaxIndices);
		s_data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int samplers[s_data.MaxTextureSlots];
		for (int i = 0; i < s_data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_data.TextureShader = Shader::Create("assets/shaders/TextureShader.glsl");
		s_data.TextureShader->Bind();
		s_data.TextureShader->SetIntArray("u_Textures", samplers, s_data.MaxTextureSlots);

		// Set all texture slots to 0
		s_data.TextureSlots[0] = s_data.WhiteTexture;

		s_data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	}

	void Renderer2D::Shutdown()
	{
		MAYHEM_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		MAYHEM_PROFILE_FUNCTION();

		s_data.TextureShader->Bind();
		s_data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_data.QuadIndexCount = 0;
		s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;

		s_data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		MAYHEM_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t)((uint8_t*)s_data.QuadVertexBufferPtr - (uint8_t*)s_data.QuadVertexBufferBase);
		s_data.QuadVertexBuffer->SetData(s_data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		// Bind textures
		for (uint32_t i = 0; i < s_data.TextureSlotIndex; i++)
			s_data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_data.QuadVertexArray, s_data.QuadIndexCount);
		s_data.Stats.DrawCalls++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		MAYHEM_PROFILE_FUNCTION();

		const float textureIndex = 0.0f; // White texture
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadIndexCount += 6;

		s_data.Stats.QuadCount++;

		/*s_data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.TextureShader->SetMat4("u_Transform", transform);
		s_data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_data.QuadVertexArray);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		MAYHEM_PROFILE_FUNCTION();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++)
		{
			if (*s_data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadIndexCount += 6;

		s_data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		MAYHEM_PROFILE_FUNCTION();

		const float textureIndex = 0.0f; // White texture
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadIndexCount += 6;

		s_data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		MAYHEM_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++)
		{
			if (*s_data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
		s_data.QuadVertexBufferPtr->Color = color;
		s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_data.QuadVertexBufferPtr++;

		s_data.QuadIndexCount += 6;

		s_data.Stats.QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_data.Stats;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_data.QuadIndexCount = 0;
		s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;

		s_data.TextureSlotIndex = 1;
	}
}