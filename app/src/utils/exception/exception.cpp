#include "src/utils/exception/exception.h"

#include <stdexcept>



#ifdef TESTING_MODE
QString lastThrownException; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
#endif



void throwException(const char* message)
{
#ifndef TESTING_MODE
    throw std::runtime_error(message);
#else
    lastThrownException = QString::fromUtf8(message);
#endif
}
