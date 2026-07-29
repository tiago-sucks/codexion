notas:

In C language, POSIX <pthread.h> standard API (Application program Interface) for all thread related functions.
It allows us to create multiple threads for concurrent process flows.

To execute the C programs that uses these functions, we may have to use the -pthread or -lpthread in the command line while compiling the file.

```cc -pthread file.c```

```cc -lpthread file.c```

| Function | Description |
|---|---|
| `pthread_create()` | Creates a new thread and starts execution. Takes a function pointer that will be executed in the new thread. |
| `pthread_exit()` | Terminates the calling thread. This does not affect other threads. |
| `pthread_join()` | Waits for the specified thread to terminate. It blocks the calling thread until the target thread finishes. |
| `pthread_detach()` | Detaches a thread, meaning it will automatically release resources when it terminates without needing `pthread_join()`. |
| `pthread_cancel()` | Requests cancellation of a thread. The thread must check periodically for cancellation to exit safely. |
| `pthread_self()` | Returns the thread ID of the calling thread. |
| `pthread_equal()` | Compares two thread IDs and returns non-zero if they are the same, zero otherwise. |
| `pthread_mutex_init()` | Initializes a mutex, which is used to prevent race conditions in concurrent programming. |
| `pthread_mutex_destroy()` | Destroys a mutex when it's no longer needed. |
| `pthread_mutex_lock()` | Locks a mutex. If the mutex is already locked by another thread, the calling thread will be blocked. |
| `pthread_mutex_unlock()` | Unlocks a mutex that was previously locked by the calling thread. |
|

# Quantum Compiler Sync — TODO

```
./quantum_sync number_of_coders time_to_burnout time_to_compile \
               time_to_debug time_to_refactor number_of_compiles_required \
               dongle_cooldown scheduler
```

## 1. Argument Parsing

- [ ] Parse and validate all 8 mandatory arguments (no defaults)
- [ ] `scheduler` must be exactly `fifo` or `edf`
- [ ] Numeric args must be positive integers
- [ ] Handle `number_of_coders == 1`: only one dongle exists, so this coder can never acquire two — must burn out after `time_to_burnout` ms without deadlocking

## 2. Data Structures (no globals)

- [ ] `Simulation` struct: holds all config + shared state, passed by pointer
- [ ] `Coder` struct: id, last_compile_start, compiles_done, state, thread handle
- [ ] `Dongle` struct: id, availability, last_released timestamp, lock
- [ ] Shared request queue for arbitration (arrival order + deadline)
- [ ] Mutex protecting shared state (stop flag, compile counts, dongle table)
- [ ] `stop_simulation` flag checked by every thread

## 3. Dongle Arbitration

- [ ] Acquire left + right dongles simultaneously, avoiding deadlock (e.g. ordered pickup or all-or-nothing retry)
- [ ] Respect `dongle_cooldown` after release
- [ ] FIFO: grant to earliest requester per dongle
- [ ] EDF: grant to earliest deadline (`last_compile_start + time_to_burnout`)
- [ ] Scheduler passed through config, not global

## 4. Coder Lifecycle

```
acquire dongles -> compile -> release dongles -> debug -> refactor -> repeat
```

- [ ] Update `last_compile_start` when compiling begins (resets burnout clock)
- [ ] Increment compile counter after each compile
- [ ] Stamp `last_released` on both dongles at release

## 5. Burnout Monitor

- [ ] Poll: `now - last_compile_start > time_to_burnout` → burnout
- [ ] On detection: set stop flag, log event, wake waiting threads
- [ ] Frequent enough to catch it promptly, without busy-spinning

## 6. Termination

- [ ] Any coder burns out → stop immediately
- [ ] All coders reach `number_of_compiles_required` → stop cleanly
- [ ] On stop: signal threads, join, clean up, print final state

## 7. Logging

- [ ] Timestamped events: compiling, debugging, refactoring, burned out, stopped
- [ ] Thread-safe stdout (own mutex)

## 8. Concurrency Safety

- [ ] No global/static mutable state
- [ ] No deadlock (test all-coders-compile-at-once)
- [ ] No data races (mutex on every shared read/write)
- [ ] No unintended starvation under FIFO/EDF

## 9. Testing

- [ ] `number_of_coders = 1` → immediate burnout
- [ ] Tight `time_to_burnout` vs long phases → burnout triggers on time
- [ ] Generous timing, reachable compile target → clean stop
- [ ] FIFO vs EDF under contention → correct, valid arbitration
- [ ] High coder count → race-free (ThreadSanitizer / Helgrind)

---

Notes: coders never communicate and don't know about others' burnout risk. Seating is circular — coder 1 sits next to coder N.