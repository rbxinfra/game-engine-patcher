#ifndef RBX_SIGNATURE_SIGNATURE_PATCHES_H_
#define RBX_SIGNATURE_SIGNATURE_PATCHES_H_

#include <Windows.h>

namespace rbx { namespace signature {

    /**
     * @brief Disables the signature check entirely.
     * @remark This will make RBX::verifySignatureBase64 immediately ret.
     * 
     * @param[in] hProcess Handle to the DLL module.
     */
    void disable_signature_check(HANDLE hProcess);

    /**
     * @brief Updates the kPublicKeys check within RBX::CryptHelper's constructor to point to our own kPublicKeys.
     * 
     * @param[in] hProcess Handle to the DLL module.
     */
    void update_public_keys(HANDLE hProcess);

    /**
     * @brief Updates our kPublicKeys with the keys from the key management service.
     */
    void update_public_keys_from_kms();

}}	// namespace rbx::signature

#endif	// !RBX_SIGNATURE_SIGNATURE_PATCHES_H_