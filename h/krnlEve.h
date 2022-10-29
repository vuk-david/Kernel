#ifndef HEADERS_KRNLEVE_H_
#define HEADERS_KRNLEVE_H_

#include "IVTEntry.h"

typedef unsigned char IVTNo;

#define PREPAREENTRY(no,flag)\
    void interrupt intr##no();\
    IVTEntry ivte##no(no,(pInterrupt) intr##no );\
    void interrupt intr##no(){\
        ivte##no.signal();\
        if(flag)\
        ivte##no.callOld();\
    }

class PCB;

void lock();
void unlock();

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv ();


	//static KernelEv *all_events[256];
	void block();
	void unblock();
	void wait ();
	void signal();

	int creator_id;
	PCB* blocked;

};




#endif /* HEADERS_KRNLEVE_H_ */
