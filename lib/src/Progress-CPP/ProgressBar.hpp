
#pragma once

#include <chrono>
#include <iostream>

namespace progresscpp {
class ProgressBar {
   private:
    size_t ticks = 0;

    const size_t                                total_ticks;
    const unsigned int                          bar_width;
    const char                                  complete_char   = '=';
    const char                                  incomplete_char = ' ';
    const std::chrono::steady_clock::time_point start_time      = std::chrono::steady_clock::now();

   public:
    ProgressBar(size_t total, size_t width, char complete, char incomplete)
        : total_ticks{total}, bar_width{width}, complete_char{complete}, incomplete_char{incomplete} {}

    ProgressBar(size_t total, size_t width) : total_ticks{total}, bar_width{width} {}

    size_t operator++() { return ++ticks; }

    void operator+=(size_t amount) { ticks += amount; }

    size_t get_ticks() const { return ticks; }

    void display() const {
        float progress = total_ticks == 0 ? 1.0F : static_cast<float>(ticks) / total_ticks;
        int   pos      = (int)(bar_width * progress);

        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

        std::cout << "[";

        for (int i = 0; i < bar_width; ++i) {
            if (i < pos)
                std::cout << complete_char;
            else if (i == pos)
                std::cout << ">";
            else
                std::cout << incomplete_char;
        }
        std::cout << "] " << int(progress * 100.0) << "% " << float(time_elapsed) / 1000.0 << "s\r";
        std::cout.flush();
    }

    void done() {
        ticks = total_ticks;
        display();
        std::cout << std::endl;
    }
};
}  // namespace progresscpp
