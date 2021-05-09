#include "Scripting/ScriptInstance.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>

#include "Scripting/Runtime.h"

#include "Core/Modules/GameManager.hpp"
#include "Core/Transform.h"


namespace Scripting
{
#pragma region Script Instance

	void ScriptInstance::SetName(const std::string& namespc, const std::string& name)
	{
		_namespace = namespc;
		_name = name;
	}

	void ScriptInstance::Bind(Runtime* runtime, MonoAssembly* assembly, Core::Transform* transform)
	{
		_environment = &runtime->GetEnvironment();
		MonoImage* image =mono_assembly_get_image(assembly);

		// Create C# script & transform
		auto scriptClass = mono_class_from_name(image, _namespace.c_str(), _name.c_str());
		_scriptObj = mono_object_new(runtime->_domain, scriptClass);

		// Get update func
		auto monoInit = mono_class_get_method_from_name(scriptClass, "Init", 0);
		auto monoUpdate = mono_class_get_method_from_name(scriptClass, "Update", 0);
		auto monoOnInput = mono_class_get_method_from_name(scriptClass, "OnInput", 1);
		if (monoInit != nullptr)
		{
			_initMethodPtr = mono_method_get_unmanaged_thunk(monoInit);
		}

		if (monoUpdate != nullptr)
		{
			_updateMethodPtr = mono_method_get_unmanaged_thunk(monoUpdate);
		}

		if (monoOnInput != nullptr)
		{
			_onInputMethodPtr = mono_method_get_unmanaged_thunk(monoOnInput);
		}

		_publicFields.size();

		MonoClassField* field;
		void* iter = nullptr;
		while ((field = mono_class_get_fields(scriptClass, &iter)) != nullptr)
		{
			if (mono_field_get_flags(field) & MONO_FIELD_ATTR_PUBLIC)
			{
				FieldInfo info;
				info.handle = field;
				info.name = std::string(mono_field_get_name(field));
				info.type = mono_type_get_type(mono_field_get_type(field)) == MONO_NATIVE_R8 ? FieldType::FLOAT : FieldType::UNKNOWN;

				_publicFields.push_back(info);
			}
		}
		mono_runtime_object_init(_scriptObj);

		// Get fields and set script transform field to created transform
		auto transformField = mono_class_get_field_from_name(scriptClass, "_transform");
		mono_field_set_value(_scriptObj, transformField, &transform);
	}

	void ScriptInstance::Init()
	{
		if (_initMethodPtr == nullptr) return;
		MonoException* exception = nullptr;
		reinterpret_cast<MonoVoidMethod>(_initMethodPtr)(_scriptObj, &exception);
	}

	void ScriptInstance::Update()
	{
		if (_updateMethodPtr == nullptr) return;
		MonoException* exception = nullptr;
		reinterpret_cast<MonoVoidMethod>(_updateMethodPtr)(_scriptObj, &exception);
	}

	void ScriptInstance::OnInput(const Core::InputEvent& event)
	{
		if (_onInputMethodPtr == nullptr) return;
		MonoException* exception = nullptr;
		auto eventObject = _environment->BoxInputEvent(event);
		reinterpret_cast<MonoOnInputMethod>(_onInputMethodPtr)(_scriptObj, eventObject, &exception);
	}
#pragma endregion
}