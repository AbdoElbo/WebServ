#include <cstddef>

int sendall(int s, char *buf, int *len);
const char *inet_ntop2(void *addr, char *buf, size_t size);
void	write_pipe(int fd);
void	listen_pipe(int fd);
int	pid_ok(int pid);

