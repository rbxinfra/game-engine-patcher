#include <rbx/signature/signature_Patches.h>

#include <rbx/addresses.h>
#include <rbx/logging.h>

#include <rbx/http.h>
#include <rbx/url.h>

#include <nlohmann/json.hpp>

namespace {
// +4 bytes to the actual address reference: mov ebx, dword ptr ds:[eax * 4 + <kPublicKeys>]
#ifdef RBX_RCC
constexpr const std::uintptr_t ADDRESS_MOV_KPUBLICKEYS			 = 0x015586B8;
constexpr const std::uintptr_t ADDRESS_RBX_VERIFYSIGNATUREBASE64 = 0x01559710;
#else
constexpr const std::uintptr_t ADDRESS_MOV_KPUBLICKEYS			 = 0x00FDC7C8;
constexpr const std::uintptr_t ADDRESS_RBX_VERIFYSIGNATUREBASE64 = 0x00FDDAF0;
#endif

/* For now, 5 empty public keys set here as update_public_keys will fetch them from key-management-service */
static LPCSTR kPublicKeys[] = {"", "", "", "", ""};

constexpr const char* const kKmsService	 = "kms.api";
constexpr const char* const kKmsEndpoint = "v1/GetClientPublicKeys";

static bool s_CanUpdatePublicKeys = false;
}  // namespace

DYNAMIC_LOGGROUP(RccPatcher);

namespace rbx { namespace signature {

	void disable_signature_check(HANDLE hProcess)
	{
		// push ebp changes to ret (0x55 -> 0xC3)

		FASTLOG(DFLog::RccPatcher, "Disabling signature check");

		rbx::addresses::write_to_address(hProcess, ADDRESS_RBX_VERIFYSIGNATUREBASE64, "\xC3", 1);
	}

	void update_public_keys(HANDLE hProcess)
	{
		if (!s_CanUpdatePublicKeys)
		{
			FASTLOG(DFLog::RccPatcher, "Skipping public key update, either they were already updated or we failed to fetch them");

			return;
		}

		// mov ebx, dword ptr ds:[eax * 4 + <kPublicKeys>] (8B1C85 D8FB7202, skip past the 8B1C85)

		FASTLOGFMT(DFLog::RccPatcher, "Updating public keys to address %p", ASLR_ADDR(ADDRESS_MOV_KPUBLICKEYS));

		void*  pKPublicKeys	 = &kPublicKeys;
		void** ppKPublicKeys = &pKPublicKeys;

		rbx::addresses::write_to_address(hProcess, ADDRESS_MOV_KPUBLICKEYS + 3, ppKPublicKeys, sizeof(std::uintptr_t));

		s_CanUpdatePublicKeys = false;
	}

	void update_public_keys_from_kms()
	{
		static std::string kmsUrl = rbx::url::get_roblox_service_url(kKmsService, kKmsEndpoint);

		FASTLOGFMT(DFLog::RccPatcher, "Fetching public keys from %s", kmsUrl.c_str());

		// Fetch public keys from the key management service
		rbx::http::http_request request;
		request.url	   = kmsUrl;
		request.method = rbx::http::HTTP_GET;

		rbx::http::http_response response = rbx::http::http_api::instance()->request(request);
		if (response.status_code != 200)
		{
			FASTLOGFMT(DFLog::RccPatcher, "Failed to fetch public keys from %s", kmsUrl.c_str());

			s_CanUpdatePublicKeys = false;

			return;
		}

		nlohmann::json json = nlohmann::json::parse(response.body);

		auto data = json["data"];
		if (data.is_null())
		{
			FASTLOG(DFLog::RccPatcher, "Failed to parse public keys from response");

			s_CanUpdatePublicKeys = false;

			return;
		}

		char* pRbxsig2	= _strdup(data["rbxsig2"].get<std::string>().c_str());
		char* pGameJoin = _strdup(data["gameJoin"].get<std::string>().c_str());
		char* pRbxsig4	= _strdup(data["rbxsig4"].get<std::string>().c_str());

		kPublicKeys[2] = pRbxsig2;
		kPublicKeys[3] = pGameJoin;
		kPublicKeys[4] = pRbxsig4;

		FASTLOGFMT(DFLog::RccPatcher,
				   "Fetched public keys: rbxsig2=%p, gameJoin=%p, rbxsig4=%p",
				   kPublicKeys[2],
				   kPublicKeys[3],
				   kPublicKeys[4]);

		s_CanUpdatePublicKeys = true;
	}

}}	// namespace rbx::signature