#pragma once

#define ThreadListHead 0x5E0 // EPROCESS::ThreadListHead
#define ThreadListEntryK 0x2F8 // KTHREAD::ThreadListEntry

PKPRCB_META ke_get_current_prcb()
{	
	return reinterpret_cast<PKPRCB_META(*)()>(ke_get_current_prcb_address)();
}

void hidden_system_threads_detect()
{	
	PKPRCB_META prcb = ke_get_current_prcb();
	PKTHREAD current_thread = prcb->CurrentThread;
	LIST_ENTRY* thread_list_entry = (LIST_ENTRY*)make_ptr(current_thread, ThreadListEntryK);
	PLIST_ENTRY list_entry = thread_list_entry;

	while ((list_entry = list_entry->Flink) != thread_list_entry)
	{
		PKTHREAD_META entry = CONTAINING_RECORD(list_entry, KTHREAD_META, ThreadListEntry);
		ULONG current_tid = reinterpret_cast<ULONG>(PsGetThreadId((PETHREAD)entry));

		if (current_tid != 0)
		{
			PETHREAD p_thread = 0;
			PsLookupThreadByThreadId(reinterpret_cast<HANDLE>(current_tid), &p_thread);

			if (p_thread == 0)
				DbgPrintEx(0, 0, "[PspCidTable] hidden thread id -> %d\n", current_tid);
		}
	}
}