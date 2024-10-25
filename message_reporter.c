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

int main() {
    // Set up shared memory
    int shmid = shmget(IPC_KEY, sizeof(struct shared_data), 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    struct shared_data *shared_mem = (struct shared_data *)shmat(shmid, NULL, 0);
    if (shared_mem == (void *)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    // Set up message queue
    int msgid = msgget(IPC_KEY, 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    // Main loop to receive messages
    struct msg_buffer msg;
    int received_count = 0;

    while (1) {
        if (msgrcv(msgid, &msg, sizeof(msg.prime), 1, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }

        printf("\nMessage received: Prime found: %d\n", msg.prime);

        // Read and print all primes from shared memory
        printf("Primes in shared memory: ");
        for (int i = 0; i < shared_mem->count; i++) {
            printf("%d", shared_mem->primes[i]);
            if (i < shared_mem->count - 1) {
                printf(", ");
            }
        }
        printf("\n");

        received_count++;
        if (received_count >= MAX_PRIMES || msg.prime >= 19) {
            // Assuming we stop after receiving all primes up to 19
            break;
        }
    }

    // Cleanup
    if (shmdt(shared_mem) == -1) {
        perror("shmdt failed");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl failed");
        exit(EXIT_FAILURE);
    }

    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(EXIT_FAILURE);
    }

    printf("Cleanup complete.\n");

    return 0;
}
