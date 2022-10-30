
#include "Base.hpp"
#include "Config.hpp"
#include "GamePad.hpp"
#include "../Misc/Timer.hpp"

Frontend g_Frontend;

void Widgets::Create()
{
	if (IsAllocated())
		return;

	paf::View* system_plugin = paf::View::Find("system_plugin");
	if (!system_plugin)
		return;

	paf::PhWidget* page_autooff_guide = system_plugin->FindWidget("page_autooff_guide");
	if (!page_autooff_guide)
		return;

	if (page_autooff_guide->FindChild("m_Background", 0) != nullptr)
		return;

	// Create planes
	m_Background = new paf::PhPlane(page_autooff_guide, nullptr);
	m_Background->SetName("m_Background");

	m_Header = new paf::PhPlane(page_autooff_guide, nullptr);
	m_Header->SetName("m_Header");

	m_Footer = new paf::PhPlane(page_autooff_guide, nullptr);
	m_Footer->SetName("m_Footer");

	m_HighlightBar = new paf::PhPlane(page_autooff_guide, nullptr);
	m_HighlightBar->SetName("m_HighlightBar");

	// Create texts
	m_Title = new paf::PhText(page_autooff_guide, nullptr);
	m_Title->SetName("m_Title")
		.SetStyle(0x13, int(0x70));

	m_Submenu = new paf::PhText(page_autooff_guide, nullptr);
	m_Submenu->SetName("m_Submenu")
		.SetStyle(0x13, int(0x70));

	for (int i = 0; i < 10; i++)
	{
		m_Options[i] = new paf::PhText(page_autooff_guide, nullptr);
		m_Options[i]->SetName(stdc::va("m_Options[%i]", i))
			.SetStyle(0x13, int(0x70));
	}
}

void Widgets::Destroy()
{
	if (!IsAllocated())
		return;

	delete m_Background;
	delete m_Header;
	delete m_Footer;
	delete m_HighlightBar;

	delete m_Title;
	delete m_Submenu;
	for (auto opt : m_Options)
		delete opt;
}

bool Widgets::IsAllocated()
{
	if (!m_Background)
		return false;

	return !m_Background->m_Data.name.empty();
}

void Widgets::ViewportScale(paf::vec2& vec)
{
	paf::vec2 currentViewport{ paf::GetScreenWidth(), paf::GetScreenHeight() };
	paf::vec2 myViewport{ 1280.0, 720.0 };

	float currentAspectRatio = currentViewport.x / currentViewport.y;
	float myAspectRatio = myViewport.x / myViewport.y;

	vec *= currentViewport / myViewport;
	vec.y *= currentAspectRatio / myAspectRatio;
}

void Widgets::ViewportScaleHorizontal(float& value)
{
	paf::vec2 vec{ value, 0 };
	ViewportScale(vec);
	value = vec.x;
}

void Widgets::ViewportScaleVertical(float& value)
{
	paf::vec2 vec{ 0, value };
	ViewportScale(vec);
	value = vec.y;
}

void Widgets::SetPlanePosAndSize(paf::PhPlane* plane, paf::vec2 pos, paf::vec2 size)
{
	if (!plane)
		return;

	ViewportScale(pos);
	ViewportScale(size);
	plane->SetPosition(pos);
	plane->SetSize(size);
}

void Widgets::SetTextPosAndHeight(paf::PhText* text, paf::vec2 pos, float height)
{
	if (!text)
		return;

	height *= 0.775;

	ViewportScale(pos);
	ViewportScaleVertical(height);
	text->SetPosition(pos);
	text->SetTextHeight(height);
}


Frontend::Frontend(Function mainMenu) : m_CurrentSubmenu(mainMenu)
{
	g_Config = Config();
	vsh::ShowNavigationMessage(L"MAPI Menu successfully loaded\r\nPress \uF888 + \uF88A to open");
}

void Frontend::OnUpdate()
{
	UpdateUI();
	UpdateGamePad();

	g_Timer.OnFrameUpdate();
}

void Frontend::OnShutdown()
{
	m_Opened = false;
	m_Widgets.Destroy();
}

void Frontend::Move(paf::vec2 movement)
{
	g_Timer.Add(&g_Config.position, g_Config.position + movement, 150);
	vsh::ShowNavigationMessage(stdc::wva(L"Menu is at X=%.0f Y=%.0f", g_Config.position.x + movement.x, g_Config.position.y + movement.y));
}

