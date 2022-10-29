#ifndef HEADERS_PCB_H_
#define HEADERS_PCB_H_


#include "SCHEDULE.h"

typedef unsigned long StackSize;
typedef unsigned int Time;

class Thread;
class Queue;
class KernelSem;

void lock();
void unlock();

enum State { NEW, ACTIVE, BLOCKED, FINISHED };

class PCB{
public:
	PCB(StackSize, Time, Thread *);
	~PCB();

	void start();
	static void wrapper();

	static volatile PCB * running;
	static Thread* idle;

	static Queue* all_threads;
	static unsigned num_of_active_threads;

	static int putInScheduler();

	int my_ID;
	unsigned * stack;
	unsigned sp;
	unsigned ss;

	Time time_slice;
	int waiting;
	State state;

	int lazy_time;
	KernelSem * my_sem;
	Thread* my_thread;

private:
	static int id_cntr;
};

void dispatch();




#endif /* HEADERS_PCB_H_ */
