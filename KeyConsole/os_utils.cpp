#include "os_utils.h"
#include "utils.h"


std::string printHresultErrorDescription( HRESULT hres )
{
	_com_error error( hres );
	return ws2s( error.ErrorMessage() );
}

std::wstring printHresultErrorDescriptionW( HRESULT hres )
{
	_com_error error{hres};
	return error.ErrorMessage();
}

std::string getLastErrorAsString()
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

	std::string message{buff, size};
	LocalFree( buff );
	return message;
}

int32_t fileExists( std::string path )
{
	uint32_t attribs = GetFileAttributesW( s2ws( path ).data() );
	return ( attribs != INVALID_FILE_ATTRIBUTES
		&& !( attribs & FILE_ATTRIBUTE_DIRECTORY ) );
}
