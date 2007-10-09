#ifndef __ASSERT__H__
#define __ASSERT__H__

#ifdef NDEBUG
#undef assert
#define assert(_Expression) ((void)0)
#else  // NDEBUG

/// Define the symbol _INGORE_ALL_ASSERTS to obviously ignore all asserts
/// This will still call and collect assertion information but it will not
/// break the debugger.
//#define _INGORE_ALL_ASSERTS

// TODO: Get rid of this.  We need our own vector template class
#include <list>
#include <string>

struct SAssert_Call_Info;

typedef std::string TAssertFileName;
typedef std::list<SAssert_Call_Info> TAssertCallInfo;
typedef TAssertCallInfo::iterator TAssertCallInfoIt;

/// Structure to hold information about assertion calls.
/// An assertion is considered unique and is identified
/// by the file and line number it is called from.
/// The number of calls to a unique assertion is tracked
/// through out the lifetime of the program.
/// TODO: allow a dump of this information
struct SAssert_Call_Info {

	// The source filename the assertion is called from
	TAssertFileName fileName;

	// The linenumber within the source file the assertion is called from
	unsigned int lineNumber;

	// The number of times this assertion has been called.
	unsigned int callCount;

	// We can set this flag to true through the debugger for 
	// individual assertions.
	bool doNotBreak;

	SAssert_Call_Info():fileName(""), lineNumber(0), callCount(0),doNotBreak(false){};
	SAssert_Call_Info(TAssertFileName filename, unsigned int lineNo, unsigned int count = 0):fileName(filename), lineNumber(lineNo), callCount(count),doNotBreak(false){};
};

#undef assert
int call_assertion(bool expressionResult, const char *filename, unsigned int lineNumber); 
#define assert(_Expression) ((!!(_Expression)) || call_assertion(#_Expression, __FILE__, __LINE__))
#endif

#endif //__ASSERT__H__