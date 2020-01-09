#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <limits>

namespace MEVO {
int round( double fValue );

const double    DOUBLE_MAX = std::numeric_limits<double>::max();
const double    DOUBLE_MIN = -1*std::numeric_limits<double>::max();

const double    DOUBLE_EPS = -1*std::numeric_limits<double>::min();

const int       INTEGER_MAX = std::numeric_limits<int>::max();
const int       INTEGER_MIN = -1*std::numeric_limits<int>::max();
}


#endif // FUNCTIONS_H
