#ifndef RBX_URL_URL_API_H_
#define RBX_URL_URL_API_H_

#include <string>

namespace rbx { namespace url {

    /**
     * @brief Gets the base url from AppSettings.xml, or cache if it's already been read.
     * @return The base url.
     */
    std::string& get_base_url();

    /**
     * @brief Gets the Roblox service host.
     * @param[in] service The service to get the host for.
     * @return The host.
     */
    std::string get_roblox_service_host(const char* service);

    /**
     * @brief Gets the Roblox service url.
     * @param[in] service The service to get the url for.
     * @param[in] url The url to append to the service.
     * @param[in] use_https Whether to use https.
     * @return The url.
     */
    std::string get_roblox_service_url(const char* service, const char* url = "", bool use_https = true);

}}	// namespace rbx::url

#endif	// RBX_URL_URL_API_H_