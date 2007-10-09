#ifndef __SYSTEM_BASE__
#define __SYSTEM_BASE__
/////////////////////////////////////////////////////////////////////
// system_base.h
// Initial Version: HAM
//
// CSystemBase is a pure virtual class that provides basic mechanisms
// common to all 'system' class, like initialization and shutdown
//
/////////////////////////////////////////////////////////////////////

class CSystemBase {
public:
	CSystemBase();
	virtual ~CSystemBase();

	// --------------------------------------------------------------
	//								               Public: Interfaces
	bool Initialize(void *initStructure = 0);
	bool Shutdown();

protected:

	// --------------------------------------------------------------
	//							    Protected: Pure virtual functions

	/// InitializeSystem must be defined in derived classes.  This
	/// function gets called within the Initialize function.
	virtual bool InitializeSystem(void *initStructure) = 0;

	/// ShutdownSystem must be defined in derived classes.  This
	/// function gets called within the Shutdown function.
	virtual bool ShutdownSystem() = 0;

private:
	bool isShutdownComplete;
	bool isInitialized;
};
#endif