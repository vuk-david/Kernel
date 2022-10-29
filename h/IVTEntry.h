#ifndef HEADERS_IVTENTRY_H_
#define HEADERS_IVTENTRY_H_

#include <dos.h>

typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

class KernelEv;

class IVTEntry{
public:
	IVTEntry(IVTNo num, pInterrupt routine);
	~IVTEntry();

	pInterrupt old_routine;

	void callOld();
	void setKernelEv(KernelEv *);
	void signal();
	static void freeup();

	static IVTEntry* IVT_entries[256];
	static IVTEntry* getIVTEntry(IVTNo x);

	KernelEv * event;
	IVTNo iVTNo;
};


#endif /* HEADERS_IVTENTRY_H_ */
