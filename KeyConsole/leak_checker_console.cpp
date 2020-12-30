#if defined(_DEBUG) || defined(DEBUG) || defined(debug) || defined(_debug)

// LNK2038	mismatch detected for '_ITERATOR_DEBUG_LEVEL': value '2' doesn't match value '0' in main.obj
// It means you're probably missing debug and release builds in libraries you include
//#	define _ITERATOR_DEBUG_LEVEL 0		// disabled (for release builds)
//#define _ITERATOR_DEBUG_LEVEL 1	// enabled (if _SECURE_SCL is defined)
//#define _ITERATOR_DEBUG_LEVEL 2	// enabled (for debug builds)

#	include "leak_checker_console.h"
#	include "console.h"

namespace debugLeak
{

class LeakChecker
{
public:
	LeakChecker()
	{
		KeyConsole& console = KeyConsole::getInstance();
		console.log( L"Memory leak checker setup\n" );
		setupLeakChecker();
	}
	~LeakChecker()
	{
		KeyConsole& console = KeyConsole::getInstance();
		if ( anyMemoryLeaks() )
		{
			console.log( L"Leaking..\n" );
		}
		else
		{
			console.log( L"No leaks. : )\n" );
		}
	}
	static inline void setupLeakChecker()
	{
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF
			| _CRTDBG_REPORT_FLAG
			| _CRTDBG_LEAK_CHECK_DF );
		return;
	}
};

LeakChecker leakChecker{};

};// namespace debugLeak

#endif	// _DEBUG