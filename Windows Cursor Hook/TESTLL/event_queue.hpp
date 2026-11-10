#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <mutex>

struct MouseEvent {
    std::uint32_t message = 0;
    std::int32_t x = 0;
    std::int32_t y = 0;
    std::uint32_t data = 0;
};

// Fixed storage: the hook never allocates memory or waits for the logger's lock.
class EventQueue {
public:
    static constexpr std::size_t capacity = 256;

    bool try_push(const MouseEvent& event) noexcept {
        if (!mutex_.try_lock()) return false;
        const std::lock_guard<std::mutex> lock(mutex_, std::adopt_lock);
        if (size_ == capacity) return false;
        events_[(front_ + size_) % capacity] = event;
        ++size_;
        return true;
    }

    bool pop(MouseEvent& event) {
        const std::lock_guard<std::mutex> lock(mutex_);
        if (size_ == 0) return false;
        event = events_[front_];
        front_ = (front_ + 1) % capacity;
        --size_;
        return true;
    }

private:
    std::mutex mutex_;
    std::array<MouseEvent, capacity> events_{};
    std::size_t front_ = 0;
    std::size_t size_ = 0;
};
