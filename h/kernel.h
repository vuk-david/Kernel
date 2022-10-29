#ifndef KERNEL_H_
#define KERNEL_H_

#include "queue.h"
#include "pcb.h"

class Sempahore;
class KSemQueue;
class Queue;

typedef unsigned int Time;

void lock();
void unlock();

class KernelSem{
public:
	KernelSem(int);
	~KernelSem();

	void block();
	void unblock();

	int wait(Time);
	int signal(int);

	static KSemQueue* semaphore_list;
	Queue* blocked_threads;
	int val;

	static void semTick();
private:
	friend class KSemQueue;
};



#endif /* KERNEL_H_ */

