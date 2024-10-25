
## Compilation and Execution

1. **Compile Both Programs**:

   ```bash
   gcc -o prime_finder prime_finder.c
   gcc -o message_reporter message_reporter.c
   ```

2. **Run the Programs**:

   - **Terminal 1**: Start the `message_reporter` first to ensure it's ready to receive messages.

     ```bash
     ./message_reporter
     ```

   - **Terminal 2**: Run the `prime_finder`.

     ```bash
     ./prime_finder
     ```

---

## Expected Output

**Terminal 1 (`message_reporter`)**:

```
Message received: Prime found: 2
Primes in shared memory: 2

Message received: Prime found: 3
Primes in shared memory: 2, 3

Message received: Prime found: 5
Primes in shared memory: 2, 3, 5

Message received: Prime found: 7
Primes in shared memory: 2, 3, 5, 7

Message received: Prime found: 11
Primes in shared memory: 2, 3, 5, 7, 11

Message received: Prime found: 13
Primes in shared memory: 2, 3, 5, 7, 11, 13

Message received: Prime found: 17
Primes in shared memory: 2, 3, 5, 7, 11, 13, 17

Message received: Prime found: 19
Primes in shared memory: 2, 3, 5, 7, 11, 13, 17, 19
Cleanup complete.
```

**Terminal 2 (`prime_finder`)**:

```
Prime found and sent: 2
Prime found and sent: 3
Prime found and sent: 5
Prime found and sent: 7
Prime found and sent: 11
Prime found and sent: 13
Prime found and sent: 17
Prime found and sent: 19
```

---
