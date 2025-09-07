#include <rbx/addresses.h>
#include <rbx/logging.h>
#include <rbx/platform.h>
#include <rbx/signature.h>

#include <windows.h>

DYNAMIC_LOGVARIABLE(RccPatcher, 0);
DYNAMIC_FASTFLAGVARIABLE(DisableSignatureChecks, false);

#ifdef RBX_RCC
DYNAMIC_FASTFLAGVARIABLE(DebugBindSoapToAllInterfaces, false);

namespace {
constexpr const std::uintptr_t ADDRESS_PUSH_LOCALHOST_SOAP_BIND = 0x0058E8C2;

constexpr const char* const kAnyAddress = "0.0.0.0";
}  // namespace
#endif

/**
 * @brief Entry point.
 * @return 0
 */
int
main()
{
	FASTLOG(DFLog::RccPatcher, "Begin RCC patcher");

	HANDLE hProcess = ::GetCurrentProcess();

	if (rbx::platform::is_running_via_wine())
	{
		FASTLOG(DFLog::RccPatcher, "Running via Wine, applying Linux patches");

		rbx::platform::patch_platform(hProcess);
		rbx::platform::patch_user_agent(hProcess);
	}

	if (DFFlag::DisableSignatureChecks)
		rbx::signature::disable_signature_check(hProcess);
	else
	{
		rbx::signature::update_public_keys_from_kms();	// Update our public keys from the key management service

		rbx::signature::update_public_keys(hProcess);
	}

#ifdef RBX_RCC
	if (DFFlag::DebugBindSoapToAllInterfaces)
	{
		FASTLOG(DFLog::RccPatcher, "Overwriting SOAP bind to all interfaces");

		rbx::addresses::overwrite_push(hProcess, ADDRESS_PUSH_LOCALHOST_SOAP_BIND, kAnyAddress);
	}
#endif

	FASTLOG(DFLog::RccPatcher, "End RCC patcher");

	return 0;
}