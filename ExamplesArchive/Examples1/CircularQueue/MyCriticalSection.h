#pragma once

//Violates the "do not use existing class libraries" rule
//Exception made on the premise that implementing a signalization algorithm
//manually is beyond the scope of the exercise.
#include <windows.h> //needed for critical section

//This pattern solves the problem of cleanly leaving the critical section
//even in cases of exceptions.
//
//pattern as shown in
//http://jrdodds.blogs.com/blog/2004/08/raii_in_c.html
namespace Griffis
{
    //The CriticalSection class wraps the Windows API calls
    //to provide a dispatch mechanism for the lock.
    class MyCriticalSection
    {
        CRITICAL_SECTION m_Win32CritSec;

    public:
        MyCriticalSection()
        { ::InitializeCriticalSection(&m_Win32CritSec); }
        virtual ~MyCriticalSection()
        { ::DeleteCriticalSection(&m_Win32CritSec); }

        void Enter()
        { ::EnterCriticalSection(&m_Win32CritSec); }
        void Leave()
        { ::LeaveCriticalSection(&m_Win32CritSec); }

    private:
        MyCriticalSection(const MyCriticalSection&);
        MyCriticalSection& operator=(const MyCriticalSection&);

    };

    //CriticalSection lock enters the critical section on construction
    //and exits the critical section on destruction.
    //This enables serialize access to be bound to the lifespan of the
    //CriticalSection object, so even in cases of exceptions,
    //the CriticalSection as always exited when it falls out of scope.
    class MyCriticalSectionLock
    {
        MyCriticalSection& m_Section;

    public:
        MyCriticalSectionLock(MyCriticalSection& criticalSection)
            : m_Section(criticalSection) 
        { 
            m_Section.Enter(); 
        }

        virtual ~MyCriticalSectionLock()
        { 
            m_Section.Leave(); 
        }

    private:
        MyCriticalSectionLock(const MyCriticalSectionLock&);
        MyCriticalSectionLock& operator=(const MyCriticalSectionLock&);

    };

}


