
#include <hpc-debugger/debug.h>
/* STL */
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
/* External */
#if defined(HPC_DEBUGGER_ENABLE_MPI)
#include <mpi.h>
#endif
/* Internal */
#include <hpc-debugger/common/configuration_manager.h>
#include <hpc-debugger/common/singleton.h>
hpc_debugger::Debug::Debug() {}

int hpc_debugger::Debug::create_file() {
  auto configuration = hpc_debugger::Singleton<
      hpc_debugger::ConfigurationManager>::get_instance();
#if defined(HPC_DEBUGGER_ENABLE_MPI)
  int rank, comm_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  const int HOSTNAME_SIZE = 256;
  char hostname[HOSTNAME_SIZE];
  gethostname(hostname, HOSTNAME_SIZE);
  int pid = getpid();
  int start_port = configuration->debug_port;
  const char *conf_file = configuration->debug_file.c_str();
  char exe[1024];
  int ret = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
  exe[ret] = 0;
  if (rank == 0) {
    remove(conf_file);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_File mpi_fh;
  int status_orig =
      MPI_File_open(MPI_COMM_WORLD, conf_file,
                    MPI_MODE_WRONLY | MPI_MODE_CREATE, MPI_INFO_NULL, &mpi_fh);
  (void)status_orig;
  const int buf_len = 16 * 1024;
  char buffer[buf_len];
  int size;
  if (rank == 0) {
    size = sprintf(buffer, "%d\n%s:%d:%s:%d:%d\n", comm_size, exe, rank,
                   hostname, start_port + rank, pid);
  } else {
    size = sprintf(buffer, "%s:%d:%s:%d:%d\n", exe, rank, hostname,
                   start_port + rank, pid);
  }
  MPI_Status status;
  MPI_File_write_ordered(mpi_fh, buffer, size, MPI_CHAR, &status);
  int written_bytes;
  MPI_Get_count(&status, MPI_CHAR, &written_bytes);
  MPI_File_close(&mpi_fh);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) {
    printf("%d ready for attach\n", comm_size);
    fflush(stdout);
    sleep(30);
  }
#else
  const int HOSTNAME_SIZE = 256;
  char hostname[HOSTNAME_SIZE];
  gethostname(hostname, HOSTNAME_SIZE);
  int pid = getpid();
  int start_port = configuration->debug_port;
  const char *conf_file = configuration->debug_file.c_str();
  char exe[1024];
  int ret = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
  exe[ret] = 0;
  remove(conf_file);
  int fd = open(conf_file, O_APPEND | O_CREAT, 777);
  if (fd != -1) {
    const int buf_len = 16 * 1024;
    char buffer[buf_len];
    int size = sprintf(buffer, "%d\n%s:%d:%s:%d:%d\n", 1, exe, 0, hostname,
                       start_port + 0, pid);
    ssize_t written_bytes = write(fd, buffer, size);
    if (size < written_bytes) {
      fprintf(stderr, "Unable to Write. Written %ld of %d. %d (%s)\n",
              written_bytes, size, errno, strerror(errno));
      return -1;
    }
    close(fd);
  } else {
    fprintf(stderr, "Unable to open file %s %d (%s)\n", conf_file, errno,
            strerror(errno));
    return -1;
  }
  printf("ready for attach\n");
  fflush(stdout);
  sleep(30);
#endif
  return 0;
}