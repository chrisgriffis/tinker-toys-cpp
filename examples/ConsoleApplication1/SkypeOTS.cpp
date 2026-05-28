// 
// Microsoft OTS
// 
// Log out
// 
// sfb
// 
// Given an integer matrix M[p][q], the following condition is guaranteed :
// 
// M[i][k] <= M[i + 1][k] for all 0 <= i < p - 1, and
// 
// 	M[k][j] <= M[k][j + 1] for all 0 <= j < q - 1
// 
// 	How many negative integers are in the matrix ?
// 
int countNegatives(int** M, int p, int q);

// Folding a singly linked list

// Given a singly linked list, eg. {1, 2, 3, 4, 5, 6}, implement foldList such that foldList({ 1,2,3,4,5,6 }) -> {1, 6, 2, 5, 3, 4}, eg.take 1st element, last element, 2nd element, 2nd last element, etc.The result should be an in - place fix up of the list_entries, not a brand new copy of the list

struct list_entry

{

	int val;

	struct list_entry* next;

};

void foldList(struct list_entry* head);
// 
// 
// Imagine you are writing a new framework / platform.
// 
// Given the following synchronization primitives :

interface ILock
{
	void Enter();
	void Leave();
};

interface IEvent
{

	// if autoReset is true, event.Signal() will only wake up all current waiting threads. It will not keep itself in a signaled state.
	// if autoReset is false, event.Signal() will keep state and the next subsequent event.Wait() will pass through
	static CreateEvent(out event, bool autoReset);
	// this method will reset the signaled state for the event. only applicable to an event where autoReset == false
	void Reset();
	void Wait(); // blocks the current thread of execution
	void Signal(); // unblocks all the threads which are waiting for this particular event
};

interface IMutex
{

	// similar to IEvent
	static CreateEvent(out event, bool autoReset);
	// similar to IEvent
	void Reset();
	void Wait(); // blocks the current thread of execution
	void Signal(); // unblock only 1 out of the many threads waiting for this particular mutex
};

// Please note IMutex and IEvent will keep track of which thread is waiting.No need to manage that within your own solution
// 
// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
// 
// Question:
// 
// 
// Please implement CSemaphore with the following signature.You can use any of the structures defined above.
// 

class CSemaphore
{
public:
	void Increment(); // increases internal count and potentially unblocking 1 thread among many that are waiting
	void Decrement(); // reduces the internal count and exit if the count is still available. Blocks the caller if the count is unavaoilable.
};

// example of usage of CSemaphore

CSemaphore g_sem;
void produceWidget()
{
	// produce a widget
	g_sem.Increment();
}

void consumeWidget()
{
	g_sem.Decrement(); // this call should block if there is not enough count within g_sem.
					   // consume a widget
}
// Time Remaining
// 
// © 2016 - Microsoft





//redo answer

#include <iostream>
#include <algorithm>


int countNegatives(int** M, int p, int q)
{
	int numberOfNegatives = 0;
	int i(0);
	//while this row has negatives in it
	//and is in range
	while (i < p && M[i][0] < 0)
	{
		//fond the index in this row of the number that is lower bounded by zero
		//this index is the count of the negative numbers
		numberOfNegatives += (std::lower_bound(M[i], M[i] + q, 0) - M[i]);
		++i;
	}
	//if the first element of a row is non-negative then that row has no negatives.
	return numberOfNegatives;
}



//original answer

int countNegatives(int** M, int p, int q)
{
	int numberOfNegatives = 0;
	int previousIndexOfLargestNegative = j;

	for (int i = 0; ++i; i < p)
	{
		auto iterToJustAfterLargestNegative = std::lower_bound(M[i], M[previousIndexOfLargestNegative], 0);
		previousIndexOfLargestNegative = &(*iterToJustAfterLargestNegative) - M[i] - 1;
		numberOfNegatives += previousIndexOfLargestNegative + 1;
	}

	return numberOfNegatives;
}










struct ScopedLock
{
	ScopedLock(ILock& lock) :m_lock(lock)
	{
		m_lock.Enter();
	}
	~ScopedLock()
	{
		m_lock.Leave();
	}
private:
	ILock& m_lock;
};

class CSemaphore
{
public:
	CSemaphore(ILock& lock, IEvent& ievent) :m_lock(lock), m_event(ievent) {}
	void Increment();
	void Decrement();
private:
	ILock& m_lock;
	IEvent& m_event;
	unsigned int m_counter;
};

void CSemaphore::Increment()
{
	ScopedLock sl(m_lock);
	if (m_counter++ == 0)
		m_event.Signal();
}

void CSemaphore::Decrement()
{
	ScopedLock sl(m_lock);
	if (m_counter == 0)
	{
		m_event.Wait();
	}
	else if (m_counter == 1)
	{
		--m_counter;
		m_event.Wait();
	}
	else --m_counter;

}
