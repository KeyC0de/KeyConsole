#include <iostream>
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

	auto nW = console.log( "Using the log function\
Whatever man. Console output works.\n" );
	console.print( "Console mode is: " + console.getConsoleModeStr() + '\n');
	printf( "using printf\nA quick brown fox jumps over the lazy dog\n" );
	console.print( "Console mode is: " + console.getConsoleModeStr() + '\n');
	std::cout << nW << " characters written\n" << '\n';
	std::cout << "Using cout\n\t\t\t all good it seems" << '\n';
	console.print( "Console mode is: " + console.getConsoleModeStr() + '\n');
	console.print( "Using the print function - all good\n" );
	console.print( "about to use console.read.." );
	auto readText = console.read();
	console.print( "Console mode is: " + console.getConsoleModeStr() + '\n');
	console.print( "Read: "
		+ readText
		+ "\nConsisting of "
		+ std::to_string( readText.length() )
		+ " characters.\n" );
	console.print( "Console mode is: " + console.getConsoleModeStr() + '\n');

	console.resetInstance();
	std::system( "pause" );
	return 0;
}