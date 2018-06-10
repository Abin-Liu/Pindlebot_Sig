#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <windows.h> 

//this code is from Q243953 in case you lose the article and wonder
//where this code came from...
class CSingleton
{
protected:
  DWORD  m_dwLastError;
  HANDLE m_hMutex;

public:
  CSingleton(LPCTSTR szMutexName)
  {
    //be sure to use a name that is unique for this application otherwise
    //two apps may think they are the same if they are using same name for
    //3rd parm to CreateMutex
    m_hMutex = CreateMutex(NULL, FALSE, szMutexName); //do early
    m_dwLastError = GetLastError(); //save for use later...
  }  
   
  ~CSingleton() 
  {
    if (m_hMutex)  //don't forget to close handles...
    {
       CloseHandle(m_hMutex); //do as late as possible
       m_hMutex = NULL; //good habit to be in
    }
  }

  BOOL IsInstanceUnique() 
  {
    return (ERROR_ALREADY_EXISTS != m_dwLastError);
  }
  
};
#endif
