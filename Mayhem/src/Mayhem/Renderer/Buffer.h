// Tutorial : https://youtu.be/jIJFM_pi6gQ?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#pragma once

namespace Mayhem
{
	enum class E_ShaderDataType : unsigned __int8
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(E_ShaderDataType type)
	{
		switch (type)
		{
		case E_ShaderDataType::Float:		return 4;
		case E_ShaderDataType::Float2:		return 4 * 2;
		case E_ShaderDataType::Float3:		return 4 * 3;
		case E_ShaderDataType::Float4:		return 4 * 4;
		case E_ShaderDataType::Mat3:		return 4 * 3 * 3;
		case E_ShaderDataType::Mat4:		return 4 * 4 * 4;
		case E_ShaderDataType::Int:			return 4;
		case E_ShaderDataType::Int2:		return 4 * 2;
		case E_ShaderDataType::Int3:		return 4 * 3;
		case E_ShaderDataType::Int4:		return 4 * 4;
		case E_ShaderDataType::Bool:		return 1;
		}

		MAYHEM_GAME_ASSERT(false, "Unknown ShaderDataType!");

		return 0;
	}

	// ############################## BufferElement ############################## //

	class BufferElement
	{
	public:
		std::string m_name;
		E_ShaderDataType m_type;
		uint32_t m_offset;
		uint32_t m_size;
		bool m_normalized;

		BufferElement() {}

		BufferElement(E_ShaderDataType type, const std::string& name, bool normalized = false) :
			m_name(name), m_type(type), m_size(ShaderDataTypeSize(type)), m_offset(0), m_normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (m_type)
			{
			case E_ShaderDataType::Float:		return 1;
			case E_ShaderDataType::Float2:		return 2;
			case E_ShaderDataType::Float3:		return 3;
			case E_ShaderDataType::Float4:		return 4;
			case E_ShaderDataType::Mat3:		return 3 * 3;
			case E_ShaderDataType::Mat4:		return 4 * 4;
			case E_ShaderDataType::Int:			return 1;
			case E_ShaderDataType::Int2:		return 2;
			case E_ShaderDataType::Int3:		return 3;
			case E_ShaderDataType::Int4:		return 4;
			case E_ShaderDataType::Bool:		return 1;
			}

			MAYHEM_GAME_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	// ############################## BufferLayout ############################## //

	class BufferLayout
	{

	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline uint32_t GetStride() const { return m_stride; }

		inline const std::vector<BufferElement>& GetElements() const { return m_elements; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_stride = 0;
			for (auto& element : m_elements)
			{
				element.m_offset = offset;
				offset += element.m_size;
				m_stride += element.m_size;
			}
		}

		std::vector<BufferElement> m_elements;
		uint32_t m_stride = 0;

	};

	// ############################## VertexBuffer ############################## //

	class VertexBuffer
	{

	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

	};

	// ############################## IndexBuffer ############################## //

	class IndexBuffer
	{

	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

	};
}