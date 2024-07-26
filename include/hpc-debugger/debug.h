/* Config */
#if defined(HPC_DEBUGGER_HAS_CONFIG)
#include <hpc-debugger/hpc_debugger_config.hpp>
#else
#error "no config"
#endif

#include <memory>
namespace hpc_debugger {
class Debug {

public:
  Debug();
  int create_file();
};
} // namespace hpc_debugger
