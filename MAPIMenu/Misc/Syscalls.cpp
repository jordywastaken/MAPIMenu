
#include "Syscalls.hpp"
#include <errno.h>
#include <cell/cell_fs.h>
#include <vshlib.hpp>


int _sys_ppu_thread_exit(int errorcode)
{
	system_call_1(41, (uint64_t)errorcode);
	return_to_user_prog(int);
}

int _sys_prx_stop_module(sys_prx_id_t id, unsigned int argc, void* argp, int* modres, sys_prx_flags_t flags, sys_prx_stop_module_option_t* pOpt)
{
	system_call_6(482, (uint64_t)id, (uint64_t)argc, (uint64_t)argp, (uint64_t)modres, (uint64_t)flags, (uint64_t)pOpt);
	return_to_user_prog(int);
}

sys_prx_id_t _sys_prx_get_module_id_by_address(void* address)
{
	system_call_1(461, (uint64_t)address);
	return_to_user_prog(sys_prx_id_t);
}

sys_prx_id_t _sys_prx_get_my_module_id()
{
	return _sys_prx_get_module_id_by_address((void*)_sys_prx_get_my_module_id);
}


/*
sys_sm_shutdown(sys_shutdown);								// power off
sys_sm_shutdown(sys_shutdown | sys_hard_shutdown);			// hard power off ?
sys_sm_shutdown(sys_reboot);								// soft reboot
sys_sm_shutdown(sys_reboot | sys_hard_shutdown);			// hard reboot
sys_sm_shutdown(sys_load_lpar | lpar_id_1 | sys_reboot);	// lv2 reboot
*/
int32_t sys_sm_shutdown(uint16_t flags)
{
	system_call_3(379, (uint64_t)flags, 0, 0);
	return_to_user_prog(int32_t);
}

int32_t sys_sm_control_led(uint8_t led, uint8_t action)
{
	system_call_2(386, (uint64_t)led, (uint64_t)action);
	return_to_user_prog(int32_t);
}

int32_t sys_sm_get_system_info(platform_info_s* platform_info)
{
	system_call_1(387, (uint64_t)platform_info);
	return_to_user_prog(int32_t);
}

int32_t sys_sm_ring_buzzer(uint16_t param)
{
	system_call_3(392, 0x1007, 0xA, (uint64_t)param);
	return_to_user_prog(int32_t);
}

void sys_storage_eject_insert(bool insert)
{
	uint8_t atapi_cmd[56];
	int32_t dev_id;

	stdc::memset(atapi_cmd, 0, 56);

	atapi_cmd[0x00] = 0x1b;
	atapi_cmd[0x01] = 0x01;
	atapi_cmd[0x04] = insert ? 0x03 : 0x02;
	atapi_cmd[0x23] = 0x0c;

	{system_call_4(600, 0x101000000000006ULL, 0, (uint64_t)&dev_id, 0); }	// sys_storage_open
	{system_call_7(616, dev_id, 1, (uint64_t)atapi_cmd, 56, 0, 0, 0); }		// sys_storage_execute_device_command
	{system_call_1(601, dev_id); }											// sys_storage_close
}


// sys_dbg
int sys_dbg_get_console_type(uint64_t* type)
{
	system_call_1(985, (uint64_t)type);
	return_to_user_prog(int);
}

int sys_dbg_read_process_memory(sys_pid_t pid, void* destination, const void* source, size_t size)
{
	system_call_4(904, (uint64_t)pid, (uint64_t)destination, size, (uint64_t)source);
	return_to_user_prog(int);
}

int sys_dbg_write_process_memory(sys_pid_t pid, void* destination, const void* source, size_t size)
{
	system_call_4(905, (uint64_t)pid, (uint64_t)destination, size, (uint64_t)source);
	return_to_user_prog(int);
}


// sys_mapi
int sys_mapi_get_hen()
{
	system_call_1(8, 0x1337ULL);
	return_to_user_prog(int);
}

int sys_mapi_get_mamba()
{
	system_call_1(8, 0x7FFFULL);
	return_to_user_prog(int);
}

