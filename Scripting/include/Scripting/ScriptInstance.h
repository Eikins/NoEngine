#pragma once

#include <mono/jit/jit.h>
#include <vector>

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
		MonoClassField* _transformField = nullptr;
		void* _updateMethodPtr = nullptr;
		std::string _namespace;
		std::string _name;
		std::vector<FieldInfo> _publicFields;

		ScriptInstance() : _publicFields() {}
		void Bind(MonoDomain* domain, MonoAssembly* assembly);

		static void Script_SetPosition(MonoObject* transform, Math::Vector3 position);
	public:
		void SetName(const std::string& namespc, const std::string& name);

		void SetTransform(Math::Vector3 position);
		Math::Vector3 GetPosition();

		void Update();

		inline std::vector<FieldInfo> GetPublicFields() { return _publicFields; }

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
	};
}