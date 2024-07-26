#ifndef HPC_DEBUGGER_BACKTRACE_H
#define HPC_DEBUGGER_BACKTRACE_H

#include <string>

namespace hpc_debugger {
class Backtrace {
  std::string execute(const char *cmd);

public:
  Backtrace() {}
  int print();
};
} // namespace hpc_debugger

#endif /* HPC_DEBUGGER_BACKTRACE_H */