#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int a = 3;
void main() {
    int b=2;
    for (int i=0;i<4;i++) {
        pid_t p=fork();
        if (p==0) {
            b++;
            execlp("/usr/bin/sleep", "/usr/bin/sleep", "2", NULL);
            a++;
        }
        else {
           wait(NULL);
           a++;
           b--;
        }
    }
    printf("variables - a:%d b:%d\n", a, b);
}
