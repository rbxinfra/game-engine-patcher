#ifndef RBX_HTTP_HTTP_API_H_
#define RBX_HTTP_HTTP_API_H_

#include <memory>
#include <string>

#include <curl/curl.h>

#include <rbx/http/http_Types.h>

namespace rbx { namespace http {

    // Singleton non-copyable class for managing the HTTP API.
	class http_api {
    private:
        CURL* curl_;

    public:
        static std::shared_ptr<http_api> instance();

        http_response request(const http_request& request);

        http_api();
        ~http_api();

        // non-copyable
        http_api(const http_api&) = delete;
        http_api& operator=(const http_api&) = delete;

    private:
        static size_t _write_callback(void* contents, size_t size, size_t nmemb, std::string* buffer);
    };

}}	// namespace rbx::http

#endif	// RBX_HTTP_HTTP_API_H_