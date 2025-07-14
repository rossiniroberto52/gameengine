#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <string>

namespace FileUtils {

inline bool createDirectoryIfNotExists(const std::string& path) {
#ifdef _WIN32
    if (_mkdir(path.c_str()) == 0) {
        return true;
    }
    return false;
#else
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        if (mkdir(path.c_str(), 0755) == 0) {
            return true;
        } else {
            return false;
        }
    }
    return true;
#endif
}

} // namespace FileUtils

#endif // FILE_UTILS_H
