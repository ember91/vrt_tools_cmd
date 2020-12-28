#ifndef LIB_PROGRESS_CPP_INCLUDE_PROGRESS_CPP_PROGRESS_BAR_H_
#define LIB_PROGRESS_CPP_INCLUDE_PROGRESS_CPP_PROGRESS_BAR_H_

#include <chrono>
#include <cstdint>

namespace progresscpp {

class ProgressBar {
   public:
    ProgressBar(uint64_t total, unsigned int width, char complete, char incomplete)
        : total_ticks{total}, bar_width{width}, complete_char{complete}, incomplete_char{incomplete} {}

    ProgressBar(uint64_t total, unsigned int width) : total_ticks{total}, bar_width{width} {}

    uint64_t operator++() { return ++ticks; }

    void operator+=(uint64_t amount) { ticks += amount; }

    uint64_t get_ticks() const { return ticks; }

    void display() const;

    void done();

    void reset();

   private:
    const uint64_t                              total_ticks;
    const unsigned int                          bar_width;
    const char                                  complete_char   = '=';
    const char                                  incomplete_char = ' ';
    const std::chrono::steady_clock::time_point start_time      = std::chrono::steady_clock::now();

    uint64_t ticks = 0;
};

}  // namespace progresscpp

#endif
