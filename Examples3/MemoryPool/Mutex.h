#pragma once

#include <windows.h> //needed for critical section
#include <cstdio>

namespace cg
{
    class Mutex
    {
        CRITICAL_SECTION m_Win32CritSec;

    public:
        Mutex()
        { ::InitializeCriticalSection(&m_Win32CritSec); }
        virtual ~Mutex()
        { ::DeleteCriticalSection(&m_Win32CritSec); }

        void Enter()
        { ::EnterCriticalSection(&m_Win32CritSec); }
        void Leave()
        { ::LeaveCriticalSection(&m_Win32CritSec); }

    private:
        Mutex(const Mutex&);
        Mutex& operator=(const Mutex&);

    };

    class Lock
    {
        Mutex& m_Section;

    public:
        Lock(Mutex& criticalSection)
            : m_Section(criticalSection) 
        { 
            printf("enter\n");
            m_Section.Enter(); 
        }

        virtual ~Lock()
        { 
            printf("leave\n");
            m_Section.Leave(); 
        }

    private:
        Lock(const Lock&);
        Lock& operator=(const Lock&);

    };

}
