#include "library/globals.h"

VOID NTAPI entry_thread(PVOID)
{
	while (1)
	{
		hidden_system_threads_detect();
	}
}

NTSTATUS entry_init()
{
	ke_get_current_prcb_address = reinterpret_cast<PVOID>(find_pattern_page_km(
		"ntoskrnl.exe",
		".text",
		"\x65\x48\x8B\x04\x25\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x89\x5C\x24\x00",
		"xxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxx?"));

	if (ke_get_current_prcb_address == 0)
		return STATUS_UNSUCCESSFUL;

	return STATUS_SUCCESS;
}

NTSTATUS entry_point(PVOID, PVOID)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	
	if (!NT_SUCCESS(entry_init()))
		return status;

	HANDLE h_thread = 0;
	status = PsCreateSystemThread(&h_thread, 0x10000000L, 0, 0, 0, entry_thread, 0);

	if (!NT_SUCCESS(status))
		return status;

	ZwClose(h_thread);

	return status;
}
