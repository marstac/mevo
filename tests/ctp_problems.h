#ifndef CTP_PROBLEMS_H
#define CTP_PROBLEMS_H

#include <math.h>

#include "../core/CFunction.h"
#include "../core/CProblem.h"

#include "zdt_problems.h"
#include <windows.h>

namespace MEVO {
class CTP1_7_F1 : public CFunction {
public:
    ICFunction* copy(){return new CTP1_7_F1();}
    const char* getDescription(){return "CTP_F1";}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        double res=_pf[0];
        return res;
    }
};

class CTP1_F2 : public CFunction {
public:
    ICFunction* copy(){return new CTP1_F2();}
    const char* getDescription(){return "CTP_F2";}

    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        double g=1.0;
        for (unsigned int i=1; i<_nf;i++){
            g+=sqrt(_pf[i]);
        }
        double f1=FProblem->getObjectiveFncValue(0);

        double res=g*exp(-f1/g);
        return res;
    }
};

class CTP1_C : public CFunction {
public:
    CTP1_C(int _no, int _all) : CFunction(){
        a=1;
        b=1;
        double delta = 1.0/(_all+1);
        double x=delta;
        double y;
        for (int j=0; j<_no; j++){
            y=a*exp(-b*x);
            a=(a+y)/2;
            b=-1.0/x*log(y/a);
            x+=delta;
        }
    }

     CTP1_C(CTP1_C &_src){
         a=_src.a;
         b=_src.b;
     }

    ICFunction* copy(){ return new CTP1_C(*this);}
    const char* getDescription(){return "CTP_C";}

    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        double f1=FProblem->getObjectiveFncValue(0);
        double f2=FProblem->getObjectiveFncValue(1);
        double res=-1*f2/(a*exp(-b*f1))+1.0;
        return res;
    }

    double a;
    double b;
};

class CTP1 : public CProblem, public CProblemWithFront {
public:
    CTP1(int _vs, int _cs) : CProblem() {
        setRealVarSize(_vs);

        addObjectiveFnc(new CTP1_7_F1);
        addObjectiveFnc(new CTP1_F2);

        for (int i=0; i<_cs; i++) {
            addConstraintFnc(new CTP1_C(i+1, _cs));
        }


        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~CTP1() {}
    std::deque<std::deque<double> > front();
    const char *getDescription(){return "CTP1";}
};


class CTP2_7_F2 : public CFunction {
public:
    ICFunction* copy(){return new CTP2_7_F2();}
    const char* getDescription(){return "CTP_F2";}

    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        double g=1.0;
        for (unsigned int i=1; i<_nf;i++){
            g+=_pf[i];
        }
        double f1=FProblem->getObjectiveFncValue(0);

        double res=g*(1-sqrt(f1/g));
        return res;
    }

};

class CTP2_7_C : public CFunction {
public:
    CTP2_7_C() : CFunction() {

    }
    ICFunction* copy(){return new CTP2_7_C();}
    const char* getDescription(){return "CTP_C2_7";}

    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        double f1=FProblem->getObjectiveFncValue(0);
        double f2=FProblem->getObjectiveFncValue(1);

        double a = FProblem->getParameters()->getReal("a");
        double b = FProblem->getParameters()->getReal("b");
        double c = FProblem->getParameters()->getReal("c");
        double d = FProblem->getParameters()->getReal("d");
        double e = FProblem->getParameters()->getReal("e");

        double theta =FProblem->getParameters()->getReal("theta");

        double exp1 = (f2-e)*cos(theta) - f1*sin(theta);
        double exp2 = (f2-e)*sin(theta) + f1*cos(theta);
        exp2 = b*PI*pow(exp2,c);
        exp2 = fabs(sin(exp2));
        exp2 = a*pow(exp2,d);
        double res=exp2-exp1;
        return res;
    }


};

class CTP2_7 : public CProblem, public CProblemWithFront {
public:
    enum enPN {CTP2, CTP3, CTP4, CTP5, CTP6, CTP7};
    CTP2_7(int _vs, enPN _pn) : CProblem() {
        if (_pn==CTP2) {
           setParameters(0.2, 10.0, 1.0, 6.0, 1.0 , -0.2*PI);
           FName = "CTP2";
        } else if (_pn==CTP3) {
           setParameters(0.1, 10.0, 1.0, 0.5, 1.0 , -0.2*PI);
           FName = "CTP3";
        } else if (_pn==CTP4) {
           setParameters(0.75, 10.0, 1.0, 0.5, 1.0 , -0.2*PI);
           FName = "CTP4";
        } else if (_pn==CTP5) {
           setParameters(0.1, 10.0, 2.0, 0.5, 1.0 , -0.2*PI);
           FName = "CTP5";
        } else if (_pn==CTP6) {
           setParameters(40, 0.5, 1.0, 2.0, -2.0 , 0.1*PI);
           FName = "CTP6";
        } else if (_pn==CTP7) {
           setParameters(40, 5, 1.0, 6, 0 , -0.05*PI);
           FName = "CTP7";
        }


        setRealVarSize(_vs);

        addObjectiveFnc(new CTP1_7_F1);
        addObjectiveFnc(new CTP2_7_F2);

        addConstraintFnc(new CTP2_7_C());

        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    void setParameters(double _a, double _b, double _c, double _d, double _e, double _theta){
        FParameters->add("a", _a);
        FParameters->add("b", _b);
        FParameters->add("c", _c);
        FParameters->add("d", _d);
        FParameters->add("e", _e);
        FParameters->add("theta", _theta);
    }
    std::string FName;

    ~CTP2_7() {}
    std::deque<std::deque<double> > front();
    const char *getDescription(){return FName.c_str();}
};


}



#endif // CTP_PROBLEMS_H
