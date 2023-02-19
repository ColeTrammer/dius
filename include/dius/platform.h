#pragma once

#include <di/container/allocator/allocator.h>
#include <di/container/allocator/forward_declaration.h>

#ifdef __linux__
#include <pthread.h>
#endif

namespace di::sync {
class DumbSpinlock;
}

namespace di::platform {
#ifdef __linux__
using ThreadId = pthread_t;

inline ThreadId get_current_thread_id() {
    return pthread_self();
}
#else
using ThreadId = i32;

inline ThreadId get_current_thread_id() {
    return 0;
}
#endif

using DefaultLock = sync::DumbSpinlock;

template<typename T>
using DefaultAllocator = container::Allocator<T>;
}