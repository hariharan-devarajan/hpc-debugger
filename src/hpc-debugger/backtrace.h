#ifndef HPC_DEBUGGER_BACKTRACE_H
#define HPC_DEBUGGER_BACKTRACE_H

#include <hpc-debugger/common/configuration_manager.h>
#include <hpc-debugger/common/singleton.h>
#include <memory>
#include <string>
namespace hpc_debugger {
class Backtrace {
  std::shared_ptr<hpc_debugger::ConfigurationManager> configuration;

  std::string execute(const char *cmd);

public:
  Backtrace() {
    configuration = hpc_debugger::Singleton<
        hpc_debugger::ConfigurationManager>::get_instance();
  }
  int print();
};
} // namespace hpc_debugger

#endif /* HPC_DEBUGGER_BACKTRACE_H */