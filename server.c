#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char * argv[]) {
    int myPipeC = open("myPipeC", O_RDWR);
    int myPipeS = open("myPipeS", O_RDWR);
    char buf[255];
    char send[255];
    int n;

    while (1) {
        printf("You: ");
        fgets(send, sizeof(send), stdin);

        int len = strlen(send);
        write(myPipeS, send, (len + 1) * sizeof(char));

        if (strcmp(send, "exit\n") == 0) {
            close(myPipeC);
            close(myPipeS);
            exit(EXIT_SUCCESS);
        }

        while ((n = read(myPipeC, &buf, sizeof(buf))) > 0) {
            printf("Client: %s", buf);
        }
    }

    return 0;
}
