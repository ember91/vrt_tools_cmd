#ifndef VRT_TRUNCATE_SRC_PROCESS_H_
#define VRT_TRUNCATE_SRC_PROCESS_H_

#include <cstdint>
#include <tuple>

namespace vrt::truncate {
struct ProgramArguments;
}

namespace vrt::truncate {

class Processor {
   public:
    Processor(const ProgramArguments& args);

    void process();

   private:
    std::tuple<uint64_t, uint64_t> calculate_begin_end() const;

    const ProgramArguments& program_args_;
};

}  // namespace vrt::truncate

#endif
