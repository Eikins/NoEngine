#include "Scripting/Environment.h"

namespace Scripting
{

	void Environment::Initialize(MonoDomain* domain, MonoAssembly* assembly)
	{
		MonoImage* image;
		image = mono_assembly_get_image(assembly);

		// Initialize Time Class
		{
			auto timeClass = mono_class_from_name(image, "NoEngine", "Time");
			_time.obj = mono_object_new(domain, timeClass);
			_time.deltaTimeField = mono_class_get_field_from_name(timeClass, "deltaTime");
			_time.timeField = mono_class_get_field_from_name(timeClass, "time");
			mono_runtime_object_init(_time.obj);
		}
	}

	void Environment::SetTime(float time, float deltaTime)
	{
		mono_field_static_set_value(_time.obj->vtable, _time.timeField, &time);
		mono_field_static_set_value(_time.obj->vtable, _time.deltaTimeField, &deltaTime);
	}
}