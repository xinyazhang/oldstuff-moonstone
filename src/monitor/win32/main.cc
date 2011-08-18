#include <windows.h>
#include "../daemon.h"
#include "../version.h"

const wchar_t* SERVICE_NAME = L"LAIRD";

SERVICE_STATUS          service_status; 
SERVICE_STATUS_HANDLE   status_handle; 


VOID WINAPI service_main(
  __in  DWORD dwArgc,
  __in  LPTSTR *lpszArgv
);

VOID WINAPI handler(
  __in  DWORD fdwControl
);

void wmain(int argc, wchar_t* argv[])
{
   SERVICE_TABLE_ENTRYW ServiceTable[2];
   ServiceTable[0].lpServiceName = SERVICE_NAME;
   ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONW)service_main;

   ServiceTable[1].lpServiceName = NULL;
   ServiceTable[1].lpServiceProc = NULL;

   StartServiceCtrlDispatcherW(ServiceTable);  
}

VOID WINAPI service_main(__in  DWORD /*dwArgc*/, __in  LPTSTR* /*lpszArgv*/)
{
	service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
	service_status.dwCurrentState = SERVICE_START_PENDING; 
	service_status.dwControlsAccepted   =  SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	service_status.dwWin32ExitCode = 0; 
	service_status.dwServiceSpecificExitCode = 0; 
	service_status.dwCheckPoint = 0; 
	service_status.dwWaitHint = 0; 

	status_handle = RegisterServiceCtrlHandlerW(SERVICE_NAME, (LPHANDLER_FUNCTION)ControlHandler); 
	SetServiceStatus(status_handle, &service_status); // Report we are starting
	
	int err = daemon_init();
	if (err)
	{
		service_status.dwCurrentState = SERVICE_STOPPED; 
		service_status.dwWin32ExitCode = -err; 
		SetServiceStatus(status_handle, &service_status); 
		return ;
	}

	service_status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(status_handle, service_status);

	daemon_release();
}

VOID WINAPI handler( __in  DWORD fdwControl)
{ 
   switch(request) 
   { 
      case SERVICE_CONTROL_STOP: 
         service_status.dwWin32ExitCode = 0; 
         service_status.dwCurrentState = SERVICE_STOPPED; 
         SetServiceStatus(status_handle, &service_status);
         return; 
 
      case SERVICE_CONTROL_SHUTDOWN: 
         ServiceStatus.dwWin32ExitCode = 0; 
         ServiceStatus.dwCurrentState = SERVICE_STOPPED; 
         SetServiceStatus (status_handle, &service_status);
         return; 
        
      default:
         break;
    } 
 
    // Report current status
    SetServiceStatus (hStatus, &ServiceStatus);
 
    return; 
}

