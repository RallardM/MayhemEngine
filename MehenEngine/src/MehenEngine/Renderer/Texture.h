#pragma once

#include <string>

#include "MehenEngine/Core.h"

namespace MehenEngine
{
	class Texture
	{

	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		//virtual void Unbind() const = 0;

		//static Ref<Texture> Create(const std::string& path);

	};

	class Texture2D : public Texture
	{

	public:
		//virtual ~Texture2D() = default;

		//static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
	};
}