#pragma once

#include <mono/jit/jit.h>
#include <vector>

#include "Core/Transform.h"
#include "Math/Math.h"

namespace Core
{
	class ScriptedBehaviour;
	class ScriptSystem;
}

namespace Scripting
{
	typedef void (*UpdateMethod)(MonoObject *object, MonoException** exception);

	enum class FieldType
	{
		UNKNOWN,
		FLOAT
	};

	struct FieldInfo
	{
		std::string name;
		FieldType type;
		void* handle;
	};

	class ScriptInstance
	{
		friend class Runtime;
		friend class Core::ScriptedBehaviour;
		friend class Core::ScriptSystem;
	private:
		MonoObject* _scriptObj = nullptr;
		void* _updateMethodPtr = nullptr;
		std::string _namespace;
		std::string _name;
		std::vector<FieldInfo> _publicFields;

		ScriptInstance() : _publicFields() {}
		void Bind(MonoDomain* domain, MonoAssembly* assembly, Core::Transform* transform);
	public:
		void SetName(const std::string& namespc, const std::string& name);

		void Update();

		inline std::vector<FieldInfo>& GetPublicFields() { return _publicFields; }

		template<typename T>
		T GetFieldValue(void* handle)
		{
			T value;
			mono_field_get_value(_scriptObj, static_cast<MonoClassField*>(handle), &value);
			return value;
		}

		template<typename T>
		void SetFieldValue(void* handle, T value)
		{
			mono_field_set_value(_scriptObj, static_cast<MonoClassField*>(handle), &value);
		}
	private:
#pragma region Internal Calls
		static void Script_SetPosition(Core::Transform* transform, Math::Vector3 position);
		static Math::Vector3 ScriptInstance::Script_GetPosition(Core::Transform* transform);
#pragma endregion

	};
}