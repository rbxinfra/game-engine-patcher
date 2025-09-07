#include <rbx/logging.h>

#ifdef WITH_LOGGING

#	include <cassert>
#	include <cctype>
#	include <cstdarg>
#	include <cstdio>
#	include <cstring>

#	include <algorithm>
#	include <atomic>
#	include <chrono>
#	include <functional>
#	include <sstream>
#	include <thread>
#	include <unordered_map>

#	define FLOAT_MARKER 0xFFFF10AD

#	define MAX_LOG_MESSAGE 260

LOGVARIABLE(FastLogValueChanged, 1);

#	include <rbx/macros.h>

using namespace std::placeholders;

namespace {

using namespace rbx::logging;

void
_default_log_func(channel_t channel, const char* message)
{
	using namespace std::chrono;

	static auto start = system_clock::now();

	// Message format:
	// iso-8601 timestamp, time since start, thread id, channel, message

	// Get the current time as an ISO-8601 string
	auto   now		  = system_clock::now();
	time_t now_time_t = system_clock::to_time_t(now);
	tm*	   now_tm	  = std::localtime(&now_time_t);
	char   now_str[64];

	std::strftime(now_str, sizeof(now_str), "%Y-%m-%dT%H:%M:%SZ", now_tm);

	// Get the time since the start of the program in seconds fixed to 6 decimal places
	auto now_s = duration_cast<duration<double>>(now - start).count();

	// Get the thread id, formatted as hex and 4 characters wide
	auto			  thread_id = std::this_thread::get_id();
	std::stringstream thread_id_ss;
	thread_id_ss << std::hex << thread_id;

	auto thread_id_str = thread_id_ss.str();
	thread_id_str.resize(4, '0');

	// Print the message
	std::printf("%s,%lf,%s,%d %s\n", now_str, now_s, thread_id_str.c_str(), channel + LOGCHANNELS, message);
}

template<class T>
T _convert_from_string(const std::string& valueData, std::string name);

template<>
int
_convert_from_string<int>(const std::string& valueData, std::string name)
{
	UNUSED(name);  // unused

	return atoi(valueData.c_str());
}
template<>
bool
_convert_from_string<bool>(const std::string& valueData, std::string name)
{
	UNUSED(name);  // unused

	std::string lowerValueData = valueData;
	std::transform(lowerValueData.begin(), lowerValueData.end(), lowerValueData.begin(), ::tolower);

	return lowerValueData == "true" ? true : false;
}
template<>
std::string
_convert_from_string<std::string>(const std::string& valueData, std::string name)
{
	UNUSED(name);  // unused

	return valueData;
}
template<>
channel_t
_convert_from_string<unsigned char>(const std::string& valueData, std::string name)
{
	UNUSED(name);  // unused

	return static_cast<unsigned char>(atoi(valueData.c_str()));
}
template<>
FFlag::Value
_convert_from_string<FFlag::Value>(const std::string& valueData, std::string name)
{
	std::string lowerValueData = valueData;
	std::transform(lowerValueData.begin(), lowerValueData.end(), lowerValueData.begin(), ::tolower);

	return FFlag::Value(lowerValueData == "true" ? true : false, name.c_str());
}

std::string
_convert_to_string(bool a)
{
	return a ? "true" : "false";
}

std::string
_convert_to_string(int a)
{
	return std::to_string(a);
}
std::string
_convert_to_string(std::string a)
{
	return a;
}

struct value_get_set_base
{
public:
	virtual void		  set(std::string name, const std::string& valueData, fast_var_type fastVarType) noexcept = 0;
	virtual std::string	  get() const noexcept																	  = 0;
	virtual fast_var_type get_type() const noexcept																  = 0;
};

template<class T>
class value_get_set : public value_get_set_base
{
	T*			  m_Value;
	fast_var_type m_Type;

public:
	value_get_set(T* value, fast_var_type _fastVarType) : m_Value(value), m_Type(_fastVarType) {}

	void set(std::string name, const std::string& valueData, fast_var_type fastVarType) noexcept
	{
		if (fastVarType != FASTVARTYPE_ANY)
		{
			if (m_Type != fastVarType)
				return;
		}

		*m_Value = _convert_from_string<T>(valueData, name);
	}

	std::string	  get() const noexcept { return _convert_to_string(*m_Value); }
	fast_var_type get_type() const noexcept { return m_Type; }
};

typedef std::unordered_map<std::string, value_get_set_base*> variables_t;
typedef std::unordered_map<std::string, std::string>		 unknown_variables_t;

static variables_t*			s_Variables;
static unknown_variables_t* s_UnknownVariables;
static custom_log_func_t	s_CustomLogFunc = ::_default_log_func;

#	if defined(__aarch64__)
typedef double float64_t;
#	else
typedef float float64_t;
#	endif

}  // namespace

namespace rbx { namespace logging {

	void fast_log(channel_t channel, const char* message)
	{
		if (channel > LOGCHANNELS)
		{
			if (s_CustomLogFunc)
				s_CustomLogFunc(channel - LOGCHANNELS, message);
		}
	}

