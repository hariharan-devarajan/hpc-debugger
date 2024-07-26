#ifndef HPC_DEBUGGER_COMMON_LOGGING_H
#define HPC_DEBUGGER_COMMON_LOGGING_H
#if defined(HPC_DEBUGGER_HAS_CONFIG)
#include <hpc-debugger/hpc_debugger_config.hpp>
#else
#error "no config"
#endif
#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

// #define HPC_DEBUGGER_NOOP_MACRO do {} while (0)

//=============================================================================
#ifdef HPC_DEBUGGER_LOGGER_NO_LOG
//=============================================================================
#define HPC_DEBUGGER_LOGGER_INIT() HPC_DEBUGGER_NOOP_MACRO
#define HPC_DEBUGGER_LOG_STDERR(...) HPC_DEBUGGER_NOOP_MACRO
#define HPC_DEBUGGER_LOG_STDOUT(...) HPC_DEBUGGER_NOOP_MACRO
#define HPC_DEBUGGER_LOG_ERROR(...) HPC_DEBUGGER_NOOP_MACRO
#define HPC_DEBUGGER_LOG_WARN(...) HPC_DEBUGGER_NOOP_MACRO
#define HPC_DEBUGGER_LOG_INFO(...) HPC_DEBUGGER_NOOP_MACRO
#define HPC_DEBUGGER_LOG_DEBUG(...) HPC_DEBUGGER_NOOP_MACRO
#define HPC_DEBUGGER_LOG_TRACE(...) HPC_DEBUGGER_NOOP_MACRO
#define HPC_DEBUGGER_LOG_STDOUT_REDIRECT(fpath) HPC_DEBUGGER_NOOP_MACRO
#define HPC_DEBUGGER_LOG_STDERR_REDIRECT(fpath) HPC_DEBUGGER_NOOP_MACRO
//=============================================================================
#else
//=============================================================================
#define HPC_DEBUGGER_LOG_STDERR(...) fprintf(stderr, __VA_ARGS__);
#define HPC_DEBUGGER_LOG_STDOUT(...) fprintf(stdout, __VA_ARGS__);
// CPP_LOGGER ---------------------------
#if defined(HPC_DEBUGGER_LOGGER_CPP_LOGGER)
#include <cpp-logger/clogger.h>

#define HPC_DEBUGGER_LOGGER_NAME "HPC_DEBUGGER"
#ifdef HPC_DEBUGGER_LOGGER_LEVEL_TRACE
#define HPC_DEBUGGER_LOGGER_INIT()                                             \
  cpp_logger_clog_level(CPP_LOGGER_TRACE, HPC_DEBUGGER_LOGGER_NAME);
#elif defined(HPC_DEBUGGER_LOGGER_LEVEL_DEBUG)
#define HPC_DEBUGGER_LOGGER_INIT()                                             \
  cpp_logger_clog_level(CPP_LOGGER_DEBUG, HPC_DEBUGGER_LOGGER_NAME);
#elif defined(HPC_DEBUGGER_LOGGER_LEVEL_INFO)
#define HPC_DEBUGGER_LOGGER_INIT()                                             \
  cpp_logger_clog_level(CPP_LOGGER_INFO, HPC_DEBUGGER_LOGGER_NAME);
#elif defined(HPC_DEBUGGER_LOGGER_LEVEL_WARN)
#define HPC_DEBUGGER_LOGGER_INIT()                                             \
  cpp_logger_clog_level(CPP_LOGGER_WARN, HPC_DEBUGGER_LOGGER_NAME);
#else
#define HPC_DEBUGGER_LOGGER_INIT()                                             \
  cpp_logger_clog_level(CPP_LOGGER_ERROR, HPC_DEBUGGER_LOGGER_NAME);
#endif

#define HPC_DEBUGGER_LOG_STDOUT_REDIRECT(fpath) freopen((fpath), "a+", stdout);
#define HPC_DEBUGGER_LOG_STDERR_REDIRECT(fpath) freopen((fpath), "a+", stderr);

#ifdef HPC_DEBUGGER_LOGGER_LEVEL_TRACE
#define HPC_DEBUGGER_LOG_TRACE(...)                                            \
  cpp_logger_clog(CPP_LOGGER_TRACE, HPC_DEBUGGER_LOGGER_NAME, __VA_ARGS__);
#else
#define HPC_DEBUGGER_LOG_DEBUG(...) HPC_DEBUGGER_NOOP_MACRO
#endif

#ifdef HPC_DEBUGGER_LOGGER_LEVEL_DEBUG
#define HPC_DEBUGGER_LOG_DEBUG(...)                                            \
  cpp_logger_clog(CPP_LOGGER_DEBUG, HPC_DEBUGGER_LOGGER_NAME, __VA_ARGS__);
#else
#define HPC_DEBUGGER_LOG_DEBUG(...) HPC_DEBUGGER_NOOP_MACRO
#endif

#ifdef HPC_DEBUGGER_LOGGER_LEVEL_INFO
#define HPC_DEBUGGER_LOG_INFO(...)                                             \
  cpp_logger_clog(CPP_LOGGER_INFO, HPC_DEBUGGER_LOGGER_NAME, __VA_ARGS__);
#else
#define HPC_DEBUGGER_LOG_INFO(...) HPC_DEBUGGER_NOOP_MACRO
#endif

#ifdef HPC_DEBUGGER_LOGGER_LEVEL_WARN
#define HPC_DEBUGGER_LOG_WARN(...)                                             \
  cpp_logger_clog(CPP_LOGGER_WARN, HPC_DEBUGGER_LOGGER_NAME, __VA_ARGS__);
#else
#define HPC_DEBUGGER_LOG_WARN(...) HPC_DEBUGGER_NOOP_MACRO
#endif

#ifdef HPC_DEBUGGER_LOGGER_LEVEL_ERROR
#define HPC_DEBUGGER_LOG_ERROR(...)                                            \
  cpp_logger_clog(CPP_LOGGER_ERROR, HPC_DEBUGGER_LOGGER_NAME, __VA_ARGS__);
#else
#define HPC_DEBUGGER_LOG_ERROR(...) HPC_DEBUGGER_NOOP_MACRO
#endif
#endif // HPC_DEBUGGER_LOGGER_CPP_LOGGER
       // ----------------------------------------------------
//=============================================================================
#endif // HPC_DEBUGGER_LOGGER_NO_LOG
//=============================================================================

#ifdef __cplusplus
}
#endif

#endif /* HPC_DEBUGGER_COMMON_LOGGING_H */
