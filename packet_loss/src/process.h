#ifndef VRT_PACKET_LOSS_SRC_PROCESS_H_
#define VRT_PACKET_LOSS_SRC_PROCESS_H_

#include <random>

namespace vrt::packet_loss {
struct ProgramArguments;
}

namespace vrt::packet_loss {

class Processor {
   public:
    Processor(const ProgramArguments& args);

    void process();

   private:
    const ProgramArguments& program_args_;

    bool lost();

    std::random_device               rd_;
    std::mt19937                     e2_;
    std::uniform_real_distribution<> dist_;

    bool prev_lost_{false};
};

}  // namespace vrt::packet_loss

#endif
