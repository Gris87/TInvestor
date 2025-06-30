#pragma once



#include <QString>



#ifdef TESTING_MODE
extern QString lastThrownException; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
#endif



void throwException(const char* message);
