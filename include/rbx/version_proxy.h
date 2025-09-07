#ifndef RBX_VERSION_PROXY_H_
#define RBX_VERSION_PROXY_H_

#ifndef STANDALONE

namespace rbx { namespace proxy {
	/**
	 * @brief Load the proxy library.
	 */
	void load_version_library();

	/**
	 * @brief Unload the proxy library.
	 */
	void unload_version_library();
}}	// namespace rbx::proxy

#endif	// !STANDALONE

#endif	// !RBX_VERSION_PROXY_H_