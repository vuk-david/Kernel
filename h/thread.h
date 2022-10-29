#ifndef HEADERS_THREAD_H_
#define HEADERS_THREAD_H_


#include "pcb.h"

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
const StackSize maxStackSize = 65535;

typedef unsigned int Time; // time, x 55ms

const Time defaultTimeSlice = 2; // default = 2*55ms

typedef int ID;

class Thread{
public:
	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

protected:
	friend class PCB;
	Thread (StackSize stack_size = defaultStackSize, Time time_slice = defaultTimeSlice);
	virtual void run() {}

private:
	PCB* myPCB;
	friend void interrupt timer(...);
};


extern void interrupt timer(...);




#endif /* HEADERS_THREAD_H_ */
