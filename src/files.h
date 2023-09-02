#ifndef FILES_H
#define FILES_H

#include <string>

namespace files {

    std::string name(std::string path);
    std::string full_name(std::string path);
    std::string extension(std::string path);

} // namespace files

#endif
