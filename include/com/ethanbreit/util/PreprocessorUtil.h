//
// Created by Ethan Breit on 2017-07-22.
//

#pragma once
#include <cstring>


#ifdef _WIN32

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#else

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#endif