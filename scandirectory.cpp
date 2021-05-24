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

std::pair< std::list<std::string>, std::list<std::string>> ScanDirectory::libsPaths() const
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
        if (it->path().string().substr(pos, it->path().string().size() - pos) == ".so")
        {
            m_libsPaths.first.push_back(it->path().string());
            dlInfo(it->path().string());
        }
    }
}

bool ScanDirectory::dlInfo(std::string path)
{
    void* lib;
    lib = dlopen(path.c_str(), RTLD_NOW);
    if (lib == nullptr)
    {
        throw std::runtime_error(dlerror());
//        std::cerr << "Cannot load library: " << dlerror() << '\n';
        return false;
    }
    dlerror();

    typedef std::string *(*GetInfo)();
    GetInfo info = nullptr;

    info = reinterpret_cast<GetInfo>(dlsym(lib, "getInfo")); // приводим к указателю на фукцнию
    if (info == nullptr)
    {
        throw std::runtime_error(dlerror());
//        std::cerr << "Cannot load create function: " << dlerror() << '\n';
        return false;
    }
    dlerror();

    std::string* inf = info();

     m_libsPaths.second.push_back(*inf);

    delete inf;

    dlclose(lib);

    return true;
}
