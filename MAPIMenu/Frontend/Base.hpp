
#pragma once

#include <vshlib.hpp>

using Function = void(*)();

static float gColorRed[]{ 0.9, 0.1, 0.01, 0.6 };
static float gColorGreen[]{ 0.008, 0.5, 0.2, 0.6 };
static float gColorBlue[]{ 0.0, 0.0, 1.0, 0.6 };
static float gColorYellow[]{ 1.0, 1.0, 0.0, 0.6 };

class Widgets
{
public:
	paf::PhPlane* m_Background{};
	paf::PhPlane* m_Header{};
	paf::PhPlane* m_Footer{};
	paf::PhPlane* m_HighlightBar{};

	paf::PhText* m_Title{};
	paf::PhText* m_Submenu{};

	static constexpr int maxOptions{ 10 };
	paf::PhText* m_Options[maxOptions]{};

	void Create();
	void Destroy();
	bool IsAllocated();

	static void ViewportScale(paf::vec2& vec);
	static void ViewportScaleHorizontal(float& value);
	static void ViewportScaleVertical(float& value);

	static void SetPlanePosAndSize(paf::PhPlane* plane, paf::vec2 pos, paf::vec2 size);
	static void SetTextPosAndHeight(paf::PhText* text, paf::vec2 pos, float height);
};

class Frontend
{
public:
	Frontend() = default;
	Frontend(Function mainMenu);

	void OnUpdate();
	void OnShutdown();

	void Move(paf::vec2 movement);

	Frontend& SubmenuTitle(wchar_t const* submenu);
	Frontend& Option(wchar_t const* option);
	Frontend& Action(Function action);
	Frontend& Toggle(bool& var, Function onEnable, Function onDisable);
	Frontend& Submenu(Function submenu);

private:
	void UpdateWidgets();
	void UpdateUI();
	void UpdateGamePad();

	void OnOpen();
	void OnClose();
	void OnCancel();
	void OnScroll(int direction); // -1 up | 1 down

	void ClearTexts();
	void EnterSubmenu(Function submenu);
	void PlaySound(char const* alias);
	bool IsPressed();

	bool m_Opened{};

	int m_CurrentOption{};
	int m_CurrentPrintedOption{};
	int m_TotalOptions{};

	static constexpr int maxSubmenuLevels{ 5 };
	Function m_LastSubmenu[maxSubmenuLevels]{};
	Function m_CurrentSubmenu{};
	Function m_SubmenuToEnter{};
	int m_CurrentSubmenuLevel{};

	float m_SizeWidth{ 240.0 };
	float m_SizeMinHeight{ 92.0 };
	float m_SizeMaxHeight{ 346.0 };
	float m_SizeBackground{ 92.0 };
	float m_SizeHeader{ 66.0 };
	float m_SizeFooter{ 26.0 };
	float m_SizeOption{ 20.0 };
	float m_SizeOptionSpacing{ 1.15 };

	float m_HighlightBarStart{ 116.0 }; // m_SizeHeader + m_SizeOption * 2.5
	float m_HighlightBarPos{ 0.0 };

	float m_OptionAlpha{ 0.0 };

	Widgets m_Widgets{};
};

extern Frontend g_Frontend;