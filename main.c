#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "main.h"

void sHandler(int signo)
{
    printf("Handling signal %d...\n", signo);

    exit(0);
}

int main(int argc, char *argv[])
{
    struct sigaction sigact,
		     oldsigact;

    print_signals( );

    sigemptyset(&sigact.sa_mask);
    sigact.sa_handler = (void (*)()) sHandler;
    sigact.sa_flags = 0;

    sigaction(SIGINT, &sigact, &oldsigact);

    printf("SIG_DFL            [%p]\n", SIG_DFL);
    printf("Old Signal Handler [%p]\n", oldsigact.sa_handler);

    print_signals( );

    exit(0);
}