int sys_mapi_get_version(uint32_t* version)
{
	system_call_2(8, 0x7000ULL, (uint64_t)(uint32_t)version);
	return_to_user_prog(int);
}

int sys_mapi_get_core_version()
{
	system_call_2(8, 0x7777ULL, 0x0011ULL);
	return_to_user_prog(int);
}

int sys_mapi_get_idps(uint8_t out[16])
{
	system_call_3(8, 0x7777ULL, 0x0081ULL, (uint64_t)out);
	return_to_user_prog(int);
}

int sys_mapi_set_idps(uint8_t idps[16])
{
	system_call_4(8, 0x7777ULL, 0x0082ULL, *(uint64_t*)&idps[0], *(uint64_t*)&idps[8]);
	return_to_user_prog(int);
}

int sys_mapi_get_psid(uint8_t out[16])
{
	system_call_3(8, 0x7777ULL, 0x0083ULL, (uint64_t)out);
	return_to_user_prog(int);
}

int sys_mapi_set_psid(uint8_t psid[16])
{
	system_call_4(8, 0x7777ULL, 0x0084ULL, *(uint64_t*)&psid[0], *(uint64_t*)&psid[8]);
	return_to_user_prog(int);
}

int sys_mapi_read_process_memory(sys_pid_t pid, void* destination, const void* source, size_t size)
{
	system_call_6(8, 0x7777ULL, 0x0031ULL, (uint64_t)pid, (uint64_t)destination, (uint64_t)source, size);
	return_to_user_prog(int);
}

int sys_mapi_write_process_memory(sys_pid_t pid, void* destination, const void* source, size_t size)
{
	system_call_6(8, 0x7777ULL, 0x0032ULL, (uint64_t)pid, (uint64_t)destination, (uint64_t)source, size);
	return_to_user_prog(int);
}


// Customs
void ExitModuleThread()
{
	_sys_ppu_thread_exit(0);
}

void UnloadMyModule()
{
	sys_prx_id_t myModuleId = _sys_prx_get_my_module_id();
	uint64_t memInfo[5]{0x28, 2, 0, 0, 0};
	_sys_prx_stop_module(myModuleId, 0, memInfo, nullptr, 0, nullptr);
}

bool IsConsoleCex()
{
	uint64_t type;
	sys_dbg_get_console_type(&type);
	return type == 1;
}

bool IsConsoleDex()
{
	uint64_t type;
	sys_dbg_get_console_type(&type);
	return type == 2;
}

bool IsConsoleDeh()
{
	uint64_t type;
	sys_dbg_get_console_type(&type);
	return type == 3;
}

bool IsPayloadHen()
{
	return sys_mapi_get_hen() == 0x1337;
}

bool IsPayloadMamba()
{
	return sys_mapi_get_mamba() == 0x666;
}

bool IsPayloadCobra()
{
	uint32_t version = 0x99999999;
	if (sys_mapi_get_version(&version) < 0)
		return false;

	return (version != 0x99999999); // If value changed, it is cobra
}

static bool bCanUseDbgSyscalls = true;
static bool bCanUseCobraSyscalls = true;
static bool bConsoleTypeChecked = false;

int ReadProcessMemory(uint32_t pid, void* destination, const void* source, size_t size)
{
	if (!bConsoleTypeChecked)
	{
		bCanUseDbgSyscalls = !IsConsoleCex();
		bCanUseCobraSyscalls = IsPayloadCobra();
		bConsoleTypeChecked = true;
	}

	if (bCanUseDbgSyscalls)
		return sys_dbg_read_process_memory(pid, destination, source, size);

	if (bCanUseCobraSyscalls)
		return sys_mapi_read_process_memory(pid, destination, source, size);

	return ENOSYS; /* The feature is not yet implemented. */
}

