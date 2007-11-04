#include "windows.h"
#include <windowsx.h> 
#include "game_base.h"
#include "assert.h"
#include <iostream>
#include "texture_manager.h"
// TODO: temporary globa variable to help get us going
CGameBase *gGame = NULL;

HWND NewWindow(LPCTSTR str_Title, int int_XPos, int int_YPos, int int_Width, int int_Height);
bool MainStartUp(void);
bool MainShutDown(void);

/////////////////////////////////////////////////////////////////////
//						   OurWindowProcedure
/////////////////////////////////////////////////////////////////////
LRESULT CALLBACK MainWindowProcedure(HWND han_Wind, UINT uint_Message, WPARAM parameter1, LPARAM parameter2) {	

	// Handle Messages
	switch(uint_Message) { 
	
		// TODO: Clean up the 'breakHere' lines and detemine how to handle all WM_SIZE states
		case WM_SIZE: 
			if (parameter1 == SIZE_RESTORED) {
				gGame->isMinimized = false;
				gGame->isActive = true;
			} else if (parameter1 == SIZE_MINIMIZED) {
				gGame->isMinimized = true;
				gGame->isActive = false;
			} else if (parameter1 == SIZE_MAXIMIZED) {
				gGame->isMinimized = false;
				gGame->isActive = true;
			} else if (parameter1 == SIZE_MAXSHOW) {
				int breakHere = 1;
			} else if (parameter1 == SIZE_MAXHIDE) {
				int breakHere = 1;
			}

			// Do normal behavior
			return DefWindowProc(han_Wind, uint_Message, parameter1, parameter2);

		case WM_ENTERSIZEMOVE:

			// We don't want to render during while adjsuting the window size/pos
			gGame->isActive = false;

			// Do normal behavior
			return DefWindowProc(han_Wind, uint_Message, parameter1, parameter2);

		case WM_EXITSIZEMOVE:

			// We are done moving/sizing, so lets be active again
			gGame->isActive = true;

			// Sync up our window state information
			gGame->graphics.SyncWindowInformation();

			// Do normal behavior
			return DefWindowProc(han_Wind, uint_Message, parameter1, parameter2);

		case WM_ACTIVATEAPP: 
			if (!gGame->isMinimized) {
				gGame->isActive = parameter1 > 0;
			} else {
				int breakHere =1;
			}
			break; 

		case WM_DESTROY:
			MainShutDown();
			break;

		default: 
			return DefWindowProc(han_Wind, uint_Message, parameter1, parameter2);

	}

  return 0L;

}
/////////////////////////////////////////////////////////////////////
//								WinMain
/////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInstance, LPSTR lpcmdline, int nCmdShow) {

	MSG msg; 
	HWND hwnd; 

	//---------------------------------------------------------------
	//													  MainStartUp
	if(!MainStartUp()) {
		MainShutDown();
		return 1;
	}

	bool bRunning=true;

	//---------------------------------------------------------------
	//									            Create the window
	hwnd = NewWindow("Zodiac Decoder", 100, 100, 500, 500);

	if(!hwnd) {
		return FALSE; 
	} 

	//---------------------------------------------------------------
	//									    Initialize the game class

	CGameBase::SGameBaseInitParams gameParams;
	gameParams.windowHandle=hwnd;
	gameParams.instance = hInstance;
	if (!gGame->Initialize(&gameParams)) {
		return FALSE;
	}
		
	ShowCursor(TRUE);

	//---------------------------------------------------------------
	//										             Program Loop
	while(bRunning){
		//-----------------------------------------------------------
		//								                 Message Pump
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) { 
			if(!GetMessage(&msg, NULL, 0, 0)) {
				return (int)msg.wParam; 
			}
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}

		//-----------------------------------------------------------
		//											  Process GameApp
		else if(gGame->ShouldProcessForeground()) {
			// Check for fatal errors
			if (gGame->CheckForFatal()) {
				bRunning = false;
			} else {
				bRunning=gGame->Process();
			}
		} else {
			WaitMessage();
		}
	}

	DestroyWindow(hwnd);
	return 0;
}
///////////////////////////////////////////////////////////////////
//								NewWindow
///////////////////////////////////////////////////////////////////
HWND NewWindow(LPCTSTR str_Title,int int_XPos, int int_YPos, int int_Width, int int_Height)
{

	//---------------------------------------------------------------
	//										 Fill in the winodw class
	WNDCLASSEX wnd_Structure;

	wnd_Structure.cbSize = sizeof(WNDCLASSEX);
	wnd_Structure.style = CS_HREDRAW | CS_VREDRAW;
	wnd_Structure.lpfnWndProc = MainWindowProcedure;
	wnd_Structure.cbClsExtra = 0;
	wnd_Structure.cbWndExtra = 0;
	wnd_Structure.hInstance = GetModuleHandle(NULL);
	wnd_Structure.hIcon = NULL;
	wnd_Structure.hCursor = NULL;
	wnd_Structure.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	wnd_Structure.lpszMenuName = NULL;
	wnd_Structure.lpszClassName = "WindowClassName";
	wnd_Structure.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	//---------------------------------------------------------------
	//										Register the window class
	RegisterClassEx(&wnd_Structure);

	return CreateWindowEx(	WS_EX_CONTROLPARENT, 
							"WindowClassName", 
							str_Title, 
							WS_THICKFRAME |
							WS_OVERLAPPED | 
							WS_CAPTION | 
							WS_SYSMENU | 
							WS_MINIMIZEBOX | 
							WS_MAXIMIZEBOX | 
							WS_VISIBLE, 
							int_XPos, 
							int_YPos, 
							int_Width, 
							int_Height, 
							NULL, 
							NULL, 
							GetModuleHandle(NULL), 
							NULL);

}

///////////////////////////////////////////////////////////////////
//								MainShutDown
///////////////////////////////////////////////////////////////////
bool MainShutDown(void)
{

	//-------------------------------------------------------------
	//										    Kill our game class
	if (gGame != NULL) {
		delete gGame;
		gGame = NULL;
	}

	ShowCursor(TRUE);
	PostQuitMessage(0); 
	return true;
}

///////////////////////////////////////////////////////////////////
//								MainStartUp
///////////////////////////////////////////////////////////////////
bool MainStartUp(void)
{

	//-------------------------------------------------------------
	//										  Create the game class
	if (gGame == NULL) {
		gGame = new CGameBase();
		if (gGame == NULL) {
			return false;
		}
	}

	return true;
}