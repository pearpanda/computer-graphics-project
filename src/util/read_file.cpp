#include <rg/util/read_file.hpp>

#include <string>
#include <fstream>
#include <sstream>

namespace rg::util {

std::string readFile(const std::string& path) {
    std::fstream file{path};
    std::stringstream medium{};
    medium << file.rdbuf();
    return medium.str();
}

} // namespace rg::util
