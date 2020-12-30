#pragma once

#include "winner.h"
#include <string>

//============================================================
//	\class	KeyConsole
//
//	\author	KeyC0de
//	\date	2020/07/10 15:10
//
//	\brief	interface to the process's console
//			Singleton class
//			by default set to output mode
//			the mode is changed dynamically depending on print/log/read operation request
//=============================================================
class KeyConsole
{
	static inline constexpr const wchar_t* currentVersion = L"v0.1";
	static inline constexpr const wchar_t* defaultConsoleTitle =
		L"KeyEngine Debug Console - ";

	FILE* m_fp;
	std::wstring m_title;
	DWORD m_con;			// HANDLE
	FILE* m_hMode;			// set this when you print/log/read to stdout/stderr/stdin
	HANDLE m_stdHandle;		// the handle to the console
	static inline KeyConsole* m_instance;
private:
	KeyConsole( const std::wstring& fontName = L"Lucida Console" );
public:
	KeyConsole( const KeyConsole& rhs ) = delete;
	KeyConsole& operator=( const KeyConsole& rhs ) = delete;
	~KeyConsole();

	static DWORD getFontFamily( HANDLE h );
	static void getConsoleInfo( HANDLE h );
public:
	bool closeConsole();

	static KeyConsole& getInstance() noexcept;
	//===================================================
	//	\function	~KeyConsole
	//	\brief  you must call this prior to program termination to help the leaker checker
	//	\date	2020/12/30 22:19
	static void resetInstance();
		
	int getConsoleMode() const noexcept;
	std::wstring getConsoleModeStr() const noexcept;
	uint32_t getConsoleCodePage() const noexcept;
	HANDLE getStdHandle() const noexcept;

	int32_t setConsoleCodePage( uint32_t cp );
	void setFont( const std::wstring& fontName );
	int32_t setCurcorPos( _COORD xy = { 0,0 } );

	//===================================================
	//	\function	print
	//	\brief  print to stdout
	//	\date	2020/12/01 21:36
	DWORD print( const std::wstring& msg );
	//===================================================
	//	\function	log
	//	\brief  print to stderr
	//	\date	2020/12/01 21:36
	DWORD log( const std::wstring& msg );
	//===================================================
	//	\function	read
	//	\brief  read from stdin, returns the string
	//	\date	2020/12/01 21:36
	std::wstring read( const uint32_t bytesToAllocate = 128u );

	inline bool operator==( const KeyConsole& rhs ) const noexcept
	{
		return this->m_fp == rhs.m_fp;
	}
	inline bool operator!=( const KeyConsole& rhs ) const noexcept
	{
		return this->m_fp != rhs.m_fp;
	}
	inline bool operator==( const KeyConsole* rhs ) const noexcept
	{
		return this->m_fp == rhs->m_fp;
	}
	inline bool operator!=( const KeyConsole* rhs ) const noexcept
	{
		return this->m_fp != rhs->m_fp;
	}
};