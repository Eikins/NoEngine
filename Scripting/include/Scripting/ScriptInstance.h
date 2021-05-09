#pragma once

#include <mono/jit/jit.h>
#include <vector>

#include "Environment.h"

#include "Core/Input/InputMaster.h"
#include "Core/Transform.h"


namespace Core
{
	class ScriptedBehaviour;
	class ScriptSystem;
}

namespace Scripting
{
	typedef void (__stdcall *MonoVoidMethod)(MonoObject *object, MonoException** exception);
	typedef void (__stdcall *MonoOnInputMethod)(MonoObject* object, MonoObject* inputEvent, MonoException** exception);

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

	class ScriptInstance : public Core::IInputEventReceiver
	{
		friend class Runtime;
		friend class Core::ScriptedBehaviour;
		friend class Core::ScriptSystem;
	private:
		Environment* _environment = nullptr;
		MonoObject* _scriptObj = nullptr;
		void* _initMethodPtr = nullptr;
		void* _updateMethodPtr = nullptr;
		void* _onInputMethodPtr = nullptr;
		std::vector<FieldInfo> _publicFields;
		std::string _namespace = "";
		std::string _name = "";

		ScriptInstance() : _publicFields() {}
		void Bind(Runtime* runtime, MonoAssembly* assembly, Core::Transform* transform);
	public:
		bool enabled = true;

		void SetName(const std::string& namespc, const std::string& name);

		void Init();
		void Update();
		virtual void OnInput(const Core::InputEvent& event) override;

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
	};
}