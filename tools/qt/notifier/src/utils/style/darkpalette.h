#pragma once



#include <QPalette>



class DarkPalette : public QPalette
{
public:
    DarkPalette();
    ~DarkPalette();

    DarkPalette(const DarkPalette& another)            = delete;
    DarkPalette& operator=(const DarkPalette& another) = delete;
};
