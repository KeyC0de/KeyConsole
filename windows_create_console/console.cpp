#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "console.h"
#include "util.h"


DWORD KeyConsole::getFontFamily( HANDLE h )
{
	CONSOLE_FONT_INFO cfi;
	BOOL conFont = GetCurrentConsoleFont( h,
		false,
		&cfi );
	std::wcout << conFont
		<< L"\nnFont="
		<< cfi.nFont
		<< L"fontSize=("
		<< cfi.dwFontSize.X
		<< L','
		<< cfi.dwFontSize.Y
		<< L")\n";
	return conFont ? cfi.nFont
		: -1;
}

void KeyConsole::getConsoleInfo( HANDLE h )
{
	using GETNUMBEROFCONSOLEFONTS = DWORD (WINAPI* )();
    using SETCONSOLEFONT = BOOL (WINAPI*)( HANDLE hConOut, DWORD nFont );
	auto GetNumberOfConsoleFonts =
		(GETNUMBEROFCONSOLEFONTS) GetProcAddress( LoadLibraryW( L"KERNEL32" ),
		"GetNumberOfConsoleFonts" );
    auto SetConsoleFont = (SETCONSOLEFONT) GetProcAddress( LoadLibraryW( L"KERNEL32" ),
		"SetConsoleFont" );
	auto font = getFontFamily( h );
	std::wcout << L"nConsoleFonts="
		<< GetNumberOfConsoleFonts()
		<< L"fontName="
		<< font
		<< L'\n';
}


KeyConsole::KeyConsole( const std::wstring& fontName )
	:
	m_fp{nullptr},
	m_title{std::wstring{ defaultConsoleTitle }
		+ std::wstring{ L" " }
		+ std::wstring{ currentVersion }},
	m_con{STD_OUTPUT_HANDLE},
	m_hMode{stdout},
	m_stdHandle{GetStdHandle( m_con )}
{
	fflush( stdout );

	// 1. Allocate console
	if ( !AllocConsole() )
	{
		MessageBoxW( nullptr,
			L"Unable to create Debug Console.",
			L"Notice",
			MB_ICONEXCLAMATION );
	}
	
	// 2. set console title
	SetConsoleTitleW( m_title.c_str() );

	// 3. set the console codepage to UTF-8 UNICODE
	if ( !IsValidCodePage( CP_UTF8 ) )
	{
		auto r = GetLastError();
		ASSERT_HRES_IF_FAILED_( HRESULT_FROM_WIN32( r ) );
	}
	if ( !SetConsoleCP( CP_UTF8 ) )
	{
		auto r = GetLastError();
		ASSERT_HRES_IF_FAILED_( HRESULT_FROM_WIN32( r ) );
	}
	if ( !SetConsoleOutputCP( CP_UTF8 ) )
	{
		auto r = GetLastError();
		ASSERT_HRES_IF_FAILED_( HRESULT_FROM_WIN32( r ) );
	}
	
	// 4. use a suitable (console) font that supports our desired glyphs
	// - post Windows Vista only
	setFont( fontName );

	// 5. set file stream translation mode - wide character 16Bit Text
	_setmode( _fileno( stdout ), _O_U16TEXT );
	_setmode( _fileno( stderr ), _O_U16TEXT );
	_setmode( _fileno( stdin ), _O_U16TEXT );
	// use w-streams for interaction with the console
	// Remember no mix and match streams!
	// use regular streams for interaction with files
	std::ios_base::sync_with_stdio( false );

	// 6. console startup info..
	SetConsoleTextAttribute( m_stdHandle,
		FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );
	print( L"Console attributes set.\n" );
	print( L"Console mode set to " + getConsoleModeStr() + L'\n' );
	print( L"Console ready.\n\n" );
	std::wcout.clear();	// after this one we are ready to print
}

KeyConsole::~KeyConsole()
{
	closeConsole();
}

DWORD KeyConsole::print( const std::wstring& msg )
{
	m_hMode = stdout;
	m_fp = freopen( "CONOUT$",
		"w",
		m_hMode );
	m_con = STD_OUTPUT_HANDLE;
	m_stdHandle = GetStdHandle( m_con );

	DWORD nWritten = 0;
	auto ret = WriteConsoleW( m_stdHandle,
		msg.c_str(),
		msg.length(),
		&nWritten,
		nullptr );
	if ( !ret )
	{
		ASSERT_HRES_IF_FAILED_( HRESULT_FROM_WIN32( GetLastError() ) );
	}
	return nWritten;
}

