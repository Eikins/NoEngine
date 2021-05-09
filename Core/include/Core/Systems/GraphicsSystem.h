#pragma once

#include "System.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/Window.h"

namespace Core
{
	class GraphicsSystem : public System
	{
	private:
		Graphics::GraphicsContext _context;
	public:
		virtual Signature CreateSignature() override;

		void CreateContext(const Graphics::WindowDescriptor& windowDescriptor);
		void BindInputMaster(InputMaster* inputMaster);
		bool RenderAsync(Camera* camera);
		void WaitForRenderCompletion();

		void Render(Camera* camera);

		Graphics::Window& GetWindow();
		void Dispose();
	};
}