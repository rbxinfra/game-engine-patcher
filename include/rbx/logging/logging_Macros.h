#ifndef RBX_LOGGING_LOGGING_MACROS_H_
#define RBX_LOGGING_LOGGING_MACROS_H_

#ifdef WITH_LOGGING

//////////////////////////////////////////////////////////////////
/// Begin Macros
//////////////////////////////////////////////////////////////////

/* fast_log */

/** Fast Log with arguments **/

/**
 * @brief Writes a log to the specified channel
 * @param[in] group The channel to write to
 * @param[in] message The message to write
 */
#	define FASTLOG(group, message)                                        \
		do                                                                 \
		{                                                                  \
			if (group)                                                     \
				rbx::logging::fast_log(group, "[" #group "] " message ""); \
		} while (0)

/** Formatted **/

/**
 * @brief Writes a formatted log to the specified channel
 * @param[in] group The channel to write to
 * @param[in] message The message to write
 */
#	define FASTLOGFMT(group, message, ...)                                                         \
		do                                                                                          \
		{                                                                                           \
			if (group)                                                                              \
				rbx::logging::fast_log_formatted(group, "[" #group "] " message "", ##__VA_ARGS__); \
		} while (0)

#	define LOG_HISTORY 2048  // Has to be a power of two. If changing, modify debugger entries below

/* Variables */

/** Fast Log **/

/**
 * @brief Defines an external log group
 * @param[in] group The name of the group
 */
#	define LOGGROUP(group)                   \
		namespace FLog {                      \
		extern rbx::logging::channel_t group; \
		}

/**
 * @brief Declares a log group
 * @param[in] group The name of the group
 * @param[in] defaulton The default state of the group
 */
#	define LOGVARIABLE(group, defaulton)                                                                                            \
		namespace FLog {                                                                                                             \
		rbx::logging::channel_t group		   = defaulton;                                                                          \
		std::int32_t			group##_initer = rbx::logging::register_log_group(#group, &group, rbx::logging::FASTVARTYPE_STATIC); \
		}

/**
 * @brief Defines a dynamic external log group
 * @param[in] group The name of the group
 */
#	define DYNAMIC_LOGGROUP(group)           \
		namespace DFLog {                     \
		extern rbx::logging::channel_t group; \
		}

/**
 * @brief Declares a dynamic log group
 * @param[in] group The name of the group
 * @param[in] defaulton The default state of the group
 */
#	define DYNAMIC_LOGVARIABLE(group, defaulton)                                                                                     \
		namespace DFLog {                                                                                                             \
		rbx::logging::channel_t group		   = defaulton;                                                                           \
		std::int32_t			group##_initer = rbx::logging::register_log_group(#group, &group, rbx::logging::FASTVARTYPE_DYNAMIC); \
		}

/** Fast String **/

/**
 * @brief Defines an external string variable
 * @param[in] var The name of the variable
 */
#	define FASTSTRING(var)     \
		namespace FString {     \
		extern std::string var; \
		}

/**
 * @brief Declares a string variable
 * @param[in] var The name of the variable
 * @param[in] defaultValue The default value of the variable
 */
#	define FASTSTRINGVARIABLE(var, defaultValue)                                                                \
		namespace FString {                                                                                      \
		std::string	 var		  = defaultValue;                                                                \
		std::int32_t var##_initer = rbx::logging::register_string(#var, &var, rbx::logging::FASTVARTYPE_STATIC); \
		}

/**
 * @brief Defines a dynamic external string variable
 * @param[in] var The name of the variable
 */
#	define DYNAMIC_FASTSTRING(var) \
		namespace DFString {        \
		extern std::string var;     \
		}

/**
 * @brief Declares a dynamic string variable
 * @param[in] var The name of the variable
 * @param[in] defaultValue The default value of the variable
 */
#	define DYNAMIC_FASTSTRINGVARIABLE(var, defaultValue)                                                         \
		namespace DFString {                                                                                      \
		std::string	 var		  = defaultValue;                                                                 \
		std::int32_t var##_initer = rbx::logging::register_string(#var, &var, rbx::logging::FASTVARTYPE_DYNAMIC); \
		}

/** Fast Flag **/

/**
 * @brief Defines an external flag variable
 * @param[in] var The name of the variable
 */
#	define FASTFLAG(var) \
		namespace FFlag { \
		extern Value var; \
		}

/**
 * @brief Declares a flag variable
 * @param[in] var The name of the variable
 * @param[in] defaultValue The default value of the variable
 */
#	define FASTFLAGVARIABLE(var, defaultValue)                                                                \
		namespace FFlag {                                                                                      \
		Value		 var		  = defaultValue;                                                              \
		std::int32_t var##_initer = rbx::logging::register_flag(#var, &var, rbx::logging::FASTVARTYPE_STATIC); \
		}

/**
 * @brief Defines a dynamic external flag variable
 * @param[in] var The name of the variable
 */
#	define DYNAMIC_FASTFLAG(var) \
		namespace DFFlag {        \
		extern FFlag::Value var;  \
		}

/**
 * @brief Declares a dynamic flag variable
 * @param[in] var The name of the variable
 * @param[in] defaultValue The default value of the variable
 */
#	define DYNAMIC_FASTFLAGVARIABLE(var, defaultValue)                                                         \
		namespace DFFlag {                                                                                      \
		FFlag::Value var		  = defaultValue;                                                               \
		std::int32_t var##_initer = rbx::logging::register_flag(#var, &var, rbx::logging::FASTVARTYPE_DYNAMIC); \
		}

/** Fast Int **/

/**
 * @brief Defines an external integer variable
 * @param[in] var The name of the variable
 */
#	define FASTINT(var)         \
		namespace FInt {         \
		extern std::int32_t var; \
		}

/**
 * @brief Declares an integer variable
 * @param[in] var The name of the variable
 * @param[in] defaultValue The default value of the variable
 */
#	define FASTINTVARIABLE(var, defaultValue)                                                                 \
		namespace FInt {                                                                                       \
		std::int32_t var		  = defaultValue;                                                              \
		std::int32_t var##_initer = rbx::logging::register_flag(#var, &var, rbx::logging::FASTVARTYPE_STATIC); \
		}

/**
 * @brief Defines a dynamic external integer variable
 * @param[in] var The name of the variable
 */
#	define DYNAMIC_FASTINT(var) \
		namespace DFInt {        \
		extern std::int32_t var; \
		}

/**
 * @brief Declares a dynamic integer variable
 * @param[in] var The name of the variable
 * @param[in] defaultValue The default value of the variable
 */
#	define DYNAMIC_FASTINTVARIABLE(var, defaultValue)                                                          \
		namespace DFInt {                                                                                       \
		std::int32_t var		  = defaultValue;                                                               \
		std::int32_t var##_initer = rbx::logging::register_flag(#var, &var, rbx::logging::FASTVARTYPE_DYNAMIC); \
		}

/**
 * @brief Raw set variable
 * @param[in] name The name of the variable
 * @param[in] value The value of the variable
 */
#	define RAWSET_VARIABLE(name, value)            \
		do                                          \
		{                                           \
			rbx::logging::set_value(#name, #value); \
		} while (0)

/**
 * @brief Raw get variable
 * @param[in] name The name of the variable
 * @param[in] value The value of the variable
 */
#	define RAWGET_VARIABLE(name, value)                 \
		do                                               \
		{                                                \
			rbx::logging::get_value(#name, value, true); \
		} while (0)

/**
 * @brief Gets the value log channels
 */
#	define LOGCHANNELS 5

/**
 * @brief Gets the file channel
 * @param[in] id The id of the channel
 */
#	define FILECHANNEL(id) (LOGCHANNELS + id)

#else

#	define FASTLOG(group, message)			((void)0)
#	define FASTLOGFMT(group, message, ...) ((void)0)

#	define LOGGROUP(group)                   \
		namespace FLog {                      \
		extern rbx::logging::channel_t group; \
		}
#	define LOGVARIABLE(group, defaulton)                \
		namespace FLog {                                 \
		const rbx::logging::channel_t group = defaulton; \
		}

#	define DYNAMIC_LOGGROUP(group)           \
		namespace DFLog {                     \
		extern rbx::logging::channel_t group; \
		}
#	define DYNAMIC_LOGVARIABLE(group, defaulton)        \
		namespace DFLog {                                \
		const rbx::logging::channel_t group = defaulton; \
		}

#	define FASTSTRING(var)     \
		namespace FString {     \
		extern std::string var; \
		}
#	define FASTSTRINGVARIABLE(var, defaultValue) \
		namespace FString {                       \
		const std::string var = defaultValue;     \
		}

#	define DYNAMIC_FASTSTRING(var) \
		namespace DFString {        \
		extern std::string var;     \
		}
#	define DYNAMIC_FASTSTRINGVARIABLE(var, defaultValue) \
		namespace DFString {                              \
		const std::string var = defaultValue;             \
		}

#	define FASTFLAG(var) \
		namespace FFlag { \
		extern bool var;  \
		}
#	define FASTFLAGVARIABLE(var, defaultValue) \
		namespace FFlag {                       \
		const bool var = defaultValue;          \
		}

#	define DYNAMIC_FASTFLAG(var) \
		namespace DFFlag {        \
		extern bool var;          \
		}
#	define DYNAMIC_FASTFLAGVARIABLE(var, defaultValue) \
		namespace DFFlag {                              \
		const bool var = defaultValue;                  \
		}

#	define FASTINT(var)         \
		namespace FInt {         \
		extern std::int32_t var; \
		}
#	define FASTINTVARIABLE(var, defaultValue) \
		namespace FInt {                       \
		const std::int32_t var = defaultValue; \
		}

#	define DYNAMIC_FASTINT(var) \
		namespace DFInt {        \
		extern std::int32_t var; \
		}
#	define DYNAMIC_FASTINTVARIABLE(var, defaultValue) \
		namespace DFInt {                              \
		const std::int32_t var = defaultValue;         \
		}

#	define RAWSET_VARIABLE(name, value)
#	define RAWGET_VARIABLE(name, value)

#endif

//////////////////////////////////////////////////////////////////
/// End Macros
//////////////////////////////////////////////////////////////////

#endif	// !RBX_LOGGING_LOGGING_MACROS_H_