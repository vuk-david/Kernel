#ifndef HEADERS_SEMAPHOR_H_
#define HEADERS_SEMAPHOR_H_

#include "kernel.h"


void lock();
void unlock();

typedef unsigned int Time;
class KernelSem;

class Semaphore {

public:
	Semaphore (int init = 1);
	virtual ~Semaphore ();
	virtual int wait (Time maxTimeToWait);
	virtual int signal(int n = 0);
	int val () const; // Returns the current value of the semaphore

private:
	KernelSem* myImpl;
};


#endif /* HEADERS_SEMAPHOR_H_ */
