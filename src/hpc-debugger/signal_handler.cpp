
#include <hpc-debugger/signal_handler.h>
/* Internal */
#include <hpc-debugger/common/singleton.h>
/* Internal header */
#include <hpc-debugger/backtrace.h>
/* external header */
#include <csignal>
hpc_debugger::HandlerFunction hpc_debugger::SingleHandler::selected_handler =
    hpc_debugger::default_handler;
void hpc_debugger::default_handler(int sig) {
  auto backtrace = hpc_debugger::Backtrace();
  auto str = backtrace.print();
  fprintf(stderr, "%s", str.c_str());
  exit(sig);
}

std::string hpc_debugger::SingleHandler::print_backtrace() {
  auto backtrace = hpc_debugger::Backtrace();
  return backtrace.print();
}

hpc_debugger::SingleHandler::SingleHandler() {}

int hpc_debugger::SingleHandler::add_handler(HandlerFunction handler) {
  hpc_debugger::SingleHandler::selected_handler = handler;
  return 0;
}

int hpc_debugger::SingleHandler::add_signal(int signal,
                                            HandlerFunction handler) {
  struct sigaction signal_action;
  sigemptyset(&signal_action.sa_mask);
  signal_action.sa_flags = SA_NOMASK;
  signal_action.sa_handler = handler;
  sigaction(signal, &signal_action, NULL);
  return 0;
}
