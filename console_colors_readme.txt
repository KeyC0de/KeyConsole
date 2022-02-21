Console colors
--------------
Printing colored console text mostly depends on the platform we are. The most common way to do this is by printing ANSI escape sequences. For a simple example, here's some python code from the blender build scripts:
class ConsoleColor:
	HEADER = '\033[95m'
	OKBLUE = '\033[94m'
	OKGREEN = '\033[92m'
	WARNING = '\033[93m'
	FAIL = '\033[91m'
	BOLD = '\033[1m'
	UNDERLINE = '\033[4m'
	ENDC = '\033[0m'

print(f"{ConsoleColor.WARNING}Warning: No active frommets remain. Continue?{ConsoleColor.ENDC}")

An alternative way uses the cross-platform utility colorama.
> pip install colorama

WARNING: On windows you have to first enable the ANSI escape sequences on console in order for any coloring to work. To do that with python run this first:
> import os
> os.system('color')

Windows NT4..8.1's COLOR command can only be used to set the text and background color for the entire console, other ANSI.SYS features require third party utilities in Windows NT4..8.1's CMD.EXE.
You can use CONEMU which has them enabled by default. ConEmu supports ANSI X3.64 with Xterm 256 color extension.

Alternatively to enable ANSI escape codes in C++
(Only works in Windows 10 build number 10586 and later)
you need to `SetConsoleMode` with:

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

Otherwise you won't see colors or get ANSI processing for that application.

#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif


int main()
{
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return GetLastError();
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return GetLastError();
	}

	// Try some Set Graphics Rendition (SGR) terminal escape sequences
	wprintf(L"\x1b[31mThis text has a red foreground using SGR.31.\r\n");
	wprintf(L"\x1b[1mThis text has a bright (bold) red foreground using SGR.1 to affect the previous color setting.\r\n");
	wprintf(L"\x1b[mThis text has returned to default colors using SGR.0 implicitly.\r\n");
	wprintf(L"\x1b[34;46mThis text shows the foreground and background change at the same time.\r\n");
	wprintf(L"\x1b[0mThis text has returned to default colors using SGR.0 explicitly.\r\n");
	wprintf(L"\x1b[31;32;33;34;35;36;101;102;103;104;105;106;107mThis text attempts to apply many colors in the same command. Note the colors are applied from left to right so only the right-most option of foreground cyan (SGR.36) and background bright white (SGR.107) is effective.\r\n");
	wprintf(L"\x1b[39mThis text has restored the foreground color only.\r\n");
	wprintf(L"\x1b[49mThis text has restored the background color only.\r\n");

	return 0;
}

Alternatively you can change that from the registry:
The registry key at HKEY_CURRENT_USER\Console\VirtualTerminalLevel sets the global default behavior for processing ANSI escape sequences. Create a DWORD key (if necessary) and set its value to 1 to globally enable (or 0 to disable) ANSI processing by default.
[HKEY_CURRENT_USER\Console]
"VirtualTerminalLevel"=dword:00000001

Alternatively in Pre Windows 10
use SetConsoleTextAttribute
ansi_colors msvs project.

You may enable/disable it at will with the command:
ANSI ON/OFF