int WriteProcessMemory(uint32_t pid, void* destination, const void* source, size_t size)
{
	if (!bConsoleTypeChecked)
	{
		bCanUseDbgSyscalls = !IsConsoleCex();
		bCanUseCobraSyscalls = IsPayloadCobra();
		bConsoleTypeChecked = true;
	}

	if (bCanUseDbgSyscalls)
		return sys_dbg_write_process_memory(pid, destination, source, size);

	if (bCanUseCobraSyscalls)
		return sys_mapi_write_process_memory(pid, destination, source, size);

	return ENOSYS; /* The feature is not yet implemented. */
}

void ShowMapiVersion()
{
	uint32_t coreVersion = sys_mapi_get_core_version();
	vsh::ShowNavigationMessage(stdc::wva(L"MAPI Core: %p.%p", (coreVersion & 0xFF00) >> 8, coreVersion & 0xFF));
}

const wchar_t* GetFirmwareType()
{
	uint64_t type;
	sys_dbg_get_console_type(&type);

	switch (type)
	{
	case 1: return L"CEX";
	case 2: return L"DEX";
	case 3: return L"DEH";
	default: return L"Unknown firmware";
	}
}
const wchar_t* GetPayloadType()
{
	if (IsPayloadHen()) return L"Hen";
	if (IsPayloadMamba()) return L"Mamba";
	if (IsPayloadCobra()) return L"Cobra";
	return L"Unknown payload";
}

void ShowFirmware()
{
	platform_info_s platformInformation;
	float firmwareVersion = 0.0;

	stdc::memset(&platformInformation, 0, sizeof(platform_info_s));
	sys_sm_get_system_info(&platformInformation);

	// for example:
	// 4.84 : 04 08 40 00
	firmwareVersion = (float)(platformInformation.firmware_version[0] & 0xF);
	firmwareVersion += (float)(platformInformation.firmware_version[1] & 0xF) * 0.1;
	firmwareVersion += (float)((platformInformation.firmware_version[2] & 0xF0) >> 4) * 0.01;

	vsh::ShowNavigationMessage(stdc::wva(L"Firmware: %1.2f %ls %ls", firmwareVersion, GetFirmwareType(), GetPayloadType()));
}

void ShowConsoleType()
{
	vsh::ShowNavigationMessage(stdc::wva(L"Console: %ls %ls", GetFirmwareType(), GetPayloadType()));
}

void PowerOff()
{
	cellFsUnlink("/dev_hdd0/tmp/turnoff");
	sys_sm_shutdown(sys_shutdown);
}

void SoftReboot()
{
	cellFsUnlink("/dev_hdd0/tmp/turnoff");
	sys_sm_shutdown(sys_reboot);
}

void HardReboot()
{
	cellFsUnlink("/dev_hdd0/tmp/turnoff");
	sys_sm_shutdown(sys_reboot | sys_hard_shutdown);
}

void ShowIdps()
{
	uint8_t idps[16];
	sys_mapi_get_idps(idps);

	vsh::ShowNavigationMessage(stdc::wva(L"Idps: %016llX%016llX", *(uint64_t*)&idps[0], *(uint64_t*)&idps[8]));
}

void ShowPsid()
{
	uint8_t psid[16];
	sys_mapi_get_psid(psid);

	vsh::ShowNavigationMessage(stdc::wva(L"Psid: %016llX%016llX", *(uint64_t*)&psid[0], *(uint64_t*)&psid[8]));
}

void RandomizeIdps()
{
	uint8_t idps[16];
	sys_mapi_get_idps(idps);

	for (int i = 0; i < 6; i++)
		idps[10 + i] = stdc::irand(0, 255);

	sys_mapi_set_idps(idps);
	vsh::ShowNavigationMessage(stdc::wva(L"New idps: %016llX%016llX", *(uint64_t*)&idps[0], *(uint64_t*)&idps[8]));
}

void RandomizePsid()
{
	uint8_t psid[16];
	sys_mapi_get_psid(psid);

	for (int i = 0; i < 16; i++)
		psid[i] = stdc::irand(0, 255);

	sys_mapi_set_psid(psid);
	vsh::ShowNavigationMessage(stdc::wva(L"New psid: %016llX%016llX", *(uint64_t*)&psid[0], *(uint64_t*)&psid[8]));
}