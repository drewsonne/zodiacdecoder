#ifndef __INPUT_SYSTEM__
#define __INPUT_SYSTEM__
/////////////////////////////////////////////////////////////////////
//	input_system.h
//	Initial Version : HAM
//
//	class CInputSystem
//		this class should provide a wrapper to whatever input
//		API we are using
/////////////////////////////////////////////////////////////////////

#define DIRECTINPUT_VERSION 0x0800
#include <assert.h>
#include <dinput.h>
#include "system_base.h"

class CInputSystem : public CSystemBase {

public:

	// --------------------------------------------------------------
	//											           Structures
	struct SInputSystemInfoStructure {
		HINSTANCE instance;
		HWND windowHandle;
	} inputSystemInfo;

	// --------------------------------------------------------------
	//									 Contructors / Deconstructors
	CInputSystem();
	~CInputSystem();

	// --------------------------------------------------------------
	//										       Public: Interfaces
	bool IsKeyDown(int key) const;

	// --------------------------------------------------------------
	//                                 Public: Overridable Interfaces
	bool ReadAllDeviceStates();
	const char* getKeyboardStateBuffer() { return keyBoardStateBuffer;}
protected:
	bool InitializeSystem(void *initStructure);
	bool ShutdownSystem();

	bool CreateMouseDevice();
	bool CreateKeyboardDevice();
	bool AcquireMouse();
	bool AcquireKeyboard();
	bool ReadKeyboardState();
private:
	LPDIRECTINPUT8 diObject;
	LPDIRECTINPUTDEVICE8 mouseDevice;
	LPDIRECTINPUTDEVICE8 keyboardDevice;
	char keyBoardStateBuffer[256];

};



#endif