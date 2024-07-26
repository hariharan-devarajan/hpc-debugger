#ifndef HPC_DEBUGGER_SIGNAL_HANDLER_H
#define HPC_DEBUGGER_SIGNAL_HANDLER_H

#include <csignal>
#include <functional>
#include <memory>

#include <hpc-debugger/common/configuration_manager.h>
namespace hpc_debugger {
typedef void (*HandlerFunction)(int);

void default_handler(int sig);

class SingleHandler {
  static HandlerFunction selected_handler;
  std::shared_ptr<hpc_debugger::ConfigurationManager> configuration;
  std::string print_backtrace();

public:
  SingleHandler();
  int add_handler(HandlerFunction handler);
  int add_signal(int signal, HandlerFunction handler = selected_handler);
};

} // namespace hpc_debugger

#endif /* HPC_DEBUGGER_SIGNAL_HANDLER_H */