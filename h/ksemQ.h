#ifndef HEADERS_KSEMQ_H_
#define HEADERS_KSEMQ_H_

class KernelSem;

void lock();
void unlock();

class KSemQueue{

public:
	KSemQueue();
	~KSemQueue();

	void insert(KernelSem* s);
	void remove(KernelSem* s);

	void tick();
private:
	friend class KernelSem;

	struct Elem {
		KernelSem* sem;
		Elem* next;
		Elem(KernelSem* semaphore) : sem(semaphore), next(0){}
	};

	Elem *head, *tail;

};



#endif /* HEADERS_KSEMQ_H_ */
