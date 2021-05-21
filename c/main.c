#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>


int counter = 0;
pthread_mutex_t lock;
bool stop = false;

void work(int identifier) {
    // pthread_mutex_lock(&lock);
    printf("%d: work %i...\n", identifier, counter);
    for (int i = 0; i < 65536; i++) {
        int j = i * 65536;
    }
    counter++;
    // pthread_mutex_unlock(&lock);
}

void *loop(void *identifier) {
    while (!stop) {
        work(*(int *) identifier);
    }

    return 0;
}

void start_threads() {
    pthread_t threads[8];
    for (int i = 0; i < 8; i++) {
        int *copied_i = malloc(sizeof *copied_i);
        *copied_i = i;

        // after this call, a new thread has been spawned
        if (pthread_create(&threads[i], NULL, loop, (void *) copied_i) != 0) {
            printf("error: failed to create thread %d", i);
            continue;
        }
    }
}

void start_processes() {
    int forked = 0;
    for (int i = 0; i < 9; i++) {
        if (forked < 0) {
            // error path that should never happen
            printf("error: failed to fork process %i", i);
            continue;
        }

        if (forked == 0) {
            // still in the parent process- do some forking
            forked = fork();
        }

        if (forked > 0) {
            // in the forked child process- do some work
            int *copied_i = malloc(sizeof *copied_i);
            *copied_i = i;
            loop((void *) copied_i);
        }
    }

    // parent process will end up here
}

void handler(int signo) {
    if (signo == SIGINT) {
        stop = true;
    }
}

int main(int argc, char **argv) {
    char usage[128];
    if (sprintf(usage, "usage: %s [thread|process]\n\n", argv[0]) < 1) {
        printf("error: couldn't build usage string\n");
        return 1;
    }

    if (signal(SIGINT, handler) == SIG_ERR) {
        printf("%s", usage);
        printf("error: could not set up CTRL + C handler\n");
        return 1;
    }

    if (argc < 2) {
        printf("%s", usage);
        printf("error: first argument not given\n");
        return 1;
    }

    if (strcmp(argv[1], "thread") == 0) {
        start_threads();
    } else if (strcmp(argv[1], "process") == 0) {
        start_processes();
    } else {
        printf("%s", usage);
        printf("error: first argument not one of [thread|process]\n");
        return 1;
    }

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("%s", usage);
        printf("error: failed to initialise mutex\n");
        return 1;
    }

    if (!stop) {
        printf("Press Ctrl + C to exit...\n");
        while (!stop) {
            usleep(1 * 1e6);
        }
    }

    return 0;
}
