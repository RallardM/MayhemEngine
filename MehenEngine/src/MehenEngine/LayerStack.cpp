// Tutorial : https://youtu.be/_Kj6BSfM6P4?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#include "MhnPCHeader.h"
#include "LayerStack.h"

namespace MehenEngine
{
	MehenEngine::LayerStack::LayerStack()
	{
		m_layerInsert = m_layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layerInsert = m_layers.emplace(m_layerInsert, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			m_layerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end())
			m_layers.erase(it);
	}
}