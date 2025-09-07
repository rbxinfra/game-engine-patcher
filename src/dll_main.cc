#include <rbx/logging.h>
#include <rbx/version_proxy.h>
#include "main.h"

#include <rbx/macros.h>

#include <windows.h>
#include <mutex>

LOGVARIABLE(EntryPoint, 0);

#ifdef STANDALONE
#	define RBX_APIENTRY __declspec(dllexport) BOOL APIENTRY
#else
#	define RBX_APIENTRY BOOL APIENTRY
#endif

/**
 * @brief DllMain entry point.
 * @param hModule Handle to the DLL module.
 * @param ul_reason_for_call Reason for calling the DLL.
 * @param lpReserved Reserved parameter.
 * @return True if the function succeeded, false otherwise.
 */
RBX_APIENTRY
DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID)
{
	static std::once_flag initFlag;
#ifndef STANDALONE
	static std::once_flag cleanupFlag;
#endif

#ifdef WITH_LOGGING
	static std::once_flag settingsFlag;

	std::call_once(settingsFlag, [&]() {
		rbx::logging::for_each_variable(
			[](const char* const name, const std::string& value, void* context) {
				UNUSED(value);
				UNUSED(context);

				if (const char* env = std::getenv(name))
					rbx::logging::set_value(name, env);
			},
			NULL,
			rbx::logging::FASTVARTYPE_ANY);

		rbx::logging::spawn_console();
	});
#endif

	FASTLOGFMT(FLog::EntryPoint, "DllMain called with reason %d, hModule %p", ul_reason_for_call, hModule);

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			std::call_once(initFlag, []() {
				FASTLOGFMT(FLog::EntryPoint,
						   "Attached to process %d, running main thread for function %p and initializing proxy",
						   GetCurrentProcessId(),
						   main);

				CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(main), NULL, NULL, NULL);

#ifndef STANDALONE
				rbx::proxy::load_version_library();
#endif
			});

			break;

		case DLL_PROCESS_DETACH:
#ifndef STANDALONE
			std::call_once(cleanupFlag, []() {
				FASTLOGFMT(FLog::EntryPoint, "Detached from process %d, unloading actual library", GetCurrentProcessId());

				rbx::proxy::unload_version_library();
			});
#endif
			break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		default:
			break;
	}

	return TRUE;
}