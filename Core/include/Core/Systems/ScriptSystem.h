#pragma once

#include "System.h"

#include "Core/Assets/Script.h"
#include "Core/Components/ScriptedBehaviour.h"
#include "Math/Math.h"

#include "Scripting/Runtime.h"

namespace Core
{

	class ScriptSystem : public System
	{
	private:
		Scripting::Runtime _runtime;
	public:
		virtual Signature CreateSignature() override;

		void InitRuntime();
		void ShutdownRuntime();

		void Reload();
		void CompileAndLoadCore();
		void CompileAndLoadScripts();
		void Update();

	};
}