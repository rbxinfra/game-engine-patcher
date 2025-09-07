#ifndef RBX_LOGGING_LOGGING_API_H_
#define RBX_LOGGING_LOGGING_API_H_

#ifdef WITH_LOGGING

#include <rbx/logging/logging_Types.h>
#include <rbx/logging/logging_Macros.h>

namespace rbx { namespace logging {

	// Mini guide:

	// ---------------------------------
	// Using logs
	// ---------------------------------
	// Use FASTLOG macros _ONLY_

	// Examples:

	// Declare a group, then log using this group:
	// LOGGROUP(Test);
	// Important(!): Add group to v8dataModel/FastLogSettings.cpp

	// ---------------------------------
	// Looking at the logs
	// ---------------------------------
	// Helpful debugger entries - select and mouse over:
	// channel_t 1 -			{,,Log}g_FastLog[0]+({,,Log}g_LogCounters[0]%2048)
	// channel_t 2 -			{,,Log}g_FastLog[1]+({,,Log}g_LogCounters[1]%2048)
	// channel_t 3 -			{,,Log}g_FastLog[2]+({,,Log}g_LogCounters[2]%2048)
	// channel_t 4 -			{,,Log}g_FastLog[3]+({,,Log}g_LogCounters[3]%2048)
	// channel_t 5 -			{,,Log}g_FastLog[4]+({,,Log}g_LogCounters[4]%2048)

	/**
	 * @brief Sets the function to call when a log is written
	 * @param lF The function to call when a log is written
	 */
	void set_custom_log_func(custom_log_func_t lF);

	/**
	 * @brief Writes a log to the specified channel
	 * @param channel The channel to write to
	 * @param message The message to write
	 */
	void fast_log(channel_t channel, const char* message);

	/**
	 * @brief Writes a formatted log to the specified channel
	 * @param channel The channel to write to
	 * @param message The message to write
	 */
	void fast_log_formatted(channel_t channel, const char* message, ...);

	/**
	 * @brief Registers a log group
	 * @param varName The name of the variable
	 * @param value The value of the variable
	 * @param fastVarType The type of the variable
	 */
	int register_log_group(const char* groupName, channel_t* groupVar, fast_var_type fastVarType = FASTVARTYPE_STATIC);

	/**
	 * @brief Registers a flag variable
	 * @param varName The name of the variable
	 * @param value The value of the variable
	 * @param fastVarType The type of the variable
	 */
	int register_flag(const char* flagName, bool* flagVar, fast_var_type fastVarType = FASTVARTYPE_STATIC);

	/**
	 * @brief Registers a flag variable but for the type FFlag::Value
	 * @param varName The name of the variable
	 * @param value The value of the variable
	 * @param fastVarType The type of the variable
	 */
	int register_flag(const char* flagName, FFlag::Value* flagVar, fast_var_type fastVarType = FASTVARTYPE_STATIC);

	/**
	 * @brief Registers an integer variable
	 * @param varName The name of the variable
	 * @param value The value of the variable
	 * @param fastVarType The type of the variable
	 */
	int register_int(const char* flagName, int* flagVar, fast_var_type fastVarType = FASTVARTYPE_STATIC);

	/**
	 * @brief Registers a string variable
	 * @param varName The name of the variable
	 * @param value The value of the variable
	 * @param fastVarType The type of the variable
	 */
	int register_string(const char* flagName, std::string* flagVar, fast_var_type fastVarType = FASTVARTYPE_STATIC);

	/**
	 * @brief Do an operation on each variable
	 * @param visitor The function to call for each variable
	 * @param context The context to pass to the visitor function
	 * @param flagType The type of variables to visit
	 */
	void for_each_variable(variable_visitor_t visitor, void* context, fast_var_type flagType);

	/**
	 * @brief Sets the value of a variable
	 * @param name The name of the variable
	 * @param value The value to set
	 * @param fastVarType The type of the variable
	 */
	bool set_value(const std::string& name, const std::string& value, fast_var_type fastVarType = FASTVARTYPE_ANY);

	/**
	 * @brief Gets the value of a variable
	 * @param name The name of the variable
	 * @param value The value to set
	 * @param alsoCheckUnknownFlags Whether to also check unknown flags
	 */
	bool get_value(const std::string& name, std::string& value, bool alsoCheckUnknownFlags = false);
}}	// namespace rbx::log

LOGGROUP(Legacy)

namespace FLog {
/**
 * @brief The channel for the legacy log group Error
 */
const rbx::logging::channel_t Error = 1;

/**
 * @brief The channel for the legacy log group Warning
 */
const rbx::logging::channel_t Warning = 1;
};	// namespace FLog

#endif    // WITH_LOGGING

#endif    // RBX_LOGGING_LOGGING_API_H_