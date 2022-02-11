# Detect-HiddenThread-via-KPRCB
Detect removed thread from PspCidTable.

Going through the system threads through the KPRCB structures,
we can easily determine which of these threads is missing in the cid table, 
checking whether the thread is in the cid table or not is carried out using the "PsLookupThreadByThreadId" function. 

This system does not check the main sheet with threads, you can also iterate through the main table and compare it with the KPRCB table.
