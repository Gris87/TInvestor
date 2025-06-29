#pragma once



#include <QString>



#ifdef TESTING_MODE
extern QString lastThrownException;
#endif



void throwException(const char* message);
