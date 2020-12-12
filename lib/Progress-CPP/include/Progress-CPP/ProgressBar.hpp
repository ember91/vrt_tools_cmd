
#pragma once

#include <chrono>

namespace progresscpp {

class ProgressBar {
   public:
    ProgressBar(size_t total, size_t width, char complete, char incomplete)
        : total_ticks{total}, bar_width{width}, complete_char{complete}, incomplete_char{incomplete} {}

    ProgressBar(size_t total, size_t width) : total_ticks{total}, bar_width{width} {}

    size_t operator++() { return ++ticks; }

    void operator+=(size_t amount) { ticks += amount; }

    size_t get_ticks() const { return ticks; }

    void display() const;

    void done();

   private:
    size_t ticks = 0;

    const size_t                                total_ticks;
    const unsigned int                          bar_width;
    const char                                  complete_char   = '=';
    const char                                  incomplete_char = ' ';
    const std::chrono::steady_clock::time_point start_time      = std::chrono::steady_clock::now();
};

}  // namespace progresscpp
