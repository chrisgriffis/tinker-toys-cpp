//thread.h
class CThread
 {
 public:
     CThread(void);
 public:
     ~CThread(void);

 private:   
     DWORD    m_ThreadID;        // ID of thread
     HANDLE    m_hThread;        // Handle to thread
     BOOL    m_bIsFree;            // Flag indicates which is free thread
     string    m_strMessage;    // Message or Task 
     HANDLE  m_hWorkEvent[2]; // m_hWorkEvent[0] Work Event. m_hWorkEvent[1] ShutDown event


 public:
     void    Run(void); // Function which will get executed by thread
     static    DWORD WINAPI ThreadProc(LPVOID Param); // Thread procedure
     BOOL    IsFree(); // Determine if thread is free or not
     void    CreateWorkerThread();
     HANDLE    GetThreadHandle();
     DWORD    GetThreadID();
     void    SetMessage(string strMessage);
     void    SignalWorkEvent();
     void    SignalShutDownEvent();
     void    SetThreadBusy();
     void    ReleaseHandles();
 }; 
    

2. Thread.cpp



#include "Thread.h"

 /****************************************************************************
 CThread Constructer
 ****************************************************************************/
 CThread::CThread(void)
 {
     // Initialize members
     m_ThreadID    = 0;
     m_hThread    = NULL;
     m_bIsFree    = TRUE;
     m_strMessage = "";

     m_hWorkEvent[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
     m_hWorkEvent[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
 }

 CThread::~CThread(void)
 {

 }

 /****************************************************************************
 CThread ThreadProc 
 Description : Thread Procedure. 
 ****************************************************************************/
 DWORD WINAPI CThread::ThreadProc(LPVOID Param)
 {
     // Create object of ThreadPoolMgr
     CThread* ptrThread = (CThread*)Param;
     
     BOOL bShutDown = FALSE;

     while( !bShutDown )
     {
         DWORD dwWaitResult = WaitForMultipleObjects( 2, ptrThread->m_hWorkEvent , FALSE, INFINITE);
         switch(dwWaitResult)
         {
         case WAIT_OBJECT_0 :
             // Work event signaled Call the Run function    
             ptrThread->Run();
             break;

         case WAIT_OBJECT_0 + 1 :
             bShutDown = TRUE;
             break;
         }
     }

     return 0;
 }

 /****************************************************************************
 CThread Run 
 Description : Main function that each thread executes. 
 ****************************************************************************/
 void CThread::Run(void)
 {    
     ofstream myfile;
     myfile.open(m_strMessage.c_str());
     
     myfile << "Thread " << GetCurrentThreadId() <<" is Running.\n";
     myfile << "Thread Message: "<< m_strMessage.c_str() << "\n";
     myfile.close();
     /*cout << "Thread " << GetCurrentThreadId() <<" is Running.\n";
     cout << "Thread Message: "<< m_strMessage.c_str() << "\n";*/
     
     Sleep(3000);

     // work is done. Now 
     // 1. m_bIsFree flag to true
     // 2. make work event to nonsiganled and 
     // 3. set message string to empty
     m_bIsFree = TRUE;
     m_strMessage.clear();
     ResetEvent(m_hWorkEvent[0]);
 }

 /****************************************************************************
 CThread IsFree 
 Description : returns state of thread. 
 ****************************************************************************/
 BOOL CThread::IsFree()
 {    
     return m_bIsFree;
 }

 void CThread::CreateWorkerThread()
 {
     m_hThread = CreateThread(NULL, NULL, ThreadProc, (LPVOID)this, NULL, &m_ThreadID);
     
     if( m_hThread == NULL )
         cout << "Thread could not be created: " << GetLastError() << "\n";
     else
         cout << "Successfully created thread: " << m_ThreadID << "\n";
 }


 HANDLE CThread::GetThreadHandle()
 {
     return m_hThread;
 }

 DWORD CThread::GetThreadID()
 {
     return m_ThreadID;
 }

 void CThread::SetMessage(std::string strMessage)
 {
     m_strMessage = strMessage;
 }

 void CThread::SignalWorkEvent()
 {
     SetEvent( m_hWorkEvent[0] );
 }

 void CThread::SignalShutDownEvent()
 {
     SetEvent( m_hWorkEvent[1] );
 }

 void CThread::SetThreadBusy()
 {
     m_bIsFree = FALSE;
 }

 void CThread::ReleaseHandles()
 {
     // Close all handles 
     CloseHandle(m_hThread);
     CloseHandle(m_hWorkEvent[0]);
     CloseHandle(m_hWorkEvent[1]);
 }


3. ThreadPoolMgr.h



class CThreadPoolMgr
 {
 public:
     CThreadPoolMgr(void);
 public:
     ~CThreadPoolMgr(void);

 private:
     CThread*    m_ptrCThread[5];
     HANDLE        m_hThreadPool[5]; // Handle will be used in the end of Pool MGR for waiting on all thread to end
     int            m_nThreadCount;
 public:

     void    Initialize(int nThread);    
     void    AssignTask( string strTask );
     void    ShutDown(void);
     int        GetFreeTherad(void);
     wstring    GetTaskMessage(void);
     int        GetThreadCount(void);
     HANDLE    GetMutex(void);

};





4. ThreadPoolMgr.cpp



#include "ThreadPoolMgr.h"



/****************************************************************************
 CThreadPoolMgr Initialize 
 Description : Creates threads. Thread limit is 5
 ****************************************************************************/
 void CThreadPoolMgr::Initialize(int nThread)
 {
     m_nThreadCount = nThread;

     int nCounter = 0;
     int nThreadCount = m_nThreadCount - 1;
     
     while( nCounter <= nThreadCount )
     {
         // Create objects in heap
         m_ptrCThread[nCounter] = new CThread();

         m_ptrCThread[nCounter]->CreateWorkerThread();
         m_hThreadPool[nCounter] = m_ptrCThread[nCounter]->GetThreadHandle();
         // Increment the counter
         nCounter++;
     }    
 }



/****************************************************************************
 CThreadPoolMgr ShutDown 
 Description : Mark shutdown siganl and wait for each thread to end 
 ****************************************************************************/
 void CThreadPoolMgr::ShutDown()
 {
     int Count = 0;

     while(Count <= (m_nThreadCount - 1))
     {
         m_ptrCThread[Count]->SignalShutDownEvent();
         
         Count++;
     }

     // Check if all threads ended successfully
     DWORD dwWaitResult = WaitForMultipleObjects( GetThreadCount(), m_hThreadPool, TRUE, INFINITE);
     
     switch(dwWaitResult)
     {
     case WAIT_OBJECT_0:
         {
             cout << "All threads are ended.\n";
             // Close all handles 
             Count = 0;
             while( Count <= (m_nThreadCount - 1))
             {
                 m_ptrCThread[Count]->ReleaseHandles();
                 delete m_ptrCThread[Count];
                 Count++;
             }
             
             break;
         }

     default:
         cout << "Wait Error: " << GetLastError() << "\n";
     }
     
 }

 /****************************************************************************
 CThreadPoolMgr GetFreeTherad 
 Description : Return no. of free thread. 
 ****************************************************************************/
 int CThreadPoolMgr::GetFreeTherad()
 {
     // Search which thread is free
     int count = 0;
     bool bIsAllBusy = true;
     while(count <= (m_nThreadCount - 1))
     {
         if(m_ptrCThread[count]->IsFree() == TRUE)
         {
             return count;
         }
         else
         {
             cout << "Thread " << count << ": " << m_ptrCThread[count]->GetThreadID() << " is busy!!!\n";
         }
         
         count++;
     }
     if( bIsAllBusy )
     {
         cout << "All thread are busy. Wait for thread to be free!!!\n" ;
         return -1;
     }
 }

 /****************************************************************************
 CThreadPoolMgr AssignTask 
 Description : Assign task to free thread. 
               1. Seacrch which is free thread
               2. Assign task to free thread
 ****************************************************************************/
 void CThreadPoolMgr::AssignTask( string strTask )
 {
     int Count = GetFreeTherad();
     if( Count != -1)
     {
         m_ptrCThread[Count]->SetThreadBusy();
         
         // Set information to thread member so that thread can use it
         m_ptrCThread[Count]->SetMessage(strTask);
                 
         // Signal Work event
         m_ptrCThread[Count]->SignalWorkEvent();
     }
 }

 int CThreadPoolMgr::GetThreadCount()
 {
     return m_nThreadCount;
 }







5.  Main program




 #include "ThreadPoolMgr.h"







int _tmain(int argc, _TCHAR* argv[])
 {
         
     CThreadPoolMgr MGR;


     int Count = 0;
     
     cout << "************************************************************\n\n\nWelcome to ThreadPool!!!\n\nEnter number of thread that u want to start(U can only create 5 threads):)\n";
     cin >> Count;
     

     cout << "Initializing " << Count <<"threads...\n";
     MGR.Initialize(Count);

     MGR.AssignTask( "1 task" );
     MGR.AssignTask( "2 task" );

     MGR.AssignTask( "3 task" );

     MGR.AssignTask( "4 task" );   

     Sleep(5000);
     MGR.ShutDown();

     return 0;
 } 
 