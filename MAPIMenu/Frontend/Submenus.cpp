
#include "Submenus.hpp"
#include "Base.hpp"
#include "Config.hpp"
#include "../Misc/Syscalls.hpp"
#include "../Misc/Timer.hpp"

void MainMenu()
{
	g_Frontend.SubmenuTitle(L"Main menu");
	g_Frontend.Option(L"Console menu").Submenu(ConsoleMenu);
	g_Frontend.Option(L"Led menu").Submenu(LedMenu);
	g_Frontend.Option(L"Buzzer menu").Submenu(BuzzerMenu);
	g_Frontend.Option(L"Power menu").Submenu(PowerMenu);
	g_Frontend.Option(L"Consoleid menu").Submenu(ConsoleidMenu);
	g_Frontend.Option(L"Bd menu").Submenu(BdMenu);
	g_Frontend.Option(L"Theme menu").Submenu(ThemeMenu);
}

void ConsoleMenu()
{
	g_Frontend.SubmenuTitle(L"Console menu");
	g_Frontend.Option(L"Show mapi version").Action(ShowMapiVersion);
	g_Frontend.Option(L"Show firmware").Action(ShowFirmware);
	g_Frontend.Option(L"Show console type").Action(ShowConsoleType);
}

void LedMenu()
{
	g_Frontend.SubmenuTitle(L"Led menu");
	g_Frontend.Option(L"Green led").Submenu(LedGreenMenu);
	g_Frontend.Option(L"Red led").Submenu(LedRedMenu);
}

void LedGreenMenu()
{
	g_Frontend.SubmenuTitle(L"Green led");
	g_Frontend.Option(L"OFF").Action([]
	{
		sys_sm_control_led(led_green, led_action_off);
		vsh::ShowNavigationMessage(L"Green led off");
	});
	g_Frontend.Option(L"ON").Action([]
	{
		sys_sm_control_led(led_green, led_action_on);
		vsh::ShowNavigationMessage(L"Green led on");
	});
	g_Frontend.Option(L"Blink").Action([]
	{
		sys_sm_control_led(led_green, led_action_blink_fast);
		vsh::ShowNavigationMessage(L"Green led blink");
	});
}

void LedRedMenu()
{
	g_Frontend.SubmenuTitle(L"Red led");
	g_Frontend.Option(L"OFF").Action([]
	{
		sys_sm_control_led(led_red, led_action_off);
		vsh::ShowNavigationMessage(L"Red led off");
	});
	g_Frontend.Option(L"ON").Action([]
	{
		sys_sm_control_led(led_red, led_action_on);
		vsh::ShowNavigationMessage(L"Red led on");
	});
	g_Frontend.Option(L"Blink").Action([]
	{
		sys_sm_control_led(led_red, led_action_blink_fast);
		vsh::ShowNavigationMessage(L"Red led blink");
	});
}

void BuzzerMenu()
{
	g_Frontend.SubmenuTitle(L"Buzzer menu");
	g_Frontend.Option(L"Single").Action([]
	{
		sys_sm_ring_buzzer(ring_buzzer_single);
		vsh::ShowNavigationMessage(L"Ring buzzer 'Single'");
	});
	g_Frontend.Option(L"Double").Action([]
	{
		sys_sm_ring_buzzer(ring_buzzer_double);
		vsh::ShowNavigationMessage(L"Ring buzzer 'Double'");
	});
	g_Frontend.Option(L"Triple").Action([]
	{
		sys_sm_ring_buzzer(ring_buzzer_triple);
		vsh::ShowNavigationMessage(L"Ring buzzer 'Triple'");
	});
	g_Frontend.Option(L"Continuous").Action([]
	{
		sys_sm_ring_buzzer(ring_buzzer_continuous);
		vsh::ShowNavigationMessage(L"Ring buzzer 'Continuous'");
	});
}

void PowerMenu()
{
	g_Frontend.SubmenuTitle(L"Power menu");
	g_Frontend.Option(L"Power off").Action(PowerOff);
	g_Frontend.Option(L"Soft reboot").Action(SoftReboot);
	g_Frontend.Option(L"Hard reboot").Action(HardReboot);
}

void ConsoleidMenu()
{
	g_Frontend.SubmenuTitle(L"Consoleid menu");
	g_Frontend.Option(L"Idps menu").Submenu(ConsoleidIdpsMenu);
	g_Frontend.Option(L"Psid menu").Submenu(ConsoleidPsidMenu);
}

void ConsoleidIdpsMenu()
{
	g_Frontend.SubmenuTitle(L"Idps menu");
	g_Frontend.Option(L"Show current idps").Action(ShowIdps);
	g_Frontend.Option(L"Randomize current idps").Action(RandomizeIdps);
	g_Frontend.Option(L"Show boot idps").Action([]{ vsh::ShowNavigationMessage(L"Not implemented"); });
	g_Frontend.Option(L"Clear boot idps").Action([]{ vsh::ShowNavigationMessage(L"Not implemented"); });
}

void ConsoleidPsidMenu()
{
	g_Frontend.SubmenuTitle(L"Psid menu");
	g_Frontend.Option(L"Show current psid").Action(ShowPsid);
	g_Frontend.Option(L"Randomize current psid").Action(RandomizePsid);
	g_Frontend.Option(L"Show boot psid").Action([]{ vsh::ShowNavigationMessage(L"Not implemented"); });
	g_Frontend.Option(L"Clear boot psid").Action([]{ vsh::ShowNavigationMessage(L"Not implemented"); });
}

void BdMenu()
{
	g_Frontend.SubmenuTitle(L"Bd menu");
	g_Frontend.Option(L"Eject").Action([]
	{
		sys_storage_eject_insert(false);
		vsh::ShowNavigationMessage(L"Eject");
	});
	g_Frontend.Option(L"Insert").Action([]
	{
		sys_storage_eject_insert(true);
		vsh::ShowNavigationMessage(L"Insert");
	});
}

void ThemeMenu()
{
	g_Frontend.SubmenuTitle(L"Theme menu");
	g_Frontend.Option(L"Theme red").Action([]{
		vsh::ShowNavigationMessage(L"Theme red");
		g_Timer.Add(&g_Config.color, gColorRed, 150);
	});
	g_Frontend.Option(L"Theme green").Action([]{
		vsh::ShowNavigationMessage(L"Theme green");
		g_Timer.Add(&g_Config.color, gColorGreen, 150);
	});
	g_Frontend.Option(L"Theme blue").Action([]{
		vsh::ShowNavigationMessage(L"Theme blue");
		g_Timer.Add(&g_Config.color, gColorBlue, 150);
	});
	g_Frontend.Option(L"Theme yellow").Action([]{
		vsh::ShowNavigationMessage(L"Theme yellow");
		g_Timer.Add(&g_Config.color, gColorYellow, 150);
	});
	g_Frontend.Option(L"Move menu up").Action([]{
		g_Frontend.Move({ 0.0, 10.0 });
	});
	g_Frontend.Option(L"Move menu down").Action([]{
		g_Frontend.Move({ 0.0, -10.0 });
	});
	g_Frontend.Option(L"Move menu left").Action([]{
		g_Frontend.Move({ -10.0, 0.0 });
	});
	g_Frontend.Option(L"Move menu right").Action([]{
		g_Frontend.Move({ 10.0, 0.0 });
	});
	g_Frontend.Option(L"Menu sounds").Toggle(g_Config.enableSounds,
		[]{ vsh::ShowNavigationMessage(L"Sounds ON"); },
		[]{ vsh::ShowNavigationMessage(L"Sounds OFF"); });
}