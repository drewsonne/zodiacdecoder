/////////////////////////////////////////////////////////////////////
// system_base.cpp
// Initial Version: HAM
/////////////////////////////////////////////////////////////////////

#include "system_base.h"
#include "assert.h"

/////////////////////////////////////////////////////////////////////
//							                 CSystemBase::CSystemBase
/////////////////////////////////////////////////////////////////////
CSystemBase::CSystemBase(){
	isShutdownComplete = true;
	isInitialized = false;
}

/////////////////////////////////////////////////////////////////////
//							                CSystemBase::~CSystemBase
/////////////////////////////////////////////////////////////////////
CSystemBase::~CSystemBase(){
}

// ------------------------------------------------------------------
//								                   Public: Interfaces
/////////////////////////////////////////////////////////////////////
//							                  CSystemBase::Initialize
/////////////////////////////////////////////////////////////////////
bool CSystemBase::Initialize(void *initStructure){

	// If we are already initialized then don't do it again
	if (isInitialized) {
		return true;
	}

	// We have requeted initialization so we will need to shutdown
	isShutdownComplete = false;

	// Call the derived class' initialize function and pass its result
	return isInitialized = InitializeSystem(initStructure);
}

/////////////////////////////////////////////////////////////////////
//							                    CSystemBase::Shutdown
/////////////////////////////////////////////////////////////////////
bool CSystemBase::Shutdown(){

	// If we are already shutdown then don't do it again
	if (isShutdownComplete) {
		return true;
	}

	// We have requested a shutdown so are no longer initialized
	isInitialized = false;

	// Call the derived class' shutdown function and pass its result
	return isShutdownComplete = ShutdownSystem();
}