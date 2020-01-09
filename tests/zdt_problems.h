#ifndef ZDT_PROBLEMS_H
#define ZDT_PROBLEMS_H

#include <math.h>

#define PI		3.14159265358979323846

#include "../core/CProblem.h"
#include "../core/CFunction.h"
using namespace MEVO;

class CProblemWithFront{
public:
     virtual std::deque<std::deque<double> > front()=0;
};

class ZDT1_4_F1 : public CFunction {
public:
    ICFunction* copy(){return new ZDT1_4_F1();}
    const char* getDescription(){return "ZDT_F1";}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        double res=_pf[0];
        return res;
    }
};

class ZDT1_F2 : public CFunction {
public:
    ICFunction* copy(){return new ZDT1_F2();}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        double g = calculate_G(_pf,_nf);
        return g * (1.0 - sqrt (FProblem->getObjectiveFncValue(0)/g));
    }
    double calculate_G(double* _pf, unsigned int _nf) {
        double sum = 0;
        for (unsigned int i = 1; i != _nf; i++) {
            sum += _pf[i];
        }
        sum *= 9;
        sum /=(_nf-1);
        sum += 1;
        return sum;
    }
    const char * getDescription() {return "ZDT1_F2";}
};

class ZDT1 :  public CProblem, public CProblemWithFront {
public:
    ZDT1(unsigned int _varSize) : CProblem(){
        addObjectiveFnc(new ZDT1_4_F1);
        addObjectiveFnc(new ZDT1_F2);
        setRealVarSize(_varSize);
        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~ZDT1() {}

    std::deque<std::deque<double> > front();
    const char *getDescription(){return "ZDT1";}
};


class ZDT2_F2 : public CFunction {
public:
    ICFunction* copy(){return new ZDT2_F2();}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        double g = calculate_G(_pf,_nf);
        double h = 1.0 - pow(_pf[0]/g, 2.0);
        return g * h;
    }
    double calculate_G(double* _pf, unsigned int _nf) {
        double sum = 0;
        for (unsigned int i = 1; i != _nf; i++) {
            sum += _pf[i];
        }
        sum *= 9;
        sum /=(_nf-1);
        sum += 1;
        return sum;
    }
    const char * getDescription() {return "ZDT2_F2";}
};

class ZDT2 :  public CProblem, public CProblemWithFront {
public:
    ZDT2(unsigned int _varSize) : CProblem(){
        addObjectiveFnc(new ZDT1_4_F1);
        addObjectiveFnc(new ZDT2_F2);
        setRealVarSize(_varSize);
        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~ZDT2() {}

    std::deque<std::deque<double> > front();
    const char *getDescription(){return "ZDT2";}
};


class ZDT3_F2 :  public CFunction {
public:
    ICFunction* copy(){return new ZDT3_F2();}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        double g = calculate_G(_pf,_nf);
        double h = 1.0 - sqrt(_pf[0]/g)-_pf[0]/g*sin(10.0*PI*_pf[0]);
        return g * h;
    }

    double calculate_G(double* _pf, unsigned int _nf) {
        double sum = 0;
        for (unsigned int i = 1; i != _nf; i++) {
            sum += _pf[i];
        }
        sum *= 9;
        sum /=(_nf-1);
        sum += 1;
        return sum;
    }
    const char * getDescription() {return "ZDT3_F2";}
};


class ZDT3 : public CProblem, public CProblemWithFront {
public:
    ZDT3(unsigned int _varSize) : CProblem(){
        addObjectiveFnc(new ZDT1_4_F1);
        addObjectiveFnc(new ZDT3_F2);
        setRealVarSize(_varSize);
        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~ZDT3() {}

    std::deque<std::deque<double> > front();
    const char *getDescription(){return "ZDT3";}
};

class ZDT4_F2 : public CFunction {
public:
    ICFunction* copy(){return new ZDT4_F2();}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
        double g = calculate_G(_pf,_nf);
        double h = 1.0 - sqrt(_pf[0]/g);
        return g * h;
    }

    double calculate_G(double* _pf, unsigned int _nf) {
        double sum = 0;
        for (unsigned int i = 1; i != _nf; i++) {
            sum += _pf[i]*_pf[i]-10*cos(4*PI*_pf[i]);
        }
        sum += 1+10*(_nf-1);
        return sum;
    }
    const char * getDescription() {return "ZDT4_F2";}
};


class ZDT4 : public CProblem, public CProblemWithFront {
public:
    ZDT4(unsigned int _varSize) : CProblem(){
        addObjectiveFnc(new ZDT1_4_F1);
        addObjectiveFnc(new ZDT4_F2);
        setRealVarSize(_varSize);

        setRealLowLimit(0,0);
        setRealHighLimit(1,0);
        for(int i=1; i<FRealVarSize; i++) {
            setRealLowLimit(-5,i);
            setRealHighLimit(5,i);
        }
    }

    ~ZDT4() {}

    std::deque<std::deque<double> > front();
    const char *getDescription(){return "ZDT4";}
};

class ZDT5;

class ZDT5_F1 :  public CFunction {
public:
    ICFunction* copy(){return new ZDT5_F1;}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);
    const char * getDescription() {return "ZDT5_F1";}
};

class ZDT5_F2 :  public CFunction {
public:
    ICFunction* copy(){return new ZDT5_F2();}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);

    double v(int* _pi, unsigned int _ni, int varNo){
        int sum=0;
        for(int i=0; i<5; i++){
            sum+=_pi[30+5*(varNo-1)+i];
        }
        if (sum<5) {return 2+sum;}
        return 1;
    }

    double calculate_G(int* _pi, unsigned int _ni) {
        double sum = 0;
        for (unsigned int i = 1; i <11; i++) {
            sum += v(_pi, _ni,i);
        }
        return sum;
    }
    const char * getDescription() {return "ZDT5_F2";}
};

class ZDT5 : public CProblem, public CProblemWithFront {
public:
    ZDT5(unsigned int _varSize) : CProblem(){
        addObjectiveFnc(new ZDT5_F1);
        addObjectiveFnc(new ZDT5_F2);
        setIntegerVarSize(30+(_varSize-1)*5);
        for(int i=0; i<FIntegerVarSize; i++) {
            setIntegerLowLimit(0,i);
            setIntegerHighLimit(1,i);
        }
    }

    ~ZDT5() {}

    std::deque<std::deque<double> > front();
    const char *getDescription(){return "ZDT5";}
};



class ZDT6;

class ZDT6_F1 :  public CFunction {
public:
    ICFunction* copy(){return new ZDT6_F1();}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);

    const char * getDescription() {return "ZDT6_F1";}
};

class ZDT6_F2 :  public CFunction {
public:
    ICFunction* copy(){return new ZDT6_F2();}
    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);

    double calculate_G(double* _pf, unsigned int _nf) {
        double sum = 0;
        for (unsigned int i = 1; i != _nf; i++) {
            sum += _pf[i];
        }
        sum/=_nf-1;
        sum = pow(sum, 0.25);
        sum*=9;
        sum+=1;
        return sum;
    }
    const char * getDescription() {return "ZDT6_F2";}
};

class ZDT6 : public CProblem, public CProblemWithFront {
public:
    ZDT6(unsigned int _varSize) : CProblem(){
        addObjectiveFnc(new ZDT6_F1);
        addObjectiveFnc(new ZDT6_F2);
        setRealVarSize(_varSize);
        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~ZDT6() {}

    std::deque<std::deque<double> > front();
    const char *getDescription(){return "ZDT6";}
};

#endif // ZDT_PROBLEMS_H
