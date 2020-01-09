#ifndef SCHAFFER_H
#define SCHAFFER_H

#include "../core/CFunction.h"
#include "../core/CProblem.h"

#include "../algorithms/nsga2/NSGA2.h"
#include "../algorithms/amga2/AMGA2.h"

#include "../indicators/CSweepHVIndicator.h"

using namespace MEVO;

class F1 : public CFunction  {
public:
    ICFunction* copy(){return new F1();}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        return _pf[0]*_pf[0];
  }
};

class F2 : public CFunction  {
public:
    ~F2(){}
    ICFunction* copy(){return new F2();}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        return (_pf[0] - 2.0) * (_pf[0] - 2.0);
  }
};

class C1 : public CFunction {
public:
    ~C1(){}
    ICFunction* copy(){return new C1();}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        return - ((_pf[0] - 1) * (_pf[0] -1)) + 0.04;
    }
};

void solve_SCH();
#endif // SCHAFFER_H
