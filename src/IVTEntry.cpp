#include "IVTEntry.h"
#include "krnlEve.h"


IVTEntry * IVTEntry::IVT_entries[256] = {0};

IVTEntry::IVTEntry(IVTNo num, pInterrupt routine): iVTNo(num), old_routine(0), event(0)
{
#ifndef BCC_BLOCK_IGNORE
	old_routine = getvect(iVTNo);

	IVT_entries[iVTNo] = this;

	setvect(iVTNo, routine);
#endif
}


IVTEntry::~IVTEntry()
{
#ifndef BCC_BLOCK_IGNORE
	setvect(iVTNo, old_routine);
#endif
}

IVTEntry * IVTEntry::getIVTEntry(IVTNo number)
{
	return IVT_entries[number];
}

void IVTEntry::setKernelEv(KernelEv * kernelEv)
{
	event = kernelEv;
}

void IVTEntry::callOld()
{
	if (iVTNo == -1)
		return;

	old_routine();
}


void IVTEntry::freeup()
{
	for (int i = 0; i < 256; i++)
	{
		delete IVT_entries[i];
	}

	delete[] IVT_entries;
}


void IVTEntry::signal()
{
	event->signal();
}

