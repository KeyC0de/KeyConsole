#include <iostream>
#include "console.h"
#include "os_utils.h"
#include "utils.h"


DWORD KeyConsole::getFontFamily( HANDLE h )
{
	CONSOLE_FONT_INFO cfi;
	BOOL conFont = GetCurrentConsoleFont( h,
		false,
		&cfi );
	std::cout << conFont
		<< "\nnFont="
		<< cfi.nFont
		<< "fontSize=("
		<< cfi.dwFontSize.X
		<< ','
		<< cfi.dwFontSize.Y
		<< ")\n";
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
	std::cout << "nConsoleFonts="
		<< GetNumberOfConsoleFonts()
		<< "fontName="
		<< font
		<< '\n';
}


KeyConsole::KeyConsole( const std::string& fontName )
	:
	m_fp{nullptr},
	m_title{std::string{ defaultConsoleTitle }
		+ std::string{ " " }
		+ std::string{ currentVersion }},
	m_con{STD_OUTPUT_HANDLE},
	m_hMode{stdout},
	m_stdHandle{GetStdHandle( m_con )}
{
	fflush( stdout );

	// 1. Allocate console
	if ( !AllocConsole() )
	{
		MessageBoxW( nullptr,
			s2ws( "Unable to create Debug Console." ).data(),
			s2ws( "Notice" ).data(),
			MB_ICONEXCLAMATION );
	}
	
	// 2. set console title
	SetConsoleTitleW( s2ws( m_title ).data() );

	// 3. set the console codepage to UTF-8 UNICODE
	if ( !IsValidCodePage( CP_UTF8 ) )
	{
		printHresultErrorDescription( HRESULT_FROM_WIN32( GetLastError() ) );
	}
	if ( !SetConsoleCP( CP_UTF8 ) )
	{
		printHresultErrorDescription( HRESULT_FROM_WIN32( GetLastError() ) );
	}
	if ( !SetConsoleOutputCP( CP_UTF8 ) )
	{
		printHresultErrorDescription( HRESULT_FROM_WIN32( GetLastError() ) );
	}
	
	// 4. use a suitable (console) font that supports our desired glyphs
	// - post Windows Vista only
	setFont( fontName );

	// 5. set file stream translation mode
	std::ios_base::sync_with_stdio( false );

	// 6. console startup info..
	SetConsoleTextAttribute( m_stdHandle,
		FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );
	print( "Console attributes set.\n" );
	print( "Console mode set to " + getConsoleModeStr() + '\n' );
	print( "Console ready.\n\n" );
	std::cout.clear();	// after this one we are ready to print
}

KeyConsole::~KeyConsole()
{
	closeConsole();
}

DWORD KeyConsole::print( const std::string& msg )
{
	m_hMode = stdout;
	m_fp = freopen( "CONOUT$",
		"w",
		m_hMode );
	m_con = STD_OUTPUT_HANDLE;
	m_stdHandle = GetStdHandle( m_con );

	DWORD nWritten = 0;
	auto ret = WriteConsoleA( m_stdHandle,
		msg.c_str(),
		static_cast<DWORD>( msg.length() ),
		&nWritten,
		nullptr );
	if ( !ret )
	{
		printHresultErrorDescription( HRESULT_FROM_WIN32( GetLastError() ) );
	}
	return nWritten;
}

DWORD KeyConsole::log( const std::string& msg )
{
	m_hMode = stderr;
	m_fp = freopen( "CONERR$",
			"w",
			m_hMode );
	m_con = STD_ERROR_HANDLE;
	m_stdHandle = GetStdHandle( m_con );

	DWORD nWritten = 0;
	auto ret = WriteConsoleA( m_stdHandle,
		msg.c_str(),
		static_cast<DWORD>( msg.length() ),
		&nWritten,
		nullptr );
	if ( !ret )
	{
		printHresultErrorDescription( HRESULT_FROM_WIN32( GetLastError() ) );
	}
	return nWritten;
}

std::string KeyConsole::read( const uint32_t bytesToAllocate )
{
	m_hMode = stdin;
	m_fp = freopen( "CONIN$",
		"w",
		m_hMode );
	m_con = STD_INPUT_HANDLE;
	m_stdHandle = GetStdHandle( m_con );

	DWORD nRead = 0;
	std::string buff;
	buff.reserve( bytesToAllocate );
	auto ret = ReadConsoleW( m_stdHandle,
		buff.data(),
		sizeof( buff ),
		&nRead,
		nullptr );
	if ( !ret )
	{
		printHresultErrorDescription( HRESULT_FROM_WIN32( GetLastError() ) );
	}
	return std::string( buff );
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

std::string KeyConsole::getConsoleModeStr() const noexcept
{
	std::string strMode;
	switch ( m_con )
	{
	case STD_OUTPUT_HANDLE:
		strMode = "out";
		break;
	case STD_ERROR_HANDLE:
		strMode = "err";
		break;
	case STD_INPUT_HANDLE:
		strMode = "in";
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

void KeyConsole::setFont( const std::string& fontName )
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
		s2ws( fontName ).data() );
	SetCurrentConsoleFontEx( m_stdHandle,
		false,
		&cfie );

	getConsoleInfo( m_stdHandle );
}

int32_t KeyConsole::setCurcorPos( _COORD xy /* = { 0,0 } */ )
{
	return SetConsoleCursorPosition( m_stdHandle,
		xy );
}

bool KeyConsole::closeConsole()
{
	if ( !FreeConsole() )
	{
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
	if ( ms_instance == nullptr )
	{
		ms_instance = new KeyConsole;
	}
	return *ms_instance;
}

void KeyConsole::resetInstance()
{
	if ( ms_instance != nullptr )
	{
		delete ms_instance;
	}
}
