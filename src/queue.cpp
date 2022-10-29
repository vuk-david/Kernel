#include "queue.h"
#include "thread.h"

Queue::~Queue()
{
	Elem* elem = head;
	PCB* rem = 0;

	if(elem)
	{
		while(elem)
		{
			rem = elem->pcb;

			lock();
				delete rem;
			unlock();

			elem = elem->next;
		}
	}

	head = tail = 0;
	size = 0;
}


#include <iostream.h>

void Queue::inject(PCB* pcb)
{
	Elem *elem = head;

	while(elem && elem->pcb->my_ID != pcb->my_ID)
	{
		elem = elem->next;
	}

	// Already exists
	if(elem)
		return;

	lock();
		if(!head)
			head = tail = new Elem(pcb);
		else
		{
			tail->next = new Elem(pcb);
			tail = tail->next;
		}
		size++;
	unlock();
}


int Queue::getSize()
{
	return size;
}


PCB* Queue::find(ID id)
{
	Elem *elem = head;
	PCB* found = 0;

	// Empty queue
	if(!elem)
		return 0;

	while(elem && elem->pcb->my_ID != id)
	{
		elem = elem->next;
	}

	if(!elem)
		return 0;
	else
		found = elem->pcb;

	return found;
}

PCB* Queue::deleteById(ID id)
{
	Elem* elem = head;
	PCB* found = 0;
	PCB* remove = 0;

	// Empty queue
	if(!elem)
		return 0;

	if(head->pcb->my_ID == id)
	{
		lock();
			head = head->next;
			found = elem->pcb;

			if(!head)
			{
				head = tail = 0;
			}

			size--;
			delete elem;
		unlock();

		return found;
	}

	while(elem->next && elem->next->pcb->my_ID != id)
	{
		elem = elem->next;
	}

	if(elem->next)
	{
		lock();
			found = elem->next->pcb;
			remove = elem->next->pcb;

			elem->next = elem->next->next;

			delete remove;
		unlock();

		return found;
	}


	return 0;
}


PCB* Queue::deleteHead()
{
	Elem *elem = head;
	PCB* removed = 0;

	// Empty queue
	if(!elem)
		return 0;

	lock();
		if(head == tail)
			head = tail = 0;
		else
			head = head->next;

		removed = elem->pcb;
		size--;
		delete elem;
	unlock();

	return removed;
}

int Queue::time_tick()
{
	int increment = 0;
	PCB* process = 0;
	Elem* elem = head;

	for (int i = 0; i < size; i++)
	{
		if(elem->pcb->lazy_time > 0)
			elem->pcb->lazy_time--;

		// Time's up
		if(elem->pcb->lazy_time == 0 && elem->pcb->waiting == 1)
		{
			lock();
				increment++;

				process = deleteById(elem->pcb->my_ID);


				process->state = ACTIVE;
				Scheduler::put(process);
			unlock();
		}
		elem = elem->next;
	}

	return increment;
}






