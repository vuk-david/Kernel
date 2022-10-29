#include "Event.h"

Event::Event(IVTNo iVTNo): myImpl(0)
{
	lock();
		myImpl = new KernelEv(iVTNo);
	unlock();
}

Event::~Event()
{
	lock();
		delete myImpl;
	unlock();
}

void Event::wait()
{
	myImpl->wait();
}

void Event::signal()
{
	myImpl->signal();
}
