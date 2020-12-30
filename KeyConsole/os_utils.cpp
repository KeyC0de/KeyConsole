#include "os_utils.h"

std::wstring printHresultErrorDescription( HRESULT hres )
{
	_com_error error( hres );
	return error.ErrorMessage();
}

#ifdef _UNICODE
std::wstring getLastErrorAsStringW()
{
	//Get the error message, if any.
	DWORD errorMsgId = ::GetLastError();
	if ( errorMsgId == 0 )
	{
		return std::wstring{L""}; // no recorded error messages
	}

	wchar_t buff[256];
	size_t size = FormatMessageW( FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorMsgId,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		buff,
		( sizeof( buff ) / sizeof( wchar_t ) ),
		nullptr );

	std::wstring message( buff,
		size );
	LocalFree( buff );	// Free the buffer.
	return message;
}
#else
std::string getLastErrorAsStringA()
{
	//Get the error message, if any.
	DWORD errorMsgId = ::GetLastError();
	if ( errorMsgId == 0 )
	{
		return std::string{""};
	}

	LPSTR buff = nullptr;
	size_t size = FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorMsgId,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(LPSTR)&buff,
		0,
		nullptr );

	std::string message( buff,
		size );
	LocalFree( buff) ;
	return message;
}
#endif

int32_t fileExists( std::wstring path )
{
	uint32_t attribs = GetFileAttributesW( path.c_str() );
	return ( attribs != INVALID_FILE_ATTRIBUTES
		&& !( attribs & FILE_ATTRIBUTE_DIRECTORY ) );
}
