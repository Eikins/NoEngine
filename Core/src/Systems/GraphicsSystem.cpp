#include "Core/Systems/GraphicsSystem.h"

#include "Core/Modules/GameManager.hpp"

#include "Core/Components/Light.h"

#include "Editor/Editor.h"
#include "Core/Time.h"

namespace Core
{
	Signature GraphicsSystem::CreateSignature()
	{
		Signature signature;
		signature.set(_gameManager->GetComponentType<Renderer>());
		return signature;
	}

	void GraphicsSystem::CreateContext(const Graphics::WindowDescriptor& windowDescriptor)
	{
		_context = CreateGraphicsContext(windowDescriptor);
	}

	void GraphicsSystem::BindInputMaster(InputMaster* inputMaster)
	{
		_context.BindInputMaster(inputMaster);
	}

	bool GraphicsSystem::RenderAsync(Camera* camera)
	{
		_context.SetupCameraProperties(camera);
		auto& lights = _gameManager->GetAllComponents<Light>();
		// Process light, for the moment we only have directional ones
		if (lights.size() > 0 && lights[0].IsEnabled() && lights[0].GetGameObject()->IsActive())
		{
			_context.SetDirectionalLight(lights[0].GetTransform()->ForwardVector(), lights[0]._color);
		}
		else
		{
			_context.SetDirectionalLight(Math::Vector3::Zero, Math::Vector3::Zero);
		}

		auto& renderers = _gameManager->GetAllComponents<Renderer>();

		_context.PrepareRenderers(renderers);
		if (_context.BeginFrame())
		{
			_context.DrawRenderers(renderers);
#ifdef NoEngine_Editor
			if (Editor::Enabled())
			{
				_context.BeginEditorFrame();
				Editor::ShowFPS(1.0f / Time::deltaTime);
				// Draw gizmos
				Editor::SetupCameraProperties(camera);
				Editor::DrawGizmos();
				Editor::DrawEditors();

				_context.EndEditorFrame();
			}
#endif
			_context.EndFrame();
			_context.RenderAsync();
			return true;
		}
		else
		{
			return false;
		}
		
	}

	void GraphicsSystem::WaitForRenderCompletion()
	{
		_context.WaitForRenderCompletion();
	}

	void GraphicsSystem::Render(Camera* camera)
	{
		RenderAsync(camera);
		WaitForRenderCompletion();
	}

	Graphics::Window& GraphicsSystem::GetWindow()
	{
		return _context.GetWindow();
	}

	void GraphicsSystem::Dispose()
	{
		_context.Release();
	}
}