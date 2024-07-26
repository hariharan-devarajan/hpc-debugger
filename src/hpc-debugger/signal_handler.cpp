#include <hpc-debugger/signal_handler.h>

/* Internal header */
#include <hpc-debugger/backtrace.h>
/* external header */
#include <csignal>
hpc_debugger::HandlerFunction hpc_debugger::SingleHandler::selected_handler =
    hpc_debugger::default_handler;
void hpc_debugger::default_handler(int sig) {
  auto backtrace = hpc_debugger::Backtrace();
  backtrace.print();
  exit(0);
}

int hpc_debugger::SingleHandler::print_backtrace() {
  auto backtrace = hpc_debugger::Backtrace();
  backtrace.print();
  return 0;
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