	void fast_log_formatted(channel_t channel, const char* format, ...)
	{
		va_list pArgList;
		va_start(pArgList, format);
		char temp[MAX_LOG_MESSAGE];
#	ifdef WIN32
		::vsnprintf_s(temp, MAX_LOG_MESSAGE, format, pArgList);
#	else
		std::vsnprintf(temp, MAX_LOG_MESSAGE, format, pArgList);
#	endif
		va_end(pArgList);

		if (channel > LOGCHANNELS)
			if (s_CustomLogFunc)
				s_CustomLogFunc(channel - LOGCHANNELS, temp);
	}

	void set_custom_log_func(custom_log_func_t lF)
	{
		s_CustomLogFunc = lF;
	}

	template<class T>
	void register_variable(const char* varName, T* value, fast_var_type fastVarType)
	{
		if (s_Variables == NULL)
			s_Variables = new variables_t();

		if (s_UnknownVariables == NULL)
			s_UnknownVariables = new unknown_variables_t();

		std::string name = varName;

		assert(s_Variables->find(name) == s_Variables->end());

		auto varValue = new value_get_set<T>(value, fastVarType);
		s_Variables->insert(variables_t::value_type(name, varValue));

		auto itUnknown = s_UnknownVariables->find(varName);
		if (itUnknown != s_UnknownVariables->end())
		{
			auto itVars = s_Variables->find(name);
			itVars->second->set(name, itUnknown->second, fastVarType);

			s_UnknownVariables->erase(itUnknown);
		}
	}

	int register_log_group(const char* groupName, channel_t* groupVar, fast_var_type fastVarType)
	{
		register_variable(groupName, groupVar, fastVarType);
		return 1;
	}

	int register_string(const char* groupName, std::string* groupVar, fast_var_type fastVarType)
	{
		register_variable(groupName, groupVar, fastVarType);
		return 1;
	}

	int register_int(const char* groupName, int* groupVar, fast_var_type fastVarType)
	{
		register_variable(groupName, groupVar, fastVarType);
		return 1;
	}

	int register_flag(const char* flagName, bool* flagVar, fast_var_type fastVarType)
	{
		register_variable(flagName, flagVar, fastVarType);
		return 1;
	}

	int register_flag(const char* flagName, FFlag::Value* flagVar, fast_var_type fastVarType)
	{
		register_variable(flagName, flagVar, fastVarType);
		return 1;
	}

	static void visit_variable(variables_t::value_type pair, variable_visitor_t visitor, void* context)
	{
		visitor(pair.first.c_str(), pair.second->get(), context);
	}

	void for_each_variable(variable_visitor_t visitor, void* context, fast_var_type flagType)
	{
		if (flagType == FASTVARTYPE_ANY)
		{
			std::for_each(s_Variables->begin(), s_Variables->end(), std::bind(visit_variable, _1, visitor, context));
		}
		else
		{
			for (auto iter = s_Variables->begin(); iter != s_Variables->end(); iter++)
			{
				if ((iter->second->get_type() & flagType) > 0)
				{
					visitor(iter->first.c_str(), iter->second->get(), context);
				}
			}
		}
	}

	bool set_value(const std::string& name, const std::string& value, fast_var_type fastVarType)
	{
		if (!s_Variables)
			return false;

		if (!s_UnknownVariables)
			s_UnknownVariables = new unknown_variables_t();

		auto nameCopy = name;

		// If name starts with FXXX::, remove it and determine fastVarType from the FXXX
		if (name.find("::") != std::string::npos)
		{
			nameCopy = name.substr(name.find("::") + 2);

			// If it starts with DF it's a dynamic flag else it's a static flag
			if (name.find("DF") == 0)
				fastVarType = FASTVARTYPE_DYNAMIC;
			else
				fastVarType = FASTVARTYPE_STATIC;
		}

		bool result = false;

		auto it = s_Variables->find(nameCopy);
		if (it != s_Variables->end())
		{
			it->second->set(nameCopy, value, fastVarType);
			result = true;
		}
		else
		{
			(*s_UnknownVariables)[nameCopy] = value;
		}

		FASTLOGFMT(FLog::FastLogValueChanged, "Setting variable %s", name.c_str());
		FASTLOGFMT(FLog::FastLogValueChanged, "...to value %s", value.c_str());
		return result;
	}

	bool get_value(const std::string& name, std::string& value, bool alsoCheckUnknown)
	{
		auto nameCopy = name;

		// Ensure that we don't have FXXX:: parts in the name
		if (name.find("::") != std::string::npos)
			nameCopy = name.substr(name.find("::") + 2);

		auto it = s_Variables->find(nameCopy);
		if (it != s_Variables->end())
		{
			value = it->second->get();
			return true;
		}
		else
		{
			if (alsoCheckUnknown)
			{
				auto unknownIt = s_UnknownVariables->find(nameCopy);
				if (unknownIt != s_UnknownVariables->end())
				{
					value = unknownIt->second;
					return true;
				}
			}
			return false;
		}
	}
}}	// namespace rbx::logging

#endif