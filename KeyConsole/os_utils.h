#include <comdef.h>
#include <string>

#define pass (void)0;

// error printing
std::wstring printHresultErrorDescription( HRESULT hres );

#define ASSERT_RETURN_HRES_IF_FAILED( hres ) if ( FAILED ( hres ) )\
	{\
		std::wcout << __LINE__\
			<< L" "\
			<< printHresultErrorDescription( hres )\
			<< std::endl;\
			__debugbreak();\
		return hres;\
	}

#define ASSERT_HRES_IF_FAILED_( hres ) if ( FAILED ( hres ) )\
	{\
		std::wcout << __LINE__\
			<< L" "\
			<< printHresultErrorDescription( hres )\
			<< std::endl;\
			__debugbreak();\
		std::exit( hres );\
	}

#define ASSERT_HRES_IF_FAILED if ( FAILED ( hres ) )\
	{\
		std::wcout << __LINE__\
			<< L" "\
			<< printHresultErrorDescription( hres )\
			<< std::endl;\
			__debugbreak();\
		std::exit( hres );\
	}

//===================================================
//	\function	getLastErrorAsString
//	\brief  Returns the last Win32 error, in string format. Returns an empty string if there is no error.
//	\date	2020/11/10 1:44
#ifdef _UNICODE
std::wstring getLastErrorAsStringW();
#else
std::string getLastErrorAsStringA();
#endif
int32_t fileExists( std::wstring path );