Frontend& Frontend::SubmenuTitle(wchar_t const* submenu)
{
	m_Widgets.m_Submenu->SetText(submenu, 0);
	return *this;
}

Frontend& Frontend::Option(wchar_t const* option)
{
	if (m_CurrentPrintedOption >= Widgets::maxOptions)
		return *this;

	m_Widgets.m_Options[m_CurrentPrintedOption]->SetText(option, 0);
	m_CurrentPrintedOption++;
	return *this;
}

Frontend& Frontend::Action(Function action)
{
	if (!action)
		return *this;

	if (IsPressed())
	{
		action();
		PlaySound("snd_system_ok");
	}
	return *this;
}

Frontend& Frontend::Toggle(bool& var, Function onEnable, Function onDisable)
{
	if (IsPressed())
	{
		var ^= 1;

		if (var && onEnable)
			onEnable();
		if (!var && onDisable)
			onDisable();

		PlaySound("snd_system_ok");
	}
	return *this;
}

Frontend& Frontend::Submenu(Function submenu)
{
	if (submenu && IsPressed())
	{
		m_SubmenuToEnter = submenu;
		PlaySound("snd_system_ok");
	}
	return *this;
}

void Frontend::UpdateWidgets()
{
	if (!m_Widgets.IsAllocated())
		return;

	// Planes
	Widgets::SetPlanePosAndSize(
		m_Widgets.m_Background, 
		g_Config.position, 
		paf::vec2(m_SizeWidth, m_SizeBackground));
	m_Widgets.m_Background->SetColor({ 0.0, 0.0, 0.0, 0.6 });

	Widgets::SetPlanePosAndSize(
		m_Widgets.m_Header, 
		paf::vec2(g_Config.position.x, g_Config.position.y + m_SizeBackground / 2 - m_SizeHeader / 2), 
		paf::vec2(m_SizeWidth, m_SizeHeader));
	m_Widgets.m_Header->SetColor(g_Config.color);

	Widgets::SetPlanePosAndSize(
		m_Widgets.m_Footer,
		paf::vec2(g_Config.position.x, g_Config.position.y - m_SizeBackground / 2 + m_SizeFooter / 2),
		paf::vec2(m_SizeWidth, m_SizeFooter));
	m_Widgets.m_Footer->SetColor(g_Config.color);

	Widgets::SetPlanePosAndSize(
		m_Widgets.m_HighlightBar,
		paf::vec2(g_Config.position.x, g_Config.position.y + m_SizeMaxHeight / 2 - m_HighlightBarStart - m_HighlightBarPos),
		paf::vec2(m_SizeWidth, m_SizeOption));
	m_Widgets.m_HighlightBar->SetColor({ g_Config.color.r, g_Config.color.g, g_Config.color.b, g_Config.color.a * m_OptionAlpha });

	// Texts
	Widgets::SetTextPosAndHeight(
		m_Widgets.m_Title,
		paf::vec2(g_Config.position.x, g_Config.position.y + m_SizeBackground / 2 - m_SizeHeader / 2),
		28);
	m_Widgets.m_Title->SetColor({ 1.0, 1.0, 1.0, 1.0 });
	m_Widgets.m_Title->SetText(L"MAPI Menu", 0);

	Widgets::SetTextPosAndHeight(
		m_Widgets.m_Submenu,
		paf::vec2(g_Config.position.x, g_Config.position.y + m_SizeMaxHeight / 2 - m_SizeHeader - m_SizeOption),
		m_SizeOption);
	m_Widgets.m_Submenu->SetColor({ 1.0, 1.0, 1.0, m_OptionAlpha });

	for (int i = 0; i < Widgets::maxOptions; i++)
	{
		Widgets::SetTextPosAndHeight(
			m_Widgets.m_Options[i],
			paf::vec2(g_Config.position.x, g_Config.position.y + m_SizeMaxHeight / 2 - m_HighlightBarStart - ((m_SizeOption + m_SizeOptionSpacing) * i) + (m_SizeOption * 0.15)),
			m_SizeOption);
		m_Widgets.m_Options[i]->SetColor({ 1.0, 1.0, 1.0, m_OptionAlpha });
	}
}

