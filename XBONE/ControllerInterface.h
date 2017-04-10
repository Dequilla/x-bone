#pragma once

#include <Windows.h>
#include <Xinput.h>

#include <iostream>

class ControllerInterface
{
	bool m_connected[4] = { false, false, false, false };

public:
	// Scans and returns the number of connected controllers
	// Returns a number between 1-4 on success and -1 on failure
	int getConnected();

	// Vibrate takes in the controller number (0 - 3) and the speed of each motor
	// The motor arguments take a number between 0 - 65535
	bool vibrate(int controller, WORD leftMotor, WORD rightMotor);
};