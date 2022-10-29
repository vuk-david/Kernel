#include "ksemQ.h"
#include "kernel.h"

KSemQueue::KSemQueue() : head(0), tail(0){}

KSemQueue::~KSemQueue()
{
	Elem* elem  = head;

	while (elem)
	{
		Elem* rem = elem;
		elem = elem->next;
		delete rem;
	}

	head = tail = 0;
}


void KSemQueue::insert(KernelSem* s)
{
	lock();
		if (!head)
			head = tail = new Elem(s);
		else
		{
			tail->next = new Elem(s);
			tail = tail->next;
		}
	unlock();
}

void KSemQueue::remove(KernelSem* s)
{
	Elem* elem  = head;
	Elem* hlp = 0;

	while(elem && elem->sem != s)
	{
		hlp = elem;
		elem = elem->next;
	}

	if(elem == 0)
		return;

	lock();
		if (hlp)
			hlp->next = elem->next;
		else
			head = elem->next;

		if(elem == head)
			tail = hlp;
	unlock();

	delete elem;
}

void KSemQueue::tick()
{
	Elem* elem = head;
	int increment = 0;

	while(elem != 0)
	{
		lock();
			increment = elem->sem->blocked_threads->time_tick();
			elem->sem->val += increment;

			elem = elem->next;
		unlock();
	}
}


