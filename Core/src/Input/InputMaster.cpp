#include "Core/Input/InputMaster.h"

#include <algorithm>

namespace Core
{
	Math::Vector2 InputMaster::mousePos = Math::Vector2(0.5f, 0.5f);
	Math::Vector2 InputMaster::mouseDelta = Math::Vector2(0.0f, 0.0f);

	void InputMaster::DispatchInputEvent(const InputEvent& event)
	{
		for (auto& receiver : _eventReceivers)
		{
			receiver->OnInput(event);
		}
	}

	void InputMaster::RegisterEventReceiver(IInputEventReceiver* inputEventReceiver)
	{
		_eventReceivers.push_back(inputEventReceiver);
	}

	void InputMaster::UnregisterEventReceiver(IInputEventReceiver* inputEventReceiver)
	{
		auto index = std::find(_eventReceivers.begin(), _eventReceivers.end(), inputEventReceiver);
		if (index != _eventReceivers.end())
		{
			_eventReceivers.erase(index);
		}
	}
}