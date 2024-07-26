
#include <cstdlib>
#include <cstring>
#include <execinfo.h>
#include <hpc-debugger/backtrace.h>
#include <string>

/* STL */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <elf.h>
#include <iostream>
#include <link.h>
#include <regex>

std::string hpc_debugger::Backtrace::execute(const char *cmd) {
  FILE *fp;
  char path[1035];
  /* Open the command for reading. */
  fp = popen(cmd, "r");
  if (fp == NULL) {
    printf("Failed to run command\n");
    exit(1);
  }
  /* Read the output a line at a time - output it. */
  fgets(path, sizeof(path) - 1, fp);
  int return_size = strlen(path);
  path[return_size - 1] = '\0';
  /* close */
  pclose(fp);
  return path;
}

std::string hpc_debugger::Backtrace::print() {
  void *array[128];
  char **strings;
  int size, i;

  size = backtrace(array, 128);
  strings = backtrace_symbols(array, size);
  std::stringstream all_stream;

  if (strings != NULL) {
    all_stream << "Obtained " << size << " stack frames.\n";
    for (i = 0; i < size; i++) {

      std::regex rgx("([a-zA-Z0-9\\/"
                     "\\.\\-\\_]+)\\(([\\_\\-A-Za-z0-9]*)[\\+]*.*\\s\\[0x([0-"
                     "9a-zA-Z]+)\\]");
      std::smatch match;
      const std::string s = std::string(strings[i]);
      if (std::regex_search(s.begin(), s.end(), match, rgx)) {
        std::string executable = match[1].str();
        std::string function_name = match[2].str();
        std::string function_addr = match[3].str();
        if (function_name != "") {
          char symbol_address[4096];
          sprintf(symbol_address, "nm %s | grep %s | awk {'print $1'}",
                  executable.c_str(), function_name.c_str());
          function_addr = execute(symbol_address);
          function_addr.erase(0, function_addr.find_first_not_of('0'));
          function_addr = "0x" + function_addr;
        }
        // printf("matched %s\n", match[1].str().c_str());
        char syscom[4096];
        sprintf(syscom,
                "/usr/tce/bin/addr2line %s -p -f -C -e "
                "%s",
                function_addr.c_str(),
                executable.c_str()); // last parameter is the name of this
        // printf("cmd %s\n", syscom);
        std::string value = execute(syscom);
        if (value.find("??") != std::string::npos) {
          // pos=0 limits the search to the prefix
          all_stream << strings[i] << "\n";
        } else {
          all_stream << value << "\n";
        }

      } else {
        all_stream << strings[i] << "\n";
      }
    }
  }
  free(strings);
  return all_stream.str();
}