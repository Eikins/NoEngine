#include "Editor/SceneEditors.h"

namespace Editor
{
	void SceneEditors::DrawSceneNode(Core::Transform* transform, Core::GameObject** selectedObject)
	{
		uint32_t flags = 0;
		bool isClicked = false;
		if (*selectedObject == transform->_gameObject)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		if (transform->_children.size() == 0)
		{
			ImGui::TreeNodeEx(transform->_gameObject->_name.c_str(), flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
			if (ImGui::IsItemClicked(0))
			{
				isClicked = true;
			}
		}
		else
		{
			bool isOpen = ImGui::TreeNodeEx(transform->_gameObject->_name.c_str(), flags | ImGuiTreeNodeFlags_OpenOnArrow);
			if (ImGui::IsItemClicked(0))
			{
				isClicked = true;
			}
			if (isOpen)
			{
				for (uint32_t i = 0; i < transform->_children.size(); i++)
				{
					DrawSceneNode(transform->_children[i], selectedObject);
				}
				ImGui::TreePop();
			}
		}

		if (isClicked)
		{
			*selectedObject = transform->_gameObject;
		}
	}

	void SceneEditors::DrawSceneHierarchy(std::vector<Core::Transform*>& roots, Core::GameObject** selectedObject)
	{
		for (uint32_t i = 0; i < roots.size(); i++)
		{
			DrawSceneNode(roots[i], selectedObject);
		}
	}
}