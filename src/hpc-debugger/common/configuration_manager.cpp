//
// Created by haridev on 10/27/23.
//

#include "configuration_manager.h"

#include <hpc-debugger/common/constants.h>
#include <hpc-debugger/common/logging.h>
#include <yaml-cpp/yaml.h>

#define HD_YAML_DEBUG_FILE "debug_file"
#define HD_YAML_DEBUG_PORT "debug_port"
template <>
std::shared_ptr<hpc_debugger::ConfigurationManager>
    hpc_debugger::Singleton<hpc_debugger::ConfigurationManager>::instance =
        nullptr;
template <>
bool hpc_debugger::Singleton<
    hpc_debugger::ConfigurationManager>::stop_creating_instances = false;
hpc_debugger::ConfigurationManager::ConfigurationManager()
    : debug_file("./debug.conf"), debug_port(8000) {
  const char *env_conf = getenv(HPC_DEBUGGER_CONFIGURATION);
  YAML::Node config;
  if (env_conf != nullptr) {
    config = YAML::LoadFile(env_conf);
    if (config[HD_YAML_DEBUG_FILE]) {
      this->debug_file = config[HD_YAML_DEBUG_FILE].as<std::string>();
    }
    if (config[HD_YAML_DEBUG_PORT]) {
      this->debug_port = config[HD_YAML_DEBUG_PORT].as<int>();
    }
  }

  const char *debug_file_env = getenv(HPC_DEBUGGER_DEBUG_FILE);
  if (debug_file_env != nullptr) {
    this->debug_file = std::string(debug_file_env);
  }
  HPC_DEBUGGER_LOG_DEBUG("ENV ConfigurationManager.debug_file %s",
                         this->debug_file);

  const char *debug_port_env = getenv(HPC_DEBUGGER_DEBUG_PORT);
  if (debug_port_env != nullptr) {
    this->debug_port = atoi(debug_port_env);
  }
  HPC_DEBUGGER_LOG_DEBUG("ENV ConfigurationManager.debug_port %d",
                         this->debug_port);
  HPC_DEBUGGER_LOG_DEBUG("ENV ConfigurationManager finished", "");
}