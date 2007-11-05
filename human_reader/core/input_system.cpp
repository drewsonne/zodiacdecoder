/////////////////////////////////////////////////////////////////////
//	input_system.cpp
//	Initial Version : HAM
//
/////////////////////////////////////////////////////////////////////

#include "input_system.h"

// TODO: for now we will use std out for our debug
#include <iostream>

/////////////////////////////////////////////////////////////////////
//							               CInputSystem::CInputSystem
/////////////////////////////////////////////////////////////////////
CInputSystem::CInputSystem() {
	mouseDevice = NULL;
	keyboardDevice = NULL;
	diObject = NULL;
}

/////////////////////////////////////////////////////////////////////
//							              CInputSystem::~CInputSystem
/////////////////////////////////////////////////////////////////////
CInputSystem::~CInputSystem() {

	// TODO: Not sure if we want to call shutdown in the destructor
	// Shutdown call a pure virtual ShutdownSystem function.  When 
	// we get to the destructor the vtbl no longer points to derived
	// classes
	Shutdown(); 
}

/////////////////////////////////////////////////////////////////////
//							           CInputSystem::InitializeSystem
/////////////////////////////////////////////////////////////////////
bool CInputSystem::InitializeSystem(void *initStructure) {

	// --------------------------------------------------------------
	//					            Get the initialization parameters
	if (initStructure == NULL) {
		return false;
	}

	SInputSystemInfoStructure *initParams = (SInputSystemInfoStructure*)initStructure;

	if (initParams->windowHandle == NULL) {
		return false;
	}

	if (initParams->instance == NULL) {
		return false;
	}

	// --------------------------------------------------------------
	//										   Initialize DirectInput
	HRESULT hr = DirectInput8Create(initParams->instance, 
									DIRECTINPUT_VERSION, 
									IID_IDirectInput8, 
									(void**)&diObject, 
									NULL);

	if (FAILED(hr)) {
		return false;
	}

	inputSystemInfo.instance = initParams->instance;
	inputSystemInfo.windowHandle = initParams->windowHandle;
	
	// --------------------------------------------------------------
	//										     Create input devices
	if (!CreateMouseDevice()) {
		return false;
	}

	if (!CreateKeyboardDevice()) {
		return false;
	}

	// --------------------------------------------------------------
	//												  Acquire Devices
	if (!AcquireMouse()) {
		return false;
	}

	if (!AcquireKeyboard()) {
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//							             CInputSystem::ShutdownSystem
/////////////////////////////////////////////////////////////////////
bool CInputSystem::ShutdownSystem() {

	if (keyboardDevice) {
		keyboardDevice->Unacquire();
		keyboardDevice->Release();
		keyboardDevice=NULL;
	}

	if (mouseDevice) {
		mouseDevice->Unacquire();
		mouseDevice->Release();
		mouseDevice=NULL;
	}

	if (diObject) {
		diObject->Release();
		diObject=NULL;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//							          CInputSystem::CreateMouseDevice
/////////////////////////////////////////////////////////////////////
bool CInputSystem::CreateMouseDevice()
{

	// --------------------------------------------------------------
	//									      Create the mouse device
	HRESULT hr = diObject->CreateDevice(
								GUID_SysMouse, 
								&mouseDevice, 
								NULL
								);

	if (FAILED(hr)) {
		return false;
	}

	// --------------------------------------------------------------
	//											  Set the Data Format 
	hr = mouseDevice->SetDataFormat(&c_dfDIMouse);

	if (FAILED(hr)) {
		return false;
	}

	// --------------------------------------------------------------
	//											Set Cooperative level
	hr = mouseDevice->SetCooperativeLevel(
									inputSystemInfo.windowHandle,
									DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
									);

	if(FAILED(hr)) {
		return false;
	}



	return true;
}

/////////////////////////////////////////////////////////////////////
//							               CInputSystem::AcquireMouse
/////////////////////////////////////////////////////////////////////
bool CInputSystem::AcquireMouse()
{
	if (!mouseDevice) {
		return false;
	}

	HRESULT hr = mouseDevice->Acquire();

	if(FAILED(hr)) {
		return false;
	}

	return true;
}


/////////////////////////////////////////////////////////////////////
//							       CInputSystem::CreateKeyboardDevice
/////////////////////////////////////////////////////////////////////
bool CInputSystem::CreateKeyboardDevice() {

	// --------------------------------------------------------------
	//									   Create the keyboard device
	HRESULT hr = diObject->CreateDevice	(
									GUID_SysKeyboard,
									&keyboardDevice,
									NULL
									);
	if (FAILED(hr)) {
		return false;
	}

	// --------------------------------------------------------------
	//											  Set the Data Format
	hr = keyboardDevice->SetDataFormat(&c_dfDIKeyboard);


	if (FAILED(hr)) {
		return false;
	}

	// --------------------------------------------------------------
	//											Set Cooperative level
	hr = keyboardDevice->SetCooperativeLevel(
										inputSystemInfo.windowHandle,
										DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
										);

	if (FAILED(hr)) {
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//							            CInputSystem::AcquireKeyboard
/////////////////////////////////////////////////////////////////////
bool CInputSystem::AcquireKeyboard()
{
	if (!keyboardDevice) {
		return false;
	}

	HRESULT hr = keyboardDevice->Acquire();

	if(FAILED(hr)) {
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//							          CInputSystem::ReadKeyboardState
/////////////////////////////////////////////////////////////////////
bool CInputSystem::ReadKeyboardState()
{
    HRESULT  hr; 

	if (!keyboardDevice) {
		return false;
	}

	//---------------------------------------------------------------
	//											   Get Keyboard State 
	hr = keyboardDevice->GetDeviceState(
							sizeof(keyBoardStateBuffer),
							(LPVOID)&keyBoardStateBuffer
							);

	if (SUCCEEDED(hr)) {
		return true;
	}

	//	If Keyboard focus lost then reacquire
	if (hr==DIERR_INPUTLOST) {

		// Try again
		if (AcquireKeyboard()) {

			//-------------------------------------------------------
			//						     Get Keyboard State
			hr = keyboardDevice->GetDeviceState(
								sizeof(keyBoardStateBuffer),
								(LPVOID)&keyBoardStateBuffer
								);

			if (SUCCEEDED(hr)) {
				return true;
			}
		}
	}
	
	// TODO: Temp Debug Text
	std::cout << "Keyboard Input Error: " << hr << std::endl;
	return false;
	

}
bool CInputSystem::ReadMouseState()
{
    HRESULT  hr; 

	if (!mouseDevice) {
		return false;
	}

	//---------------------------------------------------------------
	//											   Get Keyboard State 
	hr = mouseDevice->GetDeviceState(
							sizeof(DIMOUSESTATE),
							(LPVOID)&mouseState
							);

	if (SUCCEEDED(hr)) {
		return true;
	}

	//	If Keyboard focus lost then reacquire
	if (hr==DIERR_INPUTLOST) {

		// Try again
		if (AcquireMouse()) {

			//-------------------------------------------------------
			//						     Get Keyboard State
			hr = mouseDevice->GetDeviceState(
								sizeof(DIMOUSESTATE),
								(LPVOID)&mouseState
								);

			if (SUCCEEDED(hr)) {
				return true;
			}
		}
	}
	
	// TODO: Temp Debug Text
	std::cout << "Mouse Input Error: " << hr << std::endl;
	return false;
	

}
/////////////////////////////////////////////////////////////////////
//							                  CInputSystem::IsKeyDown
/////////////////////////////////////////////////////////////////////
bool CInputSystem::IsKeyDown(int key) const
{
	assert (key >= 0);
	assert (key < sizeof(keyBoardStateBuffer));
	return (keyBoardStateBuffer[key] & 0x80)? true:false;
}

/////////////////////////////////////////////////////////////////////
//							        CInputSystem::ReadAllDeviceStates
/////////////////////////////////////////////////////////////////////
bool CInputSystem::ReadAllDeviceStates() {

	if (keyboardDevice) {
		if (!ReadKeyboardState()) {
			return false;
		}
	}

	if (mouseDevice) {
		if (!ReadMouseState()) {
			return false;
		}
	}
	return true; 
}