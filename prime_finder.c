#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define IPC_KEY 0x1234
#define MAX_PRIMES 20

struct shared_data {
    int count;
    int primes[MAX_PRIMES];
};

struct msg_buffer {
    long mtype;
    int prime;
};

bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

int main() {
    // Set up shared memory
    int shmid = shmget(IPC_KEY, sizeof(struct shared_data), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    struct shared_data *shared_mem = (struct shared_data *)shmat(shmid, NULL, 0);
    if (shared_mem == (void *)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    // Initialize shared memory
    shared_mem->count = 0;

    // Set up message queue
    int msgid = msgget(IPC_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    // Main loop to find primes
    for (int num = 2; num <= 20; num++) {
        if (is_prime(num)) {
            // Add prime to shared memory
            if (shared_mem->count < MAX_PRIMES) {
                shared_mem->primes[shared_mem->count] = num;
                shared_mem->count++;
            } else {
                fprintf(stderr, "Shared memory full\n");
                break;
            }

            // Send message to message queue
            struct msg_buffer msg;
            msg.mtype = 1;
            msg.prime = num;

            if (msgsnd(msgid, &msg, sizeof(msg.prime), 0) == -1) {
                perror("msgsnd failed");
                exit(EXIT_FAILURE);
            }

            printf("Prime found and sent: %d\n", num);
        }
        sleep(1); // Simulate processing delay
    }

    // Cleanup
    if (shmdt(shared_mem) == -1) {
        perror("shmdt failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}
