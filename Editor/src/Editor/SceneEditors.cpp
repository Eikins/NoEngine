#include "Editor/SceneEditors.h"

namespace Editor
{
	void SceneEditors::DrawSceneNode(Core::Transform* transform, int* selectionSceneIndex)
	{
		uint32_t flags = 0;
		bool isClicked = false;
		if (*selectionSceneIndex == transform->_gameObject->sceneIndex)
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
					DrawSceneNode(transform->_children[i], selectionSceneIndex);
				}
				ImGui::TreePop();
			}
		}

		if (isClicked)
		{
			*selectionSceneIndex = transform->_gameObject->sceneIndex;
		}
	}

	void SceneEditors::DrawSceneHierarchy(Core::Scene& scene, int* selectionSceneIndex)
	{
		for (uint32_t i = 0; i < scene._rootTransforms.size(); i++)
		{
			DrawSceneNode(scene._rootTransforms[i], selectionSceneIndex);
		}
	}
}