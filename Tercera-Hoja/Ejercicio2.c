#define _POSIX_C_SOURCE 200809L //PARA PATH_MAX multiplataforma
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>


int main() {
    char cwd[PATH_MAX];

    // PID, PPID, PGID, SID
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgid = getpgid(0); // 0 = proceso actual
    pid_t sid = getsid(0);   // 0 = proceso actual

    // UID y GID
    uid_t uid = getuid();
    gid_t gid = getgid();

    // Directorio de trabajo actual
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return 1;
    }

    printf("PID: %d\n", pid);
    printf("PPID: %d\n", ppid);
    printf("PGID: %d\n", pgid);
    printf("SID: %d\n", sid);
    printf("UID: %d\n", uid);
    printf("GID: %d\n", gid);
    printf("Directorio actual: %s\n", cwd);

    return 0;
}