void Frontend::UpdateUI()
{
	if (!m_Widgets.IsAllocated())
		return;

	if (!m_Opened && m_SizeBackground == m_SizeMinHeight)
	{
		m_Widgets.Destroy();
		return;
	}

	if (m_Opened && m_SizeBackground == m_SizeMaxHeight)
	{
		if (!paf::View::Find("xmb_plugin"))
		{
			OnClose();
			return;
		}
	}

	m_TotalOptions = m_CurrentPrintedOption;
	m_CurrentPrintedOption = 0;

	if (m_CurrentSubmenu)
		m_CurrentSubmenu();

	if (m_SubmenuToEnter)
	{
		EnterSubmenu(m_SubmenuToEnter);
		m_SubmenuToEnter = nullptr;
	}

	UpdateWidgets();
}

void Frontend::UpdateGamePad()
{
	if (!m_Opened && m_SizeBackground == m_SizeMinHeight && paf::View::Find("xmb_plugin"))
	{
		if (g_GamePad.IsButtonBinds(GamePad::BUTTON_L1, GamePad::BUTTON_L3))
			OnOpen();
	}
	else if (m_Opened && m_SizeBackground == m_SizeMaxHeight)
	{
		if (g_GamePad.IsButtonDownWithDelay(GamePad::BUTTON_CIRCLE, 150))
			OnCancel();
		else if (g_GamePad.IsButtonDownWithDelay(GamePad::BUTTON_PAD_UP, 115))
			OnScroll(-1);
		else if (g_GamePad.IsButtonDownWithDelay(GamePad::BUTTON_PAD_DOWN, 115))
			OnScroll(1);
	}

	g_GamePad.DisableAllSystemButtons(m_Opened);
}

void Frontend::OnOpen()
{
	m_Opened = true;
	m_CurrentOption = 1;

	m_Widgets.Create();
	ClearTexts();

	g_Timer.Add(&m_SizeBackground, m_SizeMaxHeight, 400);
	g_Timer.Add(&m_OptionAlpha, 1.0, 750);
	m_HighlightBarPos = 0.0;

	PlaySound("snd_system_ok");
}

void Frontend::OnClose()
{
	g_Timer.m_TimerStack.clear();
	g_Timer.Add(&m_Opened, 400);
	g_Timer.Add(&m_SizeBackground, m_SizeMinHeight, 400);
	m_OptionAlpha = 0.0;

	PlaySound("snd_cancel");
}

void Frontend::OnCancel()
{
	g_Timer.m_TimerStack.clear();
	m_HighlightBarPos = 0.0;
	m_CurrentOption = 1;

	if (m_CurrentSubmenuLevel == 0)
	{
		OnClose();
		return;
	}

	m_CurrentSubmenuLevel--;
	m_CurrentSubmenu = m_LastSubmenu[m_CurrentSubmenuLevel];
	ClearTexts();

	PlaySound("snd_cancel");
}

void Frontend::OnScroll(int direction)
{
	TimerData* highlightBarMove = g_Timer.FindTimer(&m_HighlightBarPos);

	if (highlightBarMove)
		*highlightBarMove->m_From = highlightBarMove->m_EndValue;

	if ((direction == -1 && m_CurrentOption > 1) || (direction == 1 && m_CurrentOption < m_TotalOptions))
	{
		m_CurrentOption += direction;
		g_Timer.Add(&m_HighlightBarPos, (m_SizeOption + m_SizeOptionSpacing) * (m_CurrentOption - 1), 200);
	}

	PlaySound("snd_cursor");
}

void Frontend::ClearTexts()
{
	if (!m_Widgets.IsAllocated())
		return;

	m_Widgets.m_Submenu->SetText(L"", 0);
	for (auto opt : m_Widgets.m_Options)
		opt->SetText(L"", 0);
}

void Frontend::EnterSubmenu(Function submenu)
{
	m_LastSubmenu[m_CurrentSubmenuLevel] = m_CurrentSubmenu;
	m_CurrentSubmenu = submenu;
	m_CurrentOption = 1;
	m_CurrentSubmenuLevel++;

	g_Timer.m_TimerStack.clear();
	m_HighlightBarPos = 0.0;

	ClearTexts();
}

void Frontend::PlaySound(char const* alias)
{
	if (!g_Config.enableSounds)
		return;

	paf::View* system_plugin = paf::View::Find("system_plugin");

	if (system_plugin)
		system_plugin->PlaySound(alias);
}

bool Frontend::IsPressed()
{
	if (m_Opened && (m_CurrentOption == m_CurrentPrintedOption))
		return g_GamePad.IsButtonDownWithDelay(GamePad::BUTTON_CROSS, 200);

	return false;
}
