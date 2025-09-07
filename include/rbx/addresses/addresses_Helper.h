#ifndef RBX_ADDRESSES_ADDRESSES_HELPER_H_
#define RBX_ADDRESSES_ADDRESSES_HELPER_H_

#include <set>

#include <Windows.h>

/**
 * @brief Macro to convert an address to an ASLR address based on the current module.
 * @param[in] addr The address to convert.
 */
#define ASLR_ADDR(addr) reinterpret_cast<void*>(addr - 0x400000 + reinterpret_cast<std::uintptr_t>(GetModuleHandleA(NULL)))

namespace rbx { namespace addresses {
	/**
	 * @brief Type for a set of addresses.
	 */
	typedef std::set<std::uintptr_t> addresses_set_t;

	/**
	 * @brief Overwrite the intermediate push instruction.
	 * 
	 * @param[in] hProcess The handle to the process.
	 * @param[in] address The address to overwrite.
	 * @param[in] value The value to push.
	 */
	void overwrite_push(HANDLE hProcess, std::uintptr_t address, const char* value);

	/**
	 * @brief Overwrite the intermediate push instructions.
	 * 
	 * @param[in] hProcess The handle to the process.
	 * @param[in] addresses The addresses to overwrite.
	 * @param[in] value The value to push.
	 */
	void overwrite_pushes(HANDLE hProcess, addresses_set_t& addresses, const char* value);

	/**
	 * @brief Writes to the specified address.
	 * 
	 * @param[in] hProcess The handle to the process.
	 * @param[in] address The address to write to.
	 * @param[in] value The value to write.
	 * @param[in] size The size of the value.
	 */
	void write_to_address(HANDLE hProcess, std::uintptr_t address, const void* value, std::size_t size);

	/**
	 * @brief Gets a relative address for near calls.
	 * 
	 * @param[in] base The base address.
	 * @param[in] address The address to calculate the relative address for.
	 * @return The relative address.
	 */
	std::uintptr_t get_relative_address(std::uintptr_t base, std::uintptr_t address);

	/**
	 * @brief Overwrites a call instruction.
	 * 
	 * @param[in] hProcess The handle to the process.
	 * @param[in] address The address to overwrite.
	 * @param[in] newAddress The new address to call.
	 */
	void overwrite_call(HANDLE hProcess, std::uintptr_t address, void* newAddress);

	/**
	 * @brief Overwrites a set of calls.
	 * 
	 * @param[in] hProcess The handle to the process.
	 * @param[in] addresses The addresses to overwrite.
	 * @param[in] newAddress The new address to call.
	 */
	void overwrite_calls(HANDLE hProcess, addresses_set_t& addresses, void* newAddress);

}}	// namespace rbx::addresses

#endif	// RBX_ADDRESSES_ADDRESSES_HELPER_H_