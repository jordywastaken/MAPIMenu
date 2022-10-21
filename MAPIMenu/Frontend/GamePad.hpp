
#pragma once

#include <cell/pad/pad_codes.h>

class GamePad
{
public:
	enum Buttons : int {
		BUTTON_L2,
		BUTTON_R2,
		BUTTON_L1,
		BUTTON_R1,
		BUTTON_TRIANGLE,
		BUTTON_CIRCLE,
		BUTTON_CROSS,
		BUTTON_SQUARE,
		BUTTON_SELECT,
		BUTTON_L3,
		BUTTON_R3,
		BUTTON_START,
		BUTTON_PAD_UP,
		BUTTON_PAD_RIGHT,
		BUTTON_PAD_DOWN,
		BUTTON_PAD_LEFT,
		BUTTON_PS,

		BUTTONS_COUNT
	};

	enum Analogs : int {
		ANALOG_RIGHT_X,
		ANALOG_RIGHT_Y,
		ANALOG_LEFT_X,
		ANALOG_LEFT_Y,

		ANALOGS_COUNT
	};

public:
	GamePad() = default;

	void Process(CellPadData* padData);

	void DisableSystemButton(Buttons btn, bool disable);
	void DisableAllSystemButtons(bool disable);
	void DisableSystemAnalog(Analogs analog, bool disable);
	void DisableAllSystemAnalogs(bool disable);

	bool IsButtonDown(Buttons btn);
	bool IsButtonDownFor(Buttons btn, uint64_t millis);
	bool IsButtonDownWithDelay(Buttons btn, uint64_t delay);
	bool IsButtonPressed(Buttons btn);
	bool IsButtonReleased(Buttons btn);
	bool IsButtonReleasedAfter(Buttons btn, uint64_t millis);
	bool IsButtonBinds(Buttons btn1, Buttons btn2);
	bool IsButtonBindsFor(Buttons btn1, Buttons btn2, uint64_t millis);

	// X axis: -1.00 = left | 1.00 = right
	// Y axis: -1.00 = top  | 1.00 = bottom
	double GetAnalogInput(Analogs analog);
	double GetRightAnalogX();
	double GetRightAnalogY();
	double GetLeftAnalogX();
	double GetLeftAnalogY();

	// Deadzone is set to 20 by default
	void SetAnalogDeadzone(int16_t deadzone);

private:
	void ProcessDisabledButtons(CellPadData* padData);
	void ProcessDisabledAnalogs(CellPadData* padData);
	void ProcessButtonsPressed();

private:
	CellPadData m_PadData{};
	uint32_t m_CurrentButtonsData{};
	uint32_t m_PreviousButtonsData{};

	bool m_IsButtonDisabled[BUTTONS_COUNT]{};
	bool m_IsAnalogDisabled[ANALOGS_COUNT]{};

	uint64_t m_CurrentTime{};
	uint64_t m_ButtonLastTimeDelay[BUTTONS_COUNT]{};
	uint64_t m_ButtonLastTimePressed[BUTTONS_COUNT]{};

	int16_t m_AnalogDeadzone = 20;
};

extern GamePad g_GamePad;
