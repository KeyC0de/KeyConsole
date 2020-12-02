#include "assertions.h"
#include "console.h"

#if defined _DEBUG && !defined _NDEBUG

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UNICODE
// just builds the error string
bool assertPrint( const wchar_t* expr,
	const wchar_t* file,
	int line,
	const wchar_t* function,
	const wchar_t* msg )
{
	KeyConsole& console = KeyConsole::getInstance();
	console.log( L">> Assertion failed @ file: ");
	console.log( file );
	console.log( L"\n>> function: " );
	console.log( function );
	console.log( L"\n>> line: " );
	console.log( std::to_wstring( line ) );
	console.log( L"\n>> !( " );
	console.log( expr );
	console.log( L" )\n" );
	if ( msg != nullptr )
	{
		console.log( L">> Diagnostic message: " );
		console.log( msg );
		console.log( L"\n" );
		return true;
	}
	return false;
}

#else

// just builds the error string
bool assertPrint( const char* expr,
	const char* file,
	int line,
	const char* function,
	const char* msg )
{
	std::cerr << ">> Assertion failed @ "
		<< file
		<< ", function: "
		<< function
		<< ", line: "
		<< line
		<< " - ! "
		<< expr
		<< '\n';
	if ( msg != nullptr )
	{
		std::cerr << ">> Diagnostic message: "
			<< msg
			<< '\n';
		return true;
	}
	std::cout.flush();
	return false;
}
#endif // UNICODE


#ifdef __cplusplus
}
#endif

#endif // _DEBUG