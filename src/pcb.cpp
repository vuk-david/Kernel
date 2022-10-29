#include <dos.h>
#include "pcb.h"
#include <iostream.h>
#include "queue.h"
#include "thread.h"
#include "kernel.h"
#include "IVTEntry.h"

int PCB::id_cntr = 0;
volatile PCB * PCB::running = 0;
unsigned PCB::num_of_active_threads = 0;
Thread* PCB::idle = 0;
Queue * PCB::all_threads = 0;


PCB::PCB(StackSize size, Time slice, Thread * thr): my_ID(id_cntr++), time_slice(slice), stack(0), ss(0), sp(0), state(NEW), my_thread(thr), my_sem(0), lazy_time(0), waiting(0)
{
	if (my_ID != 1)
		PCB::all_threads->inject(this);

	if(my_ID == 0){
		return;
	}

	if(my_ID > 1)
		num_of_active_threads++;

	lock();
		my_sem = new KernelSem(0);
		stack = new unsigned [size];
	unlock();

	// Setujemo I bit PSW registra
	stack[size - 1] = 0x200;
	void (*body)();

	body = PCB::wrapper;


#ifndef BCC_BLOCK_IGNORE

	stack[size - 2] = FP_SEG(body);
	stack[size - 3] = FP_OFF(body);


	ss = FP_SEG(stack + size - 12);
	sp = FP_OFF(stack + size - 12);
#endif

}

PCB::~PCB()
{
	delete my_sem;
	delete[] stack;
}

void PCB::wrapper()
{
	PCB::running->my_thread->run();


	PCB::running->my_sem->signal(-(PCB::running->my_sem->val));
	PCB::running->state = FINISHED;

	num_of_active_threads--;

	dispatch();
}


unsigned lock_cntr = 0;

void lock()
{
	lock_cntr++;
}

void unlock()
{
	if(lock_cntr == 0)
		return;

	lock_cntr--;
}


volatile Time cntr = 1;
volatile int context_switch_on_demand = 0;

volatile unsigned tsp;
volatile unsigned tss;

int PCB::putInScheduler()
{
	return  PCB::running != PCB::idle->myPCB &&
			PCB::running->state != FINISHED &&
			PCB::running->state != BLOCKED;
}

void interrupt timer(...)
{
	// interrupt routine
	if (!context_switch_on_demand)
		cntr--;

	if (context_switch_on_demand || (cntr == 0 && PCB::running->time_slice != 0) || !PCB::putInScheduler() )
	{
		// If there are no locks at all
		if(lock_cntr == 0)
		{
			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
			}
			#endif

			PCB::running->sp = tsp;
			PCB::running->ss = tss;

			// Scheduler
			if(PCB::putInScheduler())
			{
				Scheduler::put((PCB*)PCB::running);
			}

			PCB::running = Scheduler::get();

			if (!PCB::running)
			{
				if(PCB::num_of_active_threads)
				{
					PCB::running = PCB::idle->myPCB;
				}

			}


			tsp  = PCB::running->sp;
			tss  = PCB::running->ss;
			cntr = PCB::running->time_slice;

			#ifndef BCC_BLOCK_IGNORE
			asm{
				mov sp, tsp
				mov ss, tss
			}
			#endif


		}

	}

	/*
		The call of the old interrupt routine that
		was at 08h, and now it is at 60h
		It is called only when a context change is not
		requested - so the old routine is called only
		when an interruption actually occurred
    */
	if (!context_switch_on_demand)
	{
		#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
		#endif

		KernelSem::semTick();
	}
	else
		context_switch_on_demand = 0;
}

void dispatch(){
	#ifndef BCC_BLOCK_IGNORE
	asm sti;
	context_switch_on_demand = 1;
	timer();
	asm cli;
	#endif
}

typedef void interrupt (*pInterrupt)(...);

pInterrupt oldTimer;

// Set new interrupt routine
class MainThread: public Thread{
public:
	MainThread(): Thread(0, defaultTimeSlice){}

};

class Idle: public Thread{
public:
	Idle(): Thread(defaultStackSize, 1){}

	void run()
	{
		while(1);
	}
};

void inic()
{

#ifndef BCC_BLOCK_IGNORE
		lock();
		oldTimer = getvect(0x8);
		setvect (0x8, timer);
		setvect (0x60, oldTimer);
		unlock();
#endif

		PCB::all_threads = new Queue();
		Thread* main_c = new MainThread();
		PCB::idle = new Idle();

		main_c->start();


		PCB::running = Scheduler::get();

}

// Restore old interrupt routine
void restore()
{
#ifndef BCC_BLOCK_IGNORE
		asm cli
		setvect (0x8, oldTimer);
		asm sti
#endif

	IVTEntry::freeup();
}

extern void tick();

void PCB::start()
{
	lock();
		// Impossible to put in the same thread more than once
		if (my_ID != 1 && state == NEW)
		{
			state = ACTIVE;
			Scheduler::put(this);
		}
	unlock();
}
