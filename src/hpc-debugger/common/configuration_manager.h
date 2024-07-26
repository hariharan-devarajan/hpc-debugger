//
// Created by haridev on 10/27/23.
//

#ifndef HPC_DEBUGGER_CONFIGURATION_MANAGER_H
#define HPC_DEBUGGER_CONFIGURATION_MANAGER_H

/* Config */
#if defined(HPC_DEBUGGER_HAS_CONFIG)
#include <hpc-debugger/hpc_debugger_config.hpp>
#else
#error "no config"
#endif

#include <hpc-debugger/common/logging.h>
#include <hpc-debugger/common/singleton.h>
#include <string>
#include <vector>
namespace hpc_debugger {
class ConfigurationManager {
public:
  std::string debug_file;
  int debug_port;
  ConfigurationManager();
  void finalize() {}
};
} // namespace hpc_debugger
#endif // HPC_DEBUGGER_CONFIGURATION_MANAGER_H