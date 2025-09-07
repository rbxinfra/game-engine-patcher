#ifndef RBX_PLATFORM_PLATFORM_WINE_H_
#define RBX_PLATFORM_PLATFORM_WINE_H_

namespace rbx { namespace platform {

    /**
     * @brief Detects if the current platform is Wine.
     * 
     * @return True if the current platform is Wine, false otherwise.
     */
    bool is_running_via_wine();

}}	// namespace rbx::platform

#endif	// !RBX_PLATFORM_PLATFORM_WINE_H_