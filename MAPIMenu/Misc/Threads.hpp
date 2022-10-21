
#pragma once

#include <sys/ppu_thread.h>
#include <string>

class Thread
{
public:
	Thread()
		: bJoinable(false) {}

	Thread(void(*callback)(), Thread* thisObj, std::string const& name = "Thread()")
		: bJoinable(true), fnCallback(callback), pObj(thisObj)
	{
		sys_ppu_thread_create(&u64Id, [](uint64_t arg) -> void
		{
			Thread* pThread = reinterpret_cast<Thread*>((uint32_t)arg);

			if (pThread && pThread->fnCallback)
				pThread->fnCallback();

			sys_ppu_thread_exit(0);

		}, reinterpret_cast<uint64_t>(thisObj), Priority, StackSize, SYS_PPU_THREAD_CREATE_JOINABLE, name.c_str());
	}

	bool IsJoinable() {
		return bJoinable;
	}

	bool Join() {
		uint64_t u64ExitCode;
		return sys_ppu_thread_join(u64Id, &u64ExitCode) == SUCCEEDED;
	}
	
public:
	sys_ppu_thread_t u64Id = SYS_PPU_THREAD_ID_INVALID;
	bool bJoinable;
	void(*fnCallback)();
	Thread* pObj;

	static constexpr int Priority = 1000; // thread priority 0=highest, 3071=lowest
	static constexpr unsigned int StackSize = 2048; // stack size for newly created threads
};