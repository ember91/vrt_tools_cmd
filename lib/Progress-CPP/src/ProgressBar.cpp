#include "Progress-CPP/ProgressBar.hpp"

#include <chrono>
#include <iostream>

namespace progresscpp {

void ProgressBar::display() const {
    float progress = total_ticks == 0 ? 1.0F : static_cast<float>(ticks) / total_ticks;
    int   pos      = static_cast<int>(bar_width * progress);

    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

    std::cout << "[";

    for (unsigned int i{0}; i < bar_width; ++i) {
        if (i < pos) {
            std::cout << complete_char;
        } else if (i == pos) {
            std::cout << ">";
        } else {
            std::cout << incomplete_char;
        }
    }
    std::cout << "] " << int(progress * 100.0) << "% " << float(time_elapsed) / 1000.0 << "s\r";
    std::cout.flush();
}

void ProgressBar::done() {
    ticks = total_ticks;
    display();
    std::cout << std::endl;
}

void ProgressBar::reset() {
    ticks = 0;
    display();
}

}  // namespace progresscpp
