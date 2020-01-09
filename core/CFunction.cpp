#include "CFunction.h"
namespace MEVO {
MEVO::CFunction::CFunction()
{

}

MEVO::CFunction::CFunction(MEVO::ICProblem *_p)
{
    FProblem=_p;
}

double MEVO::CFunction::evaluate(double *_pf, unsigned int _nf, int *_pi, unsigned int _ni)
{
    double res = calculate(_pf, _nf, _pi, _ni);
    if(FProblem){
        FProblem->update(res);
    }
    return res;
}

const char *CFunction::getDescription()
{
    return "Generic function";
}


void CFunction::setProblem(ICProblem *_p)
{
    FProblem = _p;
}
}
