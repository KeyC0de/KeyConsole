#include <comdef.h>
#include <string>


// error printing
std::string printHresultErrorDescription( HRESULT hres );
std::wstring printHresultErrorDescriptionW( HRESULT hres );

#define ASSERT_RETURN_HRES_IF_FAILED( hres ) if ( FAILED ( hres ) )\
	{\
		std::cout << __LINE__\
			<< " "\
			<< printHresultErrorDescription( hres )\
			<< std::endl;\
			__debugbreak();\
		return hres;\
	}

#define ASSERT_HRES_IF_FAILED_( hres ) if ( FAILED ( hres ) )\
	{\
		std::cout << __LINE__\
			<< " "\
			<< printHresultErrorDescription( hres )\
			<< std::endl;\
			__debugbreak();\
		std::exit( hres );\
	}

#define ASSERT_HRES_IF_FAILED if ( FAILED ( hres ) )\
	{\
		std::cout << __LINE__\
			<< " "\
			<< printHresultErrorDescription( hres )\
			<< std::endl;\
			__debugbreak();\
		std::exit( hres );\
	}

//===================================================
//	\function	getLastErrorAsString
//	\brief  Returns the last Win32 error, in string format.
//			Returns an empty string if there is no error.
//	\date	2020/11/10 1:44
std::string getLastErrorAsString();