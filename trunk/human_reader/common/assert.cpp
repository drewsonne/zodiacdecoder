/////////////////////////////////////////////////////////////////////
//	assert.h
//	Initial Version : HAM
//
//	As simple assert mechanism that allows us to break the debugger,
//  track assertion call information and disable specific assertions
//  at run time.
/////////////////////////////////////////////////////////////////////
#include "assert.h"

#ifdef _DEBUG

// We store a list of assertion calls throughout the
// lifetime of the program.
static TAssertCallInfo assertCallInfo;

/////////////////////////////////////////////////////////////////////
//													   call_assertion
/////////////////////////////////////////////////////////////////////
int call_assertion(bool expressionResult, const char *filename, unsigned int lineNumber)
{

	// Iterate through the assertion history we have and see if 
	// this assertion has been called before.
	TAssertCallInfoIt it;
	it = assertCallInfo.begin();

	while (it != assertCallInfo.end()) {

		// If we find the same assertion break and handle it.
		if (!_stricmp((*it).fileName.c_str(), filename) && (*it).lineNumber == lineNumber) {
			break;
		}
		++it;
	}

	if (it != assertCallInfo.end()) {

		// If we found the assertion history then increment its call count and check if we should 
		// break the debugger or get our early.
		++((*it).callCount);
		if ((*it).doNotBreak) {
			return 1;
		}
	} else {

		// This is the first time the assertion has been called.
		// We will make a new info entry for it.
		it = assertCallInfo.insert(it, SAssert_Call_Info(filename,lineNumber, 1));
	}

	// See if we should early ouot of all asserts
#ifdef _INGORE_ALL_ASSERTS
	return 0;
#endif

	// We did not early out so interrupt the debugger.
	__asm {int 3}
	return 0;
}
#endif //_DEBUG