
#include "GamePad.hpp"
#include "../Misc/Timer.hpp"
#include <vsh/sys_io.hpp>
#include <vsh/stdc.hpp>

GamePad g_GamePad;

void GamePad::Process(CellPadData* padData)
{
	if (!padData)
		return;

	stdc::memcpy(&m_PadData, padData, sizeof(CellPadData));

	// Update buttons data
	m_PreviousButtonsData = m_CurrentButtonsData;
	m_CurrentButtonsData = (m_PadData.button[CELL_PAD_BTN_OFFSET_DIGITAL1] << 8) | m_PadData.button[CELL_PAD_BTN_OFFSET_DIGITAL2];

	m_CurrentTime = Timer::GetTimeNow();

	ProcessDisabledButtons(padData);
	ProcessDisabledAnalogs(padData);
	ProcessButtonsPressed();
}

void GamePad::ProcessDisabledButtons(CellPadData* padData)
{
	uint32_t& buttonData = *(uint32_t*)&padData->button[CELL_PAD_BTN_OFFSET_DIGITAL1];
	for (int i = 0; i < BUTTONS_COUNT; i++)
	{
		if (m_IsButtonDisabled[i])
			buttonData &= (i < 8) ? ~(1 << i) : ~(1 << 8 + i);
	}
}

void GamePad::ProcessDisabledAnalogs(CellPadData* padData)
{
	for (int i = 0; i < ANALOGS_COUNT; i++)
	{
		uint16_t& analogInput = *(uint16_t*)&padData->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X + i];
		if (m_IsAnalogDisabled[i])
			analogInput = 128;
	}
}

void GamePad::ProcessButtonsPressed()
{
	for (int i = 0; i < BUTTONS_COUNT; i++)
	{
		if (IsButtonPressed((Buttons)i))
			m_ButtonLastTimePressed[i] = m_CurrentTime;
	}
}

void GamePad::DisableSystemButton(Buttons btn, bool disable)
{
	m_IsButtonDisabled[btn] = disable;
}

void GamePad::DisableAllSystemButtons(bool disable)
{
	for (auto& b : m_IsButtonDisabled)
		b = disable;
}

void GamePad::DisableSystemAnalog(Analogs analog, bool disable)
{
	m_IsAnalogDisabled[analog] = disable;
}

void GamePad::DisableAllSystemAnalogs(bool disable)
{
	for (auto& a : m_IsAnalogDisabled)
		a = disable;
}

bool GamePad::IsButtonDown(Buttons btn)
{
	return m_CurrentButtonsData & (1 << btn);
}

bool GamePad::IsButtonDownFor(Buttons btn, uint64_t millis)
{
	if ((m_CurrentTime - m_ButtonLastTimePressed[btn]) >= millis)
	{
		if (IsButtonDown(btn))
		{
			m_ButtonLastTimePressed[btn] = m_CurrentTime;
			return true;
		}
	}
	return false;
}

bool GamePad::IsButtonDownWithDelay(Buttons btn, uint64_t delay)
{
	if (m_CurrentTime - m_ButtonLastTimeDelay[btn] >= delay)
	{
		if (IsButtonDown(btn))
		{
			m_ButtonLastTimeDelay[btn] = m_CurrentTime;
			return true;
		}
	}
	return false;
}

bool GamePad::IsButtonPressed(Buttons btn)
{
	bool isCurrentButtonPressed = (m_CurrentButtonsData & (1 << btn)),
		isPreviousButtonPressed = (m_PreviousButtonsData & (1 << btn));
	return isCurrentButtonPressed && !isPreviousButtonPressed;
}

bool GamePad::IsButtonReleased(Buttons btn)
{
	bool isCurrentButtonPressed = (m_CurrentButtonsData & (1 << btn)),
		isPreviousButtonPressed = (m_PreviousButtonsData & (1 << btn));
	return !isCurrentButtonPressed && isPreviousButtonPressed;
}

bool GamePad::IsButtonReleasedAfter(Buttons btn, uint64_t millis)
{
	if (IsButtonReleased(btn))
		return (m_CurrentTime - m_ButtonLastTimePressed[btn]) >= millis;

	return false;
}

bool GamePad::IsButtonBinds(Buttons btn1, Buttons btn2)
{
	return IsButtonDown(btn1) && IsButtonDown(btn2);
}

bool GamePad::IsButtonBindsFor(Buttons btn1, Buttons btn2, uint64_t millis)
{
	return IsButtonDownFor(btn1, millis) && IsButtonDownFor(btn2, millis);
}

double GamePad::GetAnalogInput(Analogs analog)
{
	int16_t analogInput = m_PadData.button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X + analog] - 128;
	if ((analogInput > -m_AnalogDeadzone) && (analogInput < m_AnalogDeadzone))
		return 0.0;

	return static_cast<double>(analogInput) / 128.0;
}

double GamePad::GetRightAnalogX()
{
	return GetAnalogInput(ANALOG_RIGHT_X);
}

double GamePad::GetRightAnalogY()
{
	return GetAnalogInput(ANALOG_RIGHT_Y);
}

double GamePad::GetLeftAnalogX()
{
	return GetAnalogInput(ANALOG_LEFT_X);
}

double GamePad::GetLeftAnalogY()
{
	return GetAnalogInput(ANALOG_LEFT_Y);
}

void GamePad::SetAnalogDeadzone(int16_t deadzone)
{
	m_AnalogDeadzone = deadzone;
}