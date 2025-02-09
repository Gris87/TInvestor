#pragma once



class IFile
{
public:
    IFile()
    {
    }
    virtual ~IFile() = default;

    IFile(const IFile& another)            = delete;
    IFile& operator=(const IFile& another) = delete;
};
