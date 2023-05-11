#include <atomic>

class SpinLock {

    public:
        void acquire();
        void release();

    private:
        // Inicializa lock = FALSE
        std::atomic_flag lock = ATOMIC_FLAG_INIT;
};