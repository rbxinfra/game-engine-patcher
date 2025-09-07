#include <rbx/logging.h>
#include <rbx/addresses/addresses_Helper.h>

#include <windows.h>

DYNAMIC_LOGVARIABLE(RccPatcherVerbose, 0);

namespace rbx { namespace addresses {

void
overwrite_push(HANDLE hProcess, std::uintptr_t address, const char* value)
{
    FASTLOGFMT(DFLog::RccPatcherVerbose, "Overwriting address %p with address %p", ASLR_ADDR(address), &value);

    write_to_address(hProcess, address + 1, &value, sizeof(value));
}

void
overwrite_pushes(HANDLE hProcess, addresses_set_t& addresses, const char* value)
{
    FASTLOGFMT(DFLog::RccPatcherVerbose, "Overwriting %d addresses with address %p", addresses.size(), &value);

    for (const auto& address : addresses)
        overwrite_push(hProcess, address, value);
}

void
write_to_address(HANDLE hProcess, std::uintptr_t address, const void* value, std::size_t size)
{
    void* pAslrAddr = ASLR_ADDR(address);

    FASTLOGFMT(DFLog::RccPatcherVerbose, "Writing %d bytes to address %p", size, pAslrAddr);

    DWORD dwOldProtect;

    ::VirtualProtectEx(hProcess, pAslrAddr, size, PAGE_READWRITE, &dwOldProtect);
    ::WriteProcessMemory(hProcess, pAslrAddr, value, size, NULL);
    ::VirtualProtectEx(hProcess, pAslrAddr, size, dwOldProtect, &dwOldProtect);
}

std::uintptr_t
get_relative_address(std::uintptr_t base, std::uintptr_t address)
{
    std::uintptr_t baseAddr = reinterpret_cast<std::uintptr_t>(ASLR_ADDR(base));

    return address - baseAddr - 5;
}

void
overwrite_call(HANDLE hProcess, std::uintptr_t address, void* newAddress)
{
    FASTLOGFMT(DFLog::RccPatcherVerbose, "Overwriting address %p with address %p", ASLR_ADDR(address), newAddress);

    std::uintptr_t relativeAddress = get_relative_address(address, reinterpret_cast<std::uintptr_t>(newAddress));

    FASTLOGFMT(DFLog::RccPatcherVerbose, "Relative address: %p", relativeAddress);

    write_to_address(hProcess, address + 1, &relativeAddress, sizeof(relativeAddress));
}

void
overwrite_calls(HANDLE hProcess, addresses_set_t& addresses, void* newAddress)
{
    FASTLOGFMT(DFLog::RccPatcherVerbose, "Overwriting %d calls with address %p", addresses.size(), newAddress);

    for (const auto& address : addresses)
        overwrite_call(hProcess, address, newAddress);
}

}}	// namespace rbx::addresses
