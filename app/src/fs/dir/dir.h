#pragma once



#include "src/fs/dir/idir.h"



class Dir : public IDir
{
public:
    Dir();
    ~Dir();

    Dir(const Dir& another)            = delete;
    Dir& operator=(const Dir& another) = delete;
};
