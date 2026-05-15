// Copyright (c) 2026
// Distributed under the MIT License.
// See the LICENSE file in the project root for more information.

#pragma once

#include <string>

#include "utils/filesystem.h"

namespace netcoredbg
{
namespace HotReloadPath
{

inline bool IsAbsolutePath(const std::string &path)
{
    if (path.empty())
        return false;

    if (path[0] == '/' || path[0] == '\\')
        return true;

    return path.size() > 2 && path[1] == ':' && (path[2] == '\\' || path[2] == '/');
}

inline std::string GetDirectoryName(const std::string &path)
{
    std::size_t i = path.find_last_of("/\\");
    if (i == std::string::npos)
        return std::string();

    if (i == 0)
        return path.substr(0, 1);

    return path.substr(0, i);
}

inline std::string Combine(const std::string &directory, const std::string &fileName)
{
    if (directory.empty())
        return fileName;

    char last = directory[directory.size() - 1];
    if (last == '/' || last == '\\')
        return directory + fileName;

#ifdef _WIN32
    return directory + "\\" + fileName;
#else
    return directory + "/" + fileName;
#endif
}

inline std::string GetStartupHookPath()
{
#ifdef NCDB_DOTNET_STARTUP_HOOK
    static const std::string hookPath = []() {
        std::string path(NCDB_DOTNET_STARTUP_HOOK);
        if (IsAbsolutePath(path))
            return path;

        return Combine(GetDirectoryName(GetExeAbsPath()), path);
    }();
    return hookPath;
#else
    return std::string();
#endif
}

} // namespace HotReloadPath
} // namespace netcoredbg
