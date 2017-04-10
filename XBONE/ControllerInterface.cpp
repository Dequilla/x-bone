#include "ControllerInterface.h"

int ControllerInterface::getConnected()
{
	for (int i = 0; i < 4; i++)
		m_connected[i] = false;

	int controllers = 0;

	DWORD result;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		result = XInputGetState(i, &state);
		if (result == ERROR_SUCCESS)
		{
			controllers++;
			m_connected[i] = true;
		}
	}

	if (controllers > 0)
		return controllers;
	else
		return -1;
}

bool ControllerInterface::vibrate(int controller, WORD leftMotor, WORD rightMotor)
{
	getConnected(); // Scan to make sure we know which controllers are connected

	// Make sure the controller is actually connected
	if (m_connected[controller] == true)
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

		vibration.wLeftMotorSpeed = leftMotor;
		vibration.wRightMotorSpeed = rightMotor;

		DWORD success = XInputSetState(controller, &vibration);

		if (success == ERROR_DEVICE_NOT_CONNECTED)
		{
			std::cout << "Device not connected" << std::endl;
			return false;
		}

		return true; // Success
	}

	return false; // Failure
}
