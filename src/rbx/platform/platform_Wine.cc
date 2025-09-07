#include <rbx/platform/platform_Wine.h>

#include <Windows.h>

namespace {
constexpr const wchar_t* kNtdllDll			  = L"ntdll.dll";
constexpr const char*	 kNtdllWineGetVersion = "wine_get_version";
}  // namespace

namespace rbx { namespace platform {

	bool is_running_via_wine()
	{
		HMODULE hNTDLL = ::GetModuleHandleW(kNtdllDll);
		if (hNTDLL == NULL)
			return false;

		return ::GetProcAddress(hNTDLL, kNtdllWineGetVersion) != NULL;
	}

}}	// namespace rbx::platform