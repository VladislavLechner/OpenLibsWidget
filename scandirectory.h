#pragma once

#include <dlfcn.h>
#include <stdio.h>
#include <iostream>
#include <exception>
#include <boost/filesystem.hpp>

class ScanDirectory
{
public:
    ScanDirectory() = default;
    bool checkPath(std::string path);

    std::list<std::string> libsPaths() const;

private:
    std::list<std::string> m_libsPaths;
private:
    void startScaning(std::string path);
    bool dlInfo(std::string path);
};


