#include "Scripting/ScriptInstance.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>

#include "Core/Transform.h"

namespace Scripting
{
#pragma region Script Instance

	void ScriptInstance::SetName(const std::string& namespc, const std::string& name)
	{
		_namespace = namespc;
		_name = name;
	}

	void ScriptInstance::Bind(MonoDomain* domain, MonoAssembly* assembly, Core::Transform* transform)
	{
		MonoImage* image =mono_assembly_get_image(assembly);

		// Create C# script & transform
		auto scriptClass = mono_class_from_name(image, _namespace.c_str(), _name.c_str());
		_scriptObj = mono_object_new(domain, scriptClass);

		// Get fields and set script transform field to created transform
		auto transformField = mono_class_get_field_from_name(scriptClass, "_transform");
		mono_field_set_value(_scriptObj, transformField, &transform);

		// Get update func
		auto monoUpdate = mono_class_get_method_from_name(scriptClass, "Update", 0);
		_updateMethodPtr = mono_method_get_unmanaged_thunk(monoUpdate);

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
	}

	void ScriptInstance::Update()
	{
		if (_updateMethodPtr == nullptr) return;
		MonoException* exception = nullptr;
		reinterpret_cast<UpdateMethod>(_updateMethodPtr)(_scriptObj, &exception);
	}
#pragma endregion

#pragma region Internal Calls
	void ScriptInstance::Script_SetPosition(Core::Transform* transform, Math::Vector3 position)
	{
		transform->SetPosition(position);
	}

	Math::Vector3 ScriptInstance::Script_GetPosition(Core::Transform* transform)
	{
		return transform->GetPosition();
	}
#pragma endregion


}