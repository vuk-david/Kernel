#ifndef HEADERS_EVENT_H_
#define HEADERS_EVENT_H_

#include "krnlEve.h"

typedef unsigned char IVTNo;
class KernelEv;

void lock();
void unlock();

class Event {

public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv

private:
	friend class KernelEv;
	KernelEv* myImpl;

};


#endif /* HEADERS_EVENT_H_ */
