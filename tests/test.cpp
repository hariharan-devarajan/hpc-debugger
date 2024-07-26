
#include <hpc-debugger/signal_handler.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

int func_a(int a, char b) {

  char *p = (char *)0xdeadbeef;

  a = a + b;
  *p = 10; /* CRASH here!! */

  return 2 * a;
}

int func_b(std::string str) {

  int res, a = 5;

  res = 5 + func_a(a, 't');

  return res;
}

int main() {
  hpc_debugger::SingleHandler handler;
  handler.add_signal(SIGSEGV);
  handler.add_signal(SIGINT);

  /* Do something */
  printf("%d\n", func_b("hello"));
}