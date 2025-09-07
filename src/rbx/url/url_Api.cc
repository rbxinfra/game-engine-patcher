#include <rbx/url/url_Api.h>

#include <rbx/logging.h>

#include <format>

#include <pugixml.hpp>

namespace {
const char* const kAppSettings = "AppSettings.xml";
const char* const kSettings	   = "Settings";
const char* const kBaseUrl	   = "BaseUrl";

std::string get_base_host()
{
    std::string base_url = rbx::url::get_base_url();
    if (base_url.empty())
        return "";

    size_t pos = base_url.find("://");
    if (pos == std::string::npos)
        return "";

    pos += 3;
    
    // Remove www. if it exists
    if (base_url.compare(pos, 4, "www.") == 0)
        pos += 4;

    size_t end = base_url.find('/', pos);
    if (end == std::string::npos)
        return base_url.substr(pos);

    return base_url.substr(pos, end - pos);
}
}  // namespace

namespace rbx { namespace url {

	std::string& get_base_url()
	{
		static std::string base_url;

		// Stored un Settings.BaseUrl in AppSettings.xml
		if (base_url.empty())
		{
			pugi::xml_document doc;
			if (doc.load_file(kAppSettings))
			{
				pugi::xml_node settings = doc.child(kSettings);
				if (settings)
				{
					pugi::xml_node base_url_node = settings.child(kBaseUrl);
					if (base_url_node)
						base_url = base_url_node.text().as_string();
				}
			}
		}

		return base_url;
	}

	std::string get_roblox_service_host(const char* service)
	{
		std::string base_host = get_base_host();
        if (base_host.empty())
            return "";

        // In the form of service.base_host
        return std::format("{}.{}", service, base_host);
	}

    std::string get_roblox_service_url(const char* service, const char* url, bool use_https)
    {
        std::string host = get_roblox_service_host(service);
        if (host.empty())
            return "";

        return std::format("{}://{}/{}", use_https ? "https" : "http", host, url);
    }

}}	// namespace rbx::url