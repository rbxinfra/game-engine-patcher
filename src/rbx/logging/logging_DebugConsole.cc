#ifdef WITH_LOGGING

#	include <rbx/logging.h>

#	include <Windows.h>

#ifndef RBX_RCC
DYNAMIC_FASTFLAGVARIABLE(EnableConsole, true);
#endif

namespace rbx { namespace logging {

	void spawn_console()
	{
#	ifndef RBX_RCC // Only valid for Roblox Client as RCC already has a console
		if (!DFFlag::EnableConsole)
			return;

		DWORD dwOld;
		void* freeConsole = reinterpret_cast<void*>(&FreeConsole);

		::VirtualProtect(freeConsole, 1, PAGE_EXECUTE_READWRITE, &dwOld);
		std::memset(freeConsole, 0xC3, 1);
		::VirtualProtect(freeConsole, 1, dwOld, &dwOld);

		::AllocConsole();

		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
		freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);

		HWND hWnd = ::GetConsoleWindow();
		if (hWnd)
			::ShowWindow(hWnd, SW_SHOW);
#	endif
	}

}}	// namespace rbx::logging

#endif	// WITH_LOGGING