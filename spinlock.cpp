#include "spinlock.h"

void SpinLock::acquire() {
    // Set lock = TRUE e recebe o valor anterior
    // Caso lock anterior = TRUE  -> loop infinito
    // Caso lock anterior = FALSE -> lock adquirido
    while(lock.test_and_set(std::memory_order_acquire)) {}
}

void SpinLock::release() {
    // lock = FALSE
    lock.clear(std::memory_order_release);
}