#include <rbx/http/http_Api.h>
#include <rbx/logging.h>

#include <filesystem>
#include <stdexcept>

#include <curl/curl.h>

DYNAMIC_LOGVARIABLE(HttpApi, 0);

namespace rbx { namespace http {

	http_api::http_api() : curl_(curl_easy_init())
	{
		if (curl_ == NULL)
		{
			throw std::runtime_error("Failed to initialize cURL");
		}

		FASTLOGFMT(DFLog::HttpApi, "cURL version: %s", curl_version());
	}

	http_api::~http_api()
	{
		FASTLOGFMT(DFLog::HttpApi, "Cleaning up cURL");

		curl_easy_cleanup(curl_);
	}

	std::shared_ptr<http_api> http_api::instance()
	{
		static std::shared_ptr<http_api> instance = std::make_shared<http_api>();
		return instance;
	}

	size_t http_api::_write_callback(void* contents, size_t size, size_t nmemb, std::string* buffer)
	{
		size_t realsize = size * nmemb;
		buffer->append(static_cast<char*>(contents), realsize);
		return realsize;
	}

	http_response http_api::request(const http_request& request)
	{
		FASTLOGFMT(DFLog::HttpApi, "Requesting %s", request.url.c_str());

		std::string buffer;
		curl_easy_setopt(curl_, CURLOPT_URL, request.url.c_str());
		curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, _write_callback);
		curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &buffer);
		curl_easy_setopt(curl_, CURLOPT_NOSIGNAL, 1L);

		FASTLOGFMT(DFLog::HttpApi, "Request method: %d", request.method);
		FASTLOGFMT(DFLog::HttpApi, "Request body: %s", request.body.c_str());

		curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, request.method == HTTP_GET ? "GET" : "POST");

		// If we have a body, set it.
		if (!request.body.empty())
		{
			curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, request.body.c_str());
			curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, request.body.size());
		}

		FASTLOGFMT(DFLog::HttpApi, "Request headers: %d", request.headers.size());

		// If we have headers, set them.
		if (!request.headers.empty())
		{
			struct curl_slist* headers = NULL;
			for (const auto& header : request.headers)
			{
				FASTLOGFMT(DFLog::HttpApi, "Header: %s: %s", header.name.c_str(), header.value.c_str());

				headers = curl_slist_append(headers, (header.name + ": " + header.value).c_str());
			}

			curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
		}

		// Update CA cert bundle path.
		curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, FALSE);

		// Perform the request.
		CURLcode res = curl_easy_perform(curl_);

		FASTLOGFMT(DFLog::HttpApi, "Request performed, result: %d", res);

		// Clean up headers.
		if (!request.headers.empty())
			curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, NULL);

		// Check for errors.
		if (res != CURLE_OK)
		{
			FASTLOGFMT(DFLog::HttpApi, "Failed to perform request: %s", curl_easy_strerror(res));

			return {0, ""};
		}

		// Get the response code.
		long response_code;
		curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);

		FASTLOGFMT(DFLog::HttpApi, "Response code: %ld", response_code);

		return {response_code, buffer};
	}

}}	// namespace rbx::http