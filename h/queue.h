#ifndef H_QUEUE_H_
#define H_QUEUE_H_

class PCB;
typedef int ID;

class Queue{
public:
	Queue() : head(0), tail(0), size(0){}
	~Queue();

	void inject (PCB*);
	int  getSize();

	PCB* find(ID id);
	PCB* deleteHead();
	PCB* deleteById(ID id);

	int time_tick();

private:
	friend class PCB;

	struct Elem {
		PCB*  pcb;
		Elem* next;

		Elem(PCB* process) : pcb(process), next(0) {}

	};

	Elem *head;
	Elem *tail;
	int size;
};



#endif /* H_QUEUE_H_ */
