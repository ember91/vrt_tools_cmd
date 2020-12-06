#include <fstream>
#include <string>

namespace vrt::merge {

/**
 * Output stream.
 */
class OutputStream {
   public:
    OutputStream(std::string file_path);

    /**
     * Get reference to file.
     *
     * \return Output file.
     */
    std::ofstream& get_file() { return file_; }

    void remove_file();

   private:
    const std::string file_path_;

    std::ofstream file_;
};

}  // namespace vrt::merge
