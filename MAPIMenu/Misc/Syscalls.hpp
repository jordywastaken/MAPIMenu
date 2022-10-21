
#pragma once

#include <sys/prx.h>
#include <sys/syscall.h>

enum shutdown_flags_t : uint16_t
{
	lpar_id_1 = (1 << 0),
	lpar_id_2 = (1 << 1),
	lpar_id_3 = (1 << 2),
	sys_shutdown = (1 << 8),
	sys_reboot = (1 << 9),
	sys_hard_shutdown = (1 << 12),
	sys_load_lpar = (1 << 15)
};

enum led_t : uint8_t
{
	led_green = 1,
	led_red
};

enum led_action_t : uint8_t
{
	led_action_off,
	led_action_on,
	led_action_blink_fast,
	led_action_blink_slow
};

enum ring_buzzer_params_t : uint16_t
{
	ring_buzzer_single = 0x0006,
	ring_buzzer_double = 0x0036,
	ring_buzzer_triple = 0x01B6,
	ring_buzzer_continuous = 0x0FFF
};

struct platform_info_s
{
	char firmware_version[7];
	char platform_id[9];
	uint32_t build_number;
	uint32_t unknown;
}; // size: 0x18

int _sys_ppu_thread_exit(int errorcode);
int _sys_prx_stop_module(sys_prx_id_t id, unsigned int argc, void* argp, int* modres, sys_prx_flags_t flags, sys_prx_stop_module_option_t* pOpt);
sys_prx_id_t _sys_prx_get_module_id_by_address(void* address);
sys_prx_id_t _sys_prx_get_my_module_id();

int sys_sm_shutdown(uint16_t flags);
int sys_sm_control_led(uint8_t led, uint8_t action);
int sys_sm_get_system_info(platform_info_s* platform_info);
int sys_sm_ring_buzzer(uint16_t param);

void sys_storage_eject_insert(bool insert);

int sys_dbg_get_console_type(uint64_t* type);
int sys_dbg_read_process_memory(sys_pid_t pid, void* destination, const void* source, size_t size);
int sys_dbg_write_process_memory(sys_pid_t pid, void* destination, const void* source, size_t size);

int sys_mapi_get_hen();
int sys_mapi_get_mamba();
int sys_mapi_get_version(uint32_t* version);
int sys_mapi_get_core_version();
int sys_mapi_get_idps(uint8_t out[16]);
int sys_mapi_set_idps(uint8_t idps[16]);
int sys_mapi_get_psid(uint8_t out[16]);
int sys_mapi_set_psid(uint8_t psid[16]);
int sys_mapi_read_process_memory(sys_pid_t pid, void* destination, const void* source, size_t size);
int sys_mapi_write_process_memory(sys_pid_t pid, void* destination, const void* source, size_t size);

void ExitModuleThread();
void UnloadMyModule();
bool IsConsoleCex();
bool IsConsoleDex();
bool IsConsoleDeh();
bool IsPayloadHen();
bool IsPayloadMamba();
bool IsPayloadCobra();
int ReadProcessMemory(uint32_t pid, void* destination, const void* source, size_t size);
int WriteProcessMemory(uint32_t pid, void* destination, const void* source, size_t size);

void ShowMapiVersion();
const wchar_t* GetFirmwareType();
const wchar_t* GetPayloadType();
void ShowFirmware();
void ShowConsoleType();
void PowerOff();
void SoftReboot();
void HardReboot();
void ShowIdps();
void ShowPsid();
void RandomizeIdps();
void RandomizePsid();