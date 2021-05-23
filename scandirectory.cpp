#include "scandirectory.h"

bool ScanDirectory::checkPath(std::string path)
{
    if (!boost::filesystem::exists(path))
    {
        throw std::runtime_error("Неверный путь");
        return false;
    }
    else if (!boost::filesystem::is_directory(path))
    {
        throw std::runtime_error("Переданный путь не является директорией");
        return false;
    }
    else if (path.empty())
    {
        throw std::runtime_error("Переданный путь пуст");
        return false;
    }
    startScaning(path);
    return true;
}

std::list<std::string> ScanDirectory::libsPaths() const
{
    return m_libsPaths;
}

void ScanDirectory::startScaning(std::string path)
{
    for (boost::filesystem::recursive_directory_iterator it(path); it != boost::filesystem::recursive_directory_iterator(); ++it)
    {
        size_t pos = it->path().string().rfind(".");
        if (pos > it->path().string().size())
            continue;
        std::string temp = it->path().string().substr(pos, it->path().string().size() - pos);
        std::string t = it->path().string();
        if (it->path().string().substr(pos, it->path().string().size() - pos) == ".so")
        {
            dlInfo(it->path().string());
            m_libsPaths.push_back(it->path().string());
        }

    }
}

bool ScanDirectory::dlInfo(std::string path)
{
    void* lib;
    lib = dlopen(path.c_str(), RTLD_NOW);
    if (lib == nullptr)
    {
        std::cout << "Cannot load library: " << dlerror() << '\n';
        return false;
    }
    dlerror();

//    void* info;

//    info = dlsym(lib,)

    return true;
}
