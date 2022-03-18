#pragma once

#define ThreadListHead 0x5E0 // EPROCESS::ThreadListHead
#define ThreadListEntryK 0x2F8 // KTHREAD::ThreadListEntry

void hidden_system_threads_detect()
{	
	PKPRCB_META prcb = KeGetCurrentPrcb();
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

			if (p_thread == 0){
				DbgPrintEx(0, 0, "[PspCidTable] hidden thread id -> %d\n", current_tid);
			}else{
				//Get the thread id of the thread returned.
				ULONG RetThreadID = reinterpret_cast<ULONG>(PsGetThreadID(p_thread));
				
				//Check for swapped PspCidTable entry
				if(RetThreadID!=current_tid)
					DbgPrintEx(0, 0, "[PspCidTable] Thread loookup on ID %d returned thread ID %d!\n", current_tid, RetThreadID);
				
				//Check for non-system thread returned when the actual thread from KPRCB is a sys thread
				if(!PsIsSystemThread(p_thread) && PsIsSystemThread((PETHREAD)entry))
					DbgPrintEx(0, 0, "[PspCidTable] Thread loookup on ID %d returned a non-system thread ID %d!\n", current_tid, RetThreadID);
			}	
		}
	}
}
