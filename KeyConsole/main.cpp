#include <iostream>
#include <string>
#include "winner.h"
#include "console.h"

#pragma comment( linker, "/SUBSYSTEM:WINDOWS" )


int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ wchar_t* lpCmdLine,
	_In_ int nShowCmd )
{
	// The usual Windows code
	UNREFERENCED_PARAMETER( hPrevInstance );

	KeyConsole& console = KeyConsole::getInstance();

	auto nW = console.log( L"Using the log function\
Whatever man. Console output works.\n" );
	console.print( L"Console mode is: " + console.getConsoleModeStr() + L'\n');
	printf( "using printf\nA quick brown fox jumps over the lazy dog\n" );
	console.print( L"Console mode is: " + console.getConsoleModeStr() + L'\n');
	std::wcout << nW << L" characters written\n" << L'\n';
	std::wcout << L"Using wcout\n\t\t\t all good it seems" << L'\n';
	console.print( L"Console mode is: " + console.getConsoleModeStr() + L'\n');
	console.print( L"Using the print function - all good\n" );
	console.print( L"about to use console.read.." );
	auto readText = console.read();
	console.print( L"Console mode is: " + console.getConsoleModeStr() + L'\n');
	console.print( L"Read: "
		+ readText
		+ L"\nConsisting of "
		+ std::to_wstring( readText.length() )
		+ L" characters.\n" );
	console.print( L"Console mode is: " + console.getConsoleModeStr() + L'\n');
	std::system( "pause" );
	return 0;
}