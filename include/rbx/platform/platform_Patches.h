#ifndef RBX_PLATFORM_PLATFORM_PATCHES_H_
#define RBX_PLATFORM_PLATFORM_PATCHES_H_

#include <Windows.h>

namespace rbx { namespace platform {

	/**
	 * @brief Patches the platform to Linux. (only ran if detected as running via Wine)
	 *
	 * @param[in] hProcess Handle to the DLL module.
	 */
	void patch_platform(HANDLE hProcess);

	/**
	 * @brief Patches the user agent to "Roblox/Linux". (only ran if detected as running via Wine)
	 *
	 * @param[in] hProcess Handle to the DLL module.
	 */
	void patch_user_agent(HANDLE hProcess);

}}	// namespace rbx::platform

#endif	// !RBX_PLATFORM_PLATFORM_PATCHES_H_