DWORD KeyConsole::log( const std::wstring& msg )
{
	m_hMode = stderr;
	m_fp = freopen( "CONERR$",
			"w",
			m_hMode );
	m_con = STD_ERROR_HANDLE;
	m_stdHandle = GetStdHandle( m_con );

	DWORD nWritten = 0;
	auto ret = WriteConsoleW( m_stdHandle,
		msg.c_str(),
		msg.length(),
		&nWritten,
		nullptr );
	if ( !ret )
	{
		ASSERT_HRES_IF_FAILED_( HRESULT_FROM_WIN32( GetLastError() ) );
	}
	return nWritten;

	// _IONBF unbuffered
	// _IOLBF line buffered
	// _IOFBF fully buffered
	// redirect unbuffered STDIN to the console
	/*
	stdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	fd = _open_osfhandle(stdHandle, _O_TEXT);
	m_fp = _fdopen(fd, "r");
	*stdin = *m_fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console
	stdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	fd = _open_osfhandle(stdHandle, _O_TEXT);
	m_fp = _fdopen(fd, "w");
	*stderr = *m_fp;
	setvbuf(stderr, NULL, _IONBF, 0);
	*/
}

std::wstring KeyConsole::read( const uint32_t bytesToAllocate )
{
	m_hMode == stdin;
	m_fp = freopen( "CONIN$",
		"w",
		m_hMode );
	m_con = STD_INPUT_HANDLE;
	m_stdHandle = GetStdHandle( m_con );

	DWORD nRead = 0;
	std::wstring buff;
	buff.reserve( bytesToAllocate );
	auto ret = ReadConsoleW( m_stdHandle,
		buff.data(),
		sizeof( buff ),
		&nRead,
		nullptr );
	if ( !ret )
	{
		ASSERT_HRES_IF_FAILED_( HRESULT_FROM_WIN32( GetLastError() ) );
	}
	return std::wstring( buff );
}

int KeyConsole::getConsoleMode() const noexcept
{
	if ( m_hMode == stdout )
	{
		return 0;
	}
	else if ( m_hMode == stdin )
	{
		return 1;
	}
	else if ( m_hMode == stderr )
	{
		return 2;
	}
	return -1;
}

std::wstring KeyConsole::getConsoleModeStr() const noexcept
{
	std::wstring strMode;
	switch ( m_con )
	{
	case STD_OUTPUT_HANDLE:
		strMode = L"out";
		break;
	case STD_ERROR_HANDLE:
		strMode = L"err";
		break;
	case STD_INPUT_HANDLE:
		strMode = L"in";
		break;
	default:
		break;
	}
	return strMode;
}

// get current console's Code Page. for a list of code pages check link:
// https://docs.microsoft.com/el-gr/windows/desktop/Intl/code-page-identifiers
uint32_t KeyConsole::getConsoleCodePage() const noexcept
{
	return GetConsoleCP();
}

HANDLE KeyConsole::getStdHandle() const noexcept
{
	return m_stdHandle;
}


int32_t KeyConsole::setConsoleCodePage( uint32_t cp )
{
	return SetConsoleCP( cp );
}

void KeyConsole::setFont( const std::wstring& fontName )
{
	CONSOLE_FONT_INFOEX cfie;
	const auto sz = sizeof( CONSOLE_FONT_INFOEX );
	ZeroMemory( &cfie, sz );
	cfie.cbSize = sz;
	cfie.dwFontSize.X = 0;
	cfie.dwFontSize.Y = 14;
	cfie.FontFamily = FF_DONTCARE;
	cfie.FontWeight = FW_NORMAL;
	wcscpy_s( cfie.FaceName,
		fontName.data() );
	SetCurrentConsoleFontEx( m_stdHandle,
		false,
		&cfie );

	getConsoleInfo( m_stdHandle );
}

int32_t KeyConsole::setCurcorPos(_COORD xy /* = { 0,0 } */)
{
	return SetConsoleCursorPosition( m_stdHandle,
		xy );
}

bool KeyConsole::closeConsole()
{
	if ( !FreeConsole() )
	{
		print( L"Console closing down" );
		MessageBoxW( nullptr,
			L"Failed to close the console!",
			L"Console Error",
			MB_ICONERROR );
		return false;
	}
	fclose( m_fp );
	fclose( m_hMode );
	return true;
}

KeyConsole& KeyConsole::getInstance() noexcept 
{
	static KeyConsole instance;
	return instance;
}
