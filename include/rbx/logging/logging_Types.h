#ifndef RBX_LOGGING_LOGGING_TYPES_H_
#define RBX_LOGGING_LOGGING_TYPES_H_

#ifdef WITH_LOGGING

#	include <string>
#	include <cstdint>

namespace FFlag {

/**
 * @brief Represents a flag variable
 */
struct Value
{
	bool		m_Value;
	const char* m_Name;

	/**
	 * @brief Constructs a new Value
	 *
	 * @param[in] value The value of the variable
	 * @param[in] name The name of the variable
	 */
	Value(bool value = false, const char* name = "") : m_Value(value), m_Name(name) {}

	/**
	 * @brief Sets the value of the variable
	 *
	 * @param[in] value The value to set
	 * @return Value&
	 */
	inline Value& operator=(bool value)
	{
		this->m_Value = value;

		return *this;
	}

	/**
	 * @brief Converts the variable to a bool
	 *
	 * @return bool
	 */
	inline operator bool() const { return this->m_Value; }
};

}  // namespace FFlag

namespace rbx { namespace logging {

	/**
	 * @brief Type of a fast variable
	 */
	enum fast_var_type
	{
		/**
		 * @brief Static variable
		 */
		FASTVARTYPE_STATIC = 0,

		/**
		 * @brief Dynamic variable
		 */
		FASTVARTYPE_DYNAMIC = 1,

		/**
		 * @brief Any variable
		 */
		FASTVARTYPE_ANY = FASTVARTYPE_DYNAMIC
	};

	/**
	 * @brief Type of a fast variable value
	 */
	enum fast_var_value_type
	{
		/**
		 * @brief Invalid value
		 */
		FASTVARVALUETYPE_INVALID,

		/**
		 * @brief Log value
		 */
		FASTVARVALUETYPE_LOG,

		/**
		 * @brief String value
		 */
		FASTVARVALUETYPE_STRING,

		/**
		 * @brief Integer value
		 */
		FASTVARVALUETYPE_INT,

		/**
		 * @brief Flag value
		 */
		FASTVARVALUETYPE_FLAG,
	};

	/**
	 * @brief Represents a log channel
	 */
	typedef std::uint8_t channel_t;

	/**
	 * @brief Type for a function executed when a log is written
	 * @param channel The channel the log was written to
	 * @param message The message that was written
	 */
	typedef void (*custom_log_func_t)(channel_t channel, const char* message);

	/**
	 * @brief Type for a function executed when a variable is visited
	 * @param name The name of the variable
	 * @param value The value of the variable
	 * @param context The context passed to for_each_variable
	 */
	typedef void (*variable_visitor_t)(const char* const name, const std::string& value, void* context);

}}	// namespace rbx::logging

#else

namespace rbx { namespace log {
	typedef unsigned char channel_t;
}}	// namespace rbx::log

#endif	// WITH_LOGGING

#endif	// !RBX_LOGGING_LOGGING_TYPES_H_
