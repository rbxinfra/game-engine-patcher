#include <rbx/addresses.h>
#include <rbx/logging.h>
#include <rbx/platform/platform_Patches.h>

namespace {
#ifdef RBX_RCC
constexpr const std::uintptr_t ADDRESS_PLATFORM			 = 0x01DAB850;	// RCC v0.463.0.417004
constexpr const std::uintptr_t ADDRESS_PUSH_USER_AGENT_1 = 0x00472292;
constexpr const std::uintptr_t ADDRESS_PUSH_USER_AGENT_2 = 0x00472442;
constexpr const std::uintptr_t ADDRESS_PUSH_USER_AGENT_3 = 0x0054B140;
constexpr const std::uintptr_t ADDRESS_PUSH_USER_AGENT_4 = 0x0054B157;
#else
constexpr const std::uintptr_t ADDRESS_PLATFORM = 0x0253B744;  // Roblox Client v0.463.0.417004
#endif

#ifdef RBX_RCC
constexpr const char* const		kUserAgent			= "Roblox/Linux";
rbx::addresses::addresses_set_t kAddressesUserAgent = {
	ADDRESS_PUSH_USER_AGENT_1,
	ADDRESS_PUSH_USER_AGENT_2,
	ADDRESS_PUSH_USER_AGENT_3,
	ADDRESS_PUSH_USER_AGENT_4,
};
#endif

constexpr const DWORD kPlatform = 0xD;
}  // namespace

DYNAMIC_LOGGROUP(RccPatcher);

namespace rbx { namespace platform {

	void patch_platform(HANDLE hProcess)
	{
		FASTLOGFMT(DFLog::RccPatcher, "Writing platform %d to address %p", kPlatform, ASLR_ADDR(ADDRESS_PLATFORM));

		rbx::addresses::write_to_address(hProcess, ADDRESS_PLATFORM, &kPlatform, sizeof(kPlatform));
	}

	void patch_user_agent(HANDLE hProcess)
	{
#ifdef RBX_RCC
		FASTLOGFMT(DFLog::RccPatcher, "Writing user agent %s to address set", kUserAgent);

		rbx::addresses::overwrite_pushes(hProcess, kAddressesUserAgent, kUserAgent);
#else
		FASTLOG(DFLog::RccPatcher, "Skipping user agent patching, not running RCC");
#endif
	}

}}	// namespace rbx::platform