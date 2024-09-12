#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void calculate_pi_formatted(char *pi_string, int ndigits) 
{
    double pi = 2.0 * asin(1);
    char format_str[10] = "%.";
    char digits_str[10];
    sprintf(digits_str, "%d", ndigits);
    strcat(format_str, digits_str);
    strcat(format_str, "f");
    sprintf(pi_string, format_str, pi);
}

int main(int argc, char *argv[])
{
    char pi_formatted[50];

    if (argc != 2) 
    {
        printf("Wrong usage\n");
        return 1;
    }

    int ndigits = atoi(argv[1]);
    if (ndigits <= 0) 
    {
        printf("Wrong usage\n");
        return 1;
    }

    int total_digits = ndigits + 2;
    pid_t pid;

    int fd[2];

    if (pipe(fd) < 0) {
        fprintf(stderr, "Creaping pipe failed\n");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    }
    else if (pid == 0) {
        
        calculate_pi_formatted(pi_formatted, total_digits);
        write(fd[1], pi_formatted, total_digits);
    }
    else {
        wait(NULL);
        char response[256];
        read(fd[0], response, total_digits);
        printf("%s\n", response);
    }

    return 0;
}
