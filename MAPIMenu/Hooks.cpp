
#include "Hooks.hpp"
#include "Frontend/Base.hpp"
#include "Frontend/GamePad.hpp"
#include "Misc/Detours.hpp"
#include <vshlib.hpp>

Detour* paf_85D1D23B_hk;
Detour* sub_6248FC_hk;

int paf_85D1D23B_hook(void* r3, float f1)
{
	g_Frontend.OnUpdate();

	return paf_85D1D23B_hk->GetOriginal<int>(r3, f1);
}

int sub_6248FC_hook(void* r3, int pad, void* r5, CellPadData* data, int r7)
{
	int ret = sub_6248FC_hk->GetOriginal<int>(r3, pad, r5, data, r7);

	if (!pad)
		g_GamePad.Process(data);

	return ret;
}

void InstallHooks()
{
	paf_85D1D23B_hk = new Detour(*(int*)paf::paf_85D1D23B, paf_85D1D23B_hook);
	sub_6248FC_hk = new Detour(ResolveBranch(*(int*)sys_io::sys_io_3733EA3C + 0x6C), sub_6248FC_hook);
}

void RemoveHooks()
{
	delete paf_85D1D23B_hk;
	delete sub_6248FC_hk;
}