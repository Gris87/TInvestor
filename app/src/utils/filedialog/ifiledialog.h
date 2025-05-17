#pragma once



class IFileDialog
{
public:
    IFileDialog()          = default;
    virtual ~IFileDialog() = default;

    IFileDialog(const IFileDialog& another)            = delete;
    IFileDialog& operator=(const IFileDialog& another) = delete;
};
