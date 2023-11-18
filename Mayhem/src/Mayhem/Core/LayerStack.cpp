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

 // Tutorial : https://youtu.be/_Kj6BSfM6P4?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#include "MayhemPrecompiledHeaders.h"
#include "LayerStack.h"

namespace Mayhem
{
	Mayhem::LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
		m_layerInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			layer->OnAttach();
			m_layerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end())
		{
			overlay->OnDetach();
			m_layers.erase(it);
		}
	}
}