#ifndef HPC_DEBUGGER_SIGNAL_HANDLER_H
#define HPC_DEBUGGER_SIGNAL_HANDLER_H

#include <csignal>
#include <functional>
#include <memory>

namespace hpc_debugger {
typedef void (*HandlerFunction)(int);

void default_handler(int sig);

class SingleHandler {
  static HandlerFunction selected_handler;
  std::string print_backtrace();

public:
  SingleHandler();
  int add_handler(HandlerFunction handler);
  int add_signal(int signal, HandlerFunction handler = selected_handler);
};

} // namespace hpc_debugger

#endif /* HPC_DEBUGGER_SIGNAL_HANDLER_H */