
#include <vshlib.hpp>
#include <vsh/newDelete.hpp>
#include "prxmain.hpp"
#include "Hooks.hpp"
#include "Frontend/Base.hpp"
#include "Frontend/GamePad.hpp"
#include "Frontend/Submenus.hpp"
#include "Misc/Syscalls.hpp"
#include "Misc/Threads.hpp"
#include "Misc/Timer.hpp"

Thread gMainThread;

extern "C"
{
	int __cdecl module_start(size_t args, const void *argp)
	{
		gMainThread = Thread([]
		{
			// Wait until the XMB is fully loaded
			do
			{
				Timer::Sleep(1000);
			} while (!paf::View::Find("explore_plugin"));

			g_Timer = Timer();
			g_GamePad = GamePad();
			g_Frontend = Frontend(MainMenu);

			InstallHooks();

		}, &gMainThread, "start");

		ExitModuleThread();
		return 0;
	}

	int __cdecl module_stop(size_t args, const void *argp)
	{
		Thread moduleStopThread = Thread([]
		{
			RemoveHooks();
			g_Frontend.OnShutdown();

			// Prevents unloading too fast
			sys_ppu_thread_yield();
			Timer::Sleep(1000);

			gMainThread.Join();

		}, &moduleStopThread, "stop");

		moduleStopThread.Join();

		Timer::Sleep(5);

		UnloadMyModule();
		ExitModuleThread();
		return 0;
	}
}