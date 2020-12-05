#include <fstream>
#include <string>

namespace vrt::merge {

class OutputStream {
   public:
    OutputStream(const std::string& file_path);

    std::ofstream& get_file() { return file_out_; }

   private:
    std::ofstream file_out_;
};

}  // namespace vrt::merge
