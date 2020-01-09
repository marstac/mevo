#ifndef DTLZ_PROBLEMS_H
#define DTLZ_PROBLEMS_H

#include <math.h>

#include "../core/CProblem.h"
#include "../core/CFunction.h"

#include "zdt_problems.h"

using namespace MEVO;

std::deque<std::deque<double> > dtlz1front(unsigned int size);
std::deque<std::deque<double> > dtlz2_4front(unsigned int size);
std::deque<std::deque<double> > dtlz5_6front(unsigned int size);
class DTLZ1;

class DTLZ_F : public CFunction {
public:
    DTLZ_F(unsigned int _no, unsigned int _nobj, ICProblem* _p) :CFunction(_p) {
       FNo=_no; FObj=_nobj;
    }

    ~DTLZ_F(){}
    unsigned int FNo;
    unsigned int FObj;
};

class DTLZ_P : public CProblem, public CProblemWithFront {
    public:
    DTLZ_P(int _m, int _k) : CProblem(){
        FVariables->add("g", 0.0);
        setRealVarSize(_m+_k-1);
    }
};



class DTLZ1_F : public DTLZ_F {
public:
    ICFunction* copy(){
        return new DTLZ1_F(FNo, FObj,NULL);
    }
    DTLZ1_F(unsigned int _no, unsigned int _nobj, ICProblem* _p) : DTLZ_F(_no, _nobj,_p) {}

    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);

    double calculate_G(double* _pf, unsigned int _nf) {
        double k=_nf-FObj+1;
        double g = 0;
        for (unsigned int i = _nf-k+1; i <= _nf; i++) {
            g += pow(_pf[i-1] - 0.5, 2) - cos(20 * PI * (_pf[i-1] - 0.5));
        }
        g=100*(k+g);
        return g;
    }

    const char * getDescription() {return "DTLZ1";}

};

class DTLZ1 : public DTLZ_P {
public:
    DTLZ1(unsigned int _m, unsigned int _k) : DTLZ_P(_m, _k){
        for(int i=1; i<=_m;i++) {
             DTLZ1_F* f=new DTLZ1_F(i,_m,this);
             addObjectiveFnc(f);
        }

        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~DTLZ1() {}
    std::deque<std::deque<double> > front() {return dtlz1front(15);}

    const char *getDescription(){return "DTLZ1";}
};

class DTLZ2;

class DTLZ2_F : public DTLZ_F {
public:
    ICFunction* copy(){
        return new DTLZ2_F(FNo, FObj,NULL);
    }
    DTLZ2_F(unsigned int _no, unsigned int _nobj, ICProblem* _p) : DTLZ_F(_no, _nobj,_p){}


    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);

    double calculate_G(double* _pf, unsigned int _nf) {
        double k=_nf-FObj+1;
        double g = 0;
        for (unsigned int i = _nf-k+1; i <= _nf; i++) {
            g += pow(_pf[i-1] - 0.5, 2);
        }
        return g;
    }

    const char * getDescription() {return "DTLZ2";}

};

class DTLZ2 : public DTLZ_P {
public:
    DTLZ2(unsigned int _m, unsigned int _k) : DTLZ_P(_m, _k){

        for(int i=1; i<=_m;i++) {
             DTLZ2_F* f=new DTLZ2_F(i,_m,this);
             addObjectiveFnc(f);
        }

        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~DTLZ2() {}
    std::deque<std::deque<double> > front() {return dtlz2_4front(50);}
    const char *getDescription(){return "DTLZ2";}
};

class DTLZ3;

class DTLZ3_F : public DTLZ_F {
public:
    ICFunction* copy(){
        return new DTLZ3_F(FNo, FObj,NULL);
    }
    DTLZ3_F(unsigned int _no, unsigned int _nobj, ICProblem* _p) :DTLZ_F(_no, _nobj, _p) {}

    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);

    double calculate_G(double* _pf, unsigned int _nf) {
        double k=_nf-FObj+1;
        double g = 0;
        for (unsigned int i = _nf-k+1; i <= _nf; i++) {
            g += pow(_pf[i-1] - 0.5, 2) - cos(20 * PI * (_pf[i-1] - 0.5));
        }
        g=100*(k+g);
        return g;
    }

    const char * getDescription() {return "DTLZ3";}

};

class DTLZ3 : public DTLZ_P {
public:
    DTLZ3(unsigned int _m, unsigned int _k) :DTLZ_P(_m, _k){

        for(int i=1; i<=_m;i++) {
             DTLZ3_F* f=new DTLZ3_F(i,_m,this);
             addObjectiveFnc(f);
        }

        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~DTLZ3() {}


    std::deque<std::deque<double> > front() {return dtlz2_4front(50);}
    const char *getDescription(){return "DTLZ3";}
};

class DTLZ4;

class DTLZ4_F : public DTLZ_F {
public:
    ICFunction* copy(){
        return new DTLZ4_F(FNo, FObj,NULL);
    }
    DTLZ4_F(unsigned int _no, unsigned int _nobj, ICProblem* _p) :DTLZ_F(_no, _nobj, _p){}

    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);

    double calculate_G(double* _pf, unsigned int _nf) {
        double k=_nf-FObj+1;
        double g = 0;
        for (unsigned int i = _nf-k+1; i <= _nf; i++) {
            g += pow(_pf[i-1] - 0.5, 2);
        }
        return g;
    }

   const char * getDescription() {return "DTLZ4";}

};

class DTLZ4 : public DTLZ_P {
public:
    DTLZ4(unsigned int _m, unsigned int _k) : DTLZ_P(_m, _k){
        FParameters->add("alpha", 100.0);

        FVariables->add("metaVar", _m, 0.0);

        for(int i=1; i<=_m;i++) {
             DTLZ4_F* f=new DTLZ4_F(i,_m,this);
             addObjectiveFnc(f);
        }

        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~DTLZ4() {}


    std::deque<std::deque<double> > front() {return dtlz2_4front(50);}
    const char *getDescription(){return "DTLZ4";}
};

class DTLZ5;

class DTLZ5_F : public DTLZ_F {
public:
    ICFunction* copy(){
        return new DTLZ5_F(FNo, FObj,NULL);
    }
    DTLZ5_F(unsigned int _no, unsigned int _nobj, ICProblem* _p) : DTLZ_F(_no, _nobj, _p){}

    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);

    double calculate_G(double* _pf, unsigned int _nf) {
        double k=_nf-FObj+1;
        double g = 0;
        for (unsigned int i = _nf-k+1; i <= _nf; i++) {
            g += pow(_pf[i-1]-0.5, 2);
        }
        return g;
    }

    const char * getDescription() {return "DTLZ5";}

};

class DTLZ5 : public DTLZ_P {
public:
    DTLZ5(unsigned int _m, unsigned int _k) : DTLZ_P(_m, _k){

        FVariables->add("metaVar", _m, 0.0);

        for(int i=1; i<=_m;i++) {
             DTLZ5_F* f=new DTLZ5_F(i,_m,this);
             addObjectiveFnc(f);
        }

        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~DTLZ5() {}
    std::deque<std::deque<double> > front() {return dtlz5_6front(1000);}
    const char *getDescription(){return "DTLZ5";}
};

// DTLZ6
class DTLZ6;

class DTLZ6_F : public DTLZ_F {
public:
    ICFunction* copy(){
        return new DTLZ6_F(FNo, FObj,NULL);
    }
    DTLZ6_F(unsigned int _no, unsigned int _nobj, ICProblem* _p) : DTLZ_F(_no, _nobj, _p){}

    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);

    double calculate_G(double* _pf, unsigned int _nf) {
        double k=_nf-FObj+1;
        double g = 0;
        for (unsigned int i = _nf-k+1; i <= _nf; i++) {
            g += pow(_pf[i-1], 0.1);
        }
        return g;
    }

    const char * getDescription() {return "DTLZ6";}

};

class DTLZ6 : public DTLZ_P {
public:
    DTLZ6(unsigned int _m, unsigned int _k) : DTLZ_P(_m,_k){

        FVariables->add("metaVar", _m, 0.0);

        for(int i=1; i<=_m;i++) {
             DTLZ6_F* f=new DTLZ6_F(i,_m,this);
             addObjectiveFnc(f);
        }

        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~DTLZ6() {}
    std::deque<std::deque<double> > front() {return dtlz5_6front(1000);}
    const char *getDescription(){return "DTLZ6";}
};




class DTLZ7;

class DTLZ7_F : public DTLZ_F {
public:
    ICFunction* copy(){
        return new DTLZ7_F(FNo, FObj,NULL);
    }
    DTLZ7_F(unsigned int _no, unsigned int _nobj, ICProblem* _p) : DTLZ_F(_no, _nobj, _p){}

    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);

    double calculate_G(double* _pf, unsigned int _nf) {
        double k=_nf-FObj+1;
        double g = 0;
        for (unsigned int i = _nf-k+1; i <= _nf; i++) {
            g += _pf[i-1];
        }
        g*=9.0/k;
        g+=1;
        return g;
    }

    double calculate_H(double* _pf, unsigned int _nf, double _g) {
        double h = 0;
        for (unsigned int i = 0; i < FObj-1; i++){
            h+=_pf[i]/(1.0+_g)*(1+sin(3*PI*_pf[i]));
        }
        h=FObj-h;
        return h;
    }

    const char * getDescription() {return "DTLZ7";}

};

class DTLZ7 : public DTLZ_P {
public:
    DTLZ7(unsigned int _m, unsigned int _k) :DTLZ_P(_m, _k){

        for(int i=1; i<=_m;i++) {
             DTLZ7_F* f=new DTLZ7_F(i,_m,this);
             addObjectiveFnc(f);
        }

        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~DTLZ7() {}

    const char *getDescription(){return "DTLZ7";}
};

class DTLZ8;

class DTLZ8_F : public DTLZ_F {
public:
    ICFunction* copy(){
        return new DTLZ8_F(FNo, FObj,NULL);
    }
    DTLZ8_F(unsigned int _no, unsigned int _nobj, ICProblem* _p) : DTLZ_F(_no, _nobj, _p){}


    double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni);

    const char * getDescription() {return "DTLZ8";}

};

class DTLZ8 : public DTLZ_P {
public:
    DTLZ8(unsigned int _m, unsigned int _k) : DTLZ_P(_m, _k){

        for(int i=1; i<=_m;i++) {
             DTLZ8_F* f=new DTLZ8_F(i,_m,this);
             addObjectiveFnc(f);
        }

        for(int i=0; i<FRealVarSize; i++) {
            setRealLowLimit(0,i);
            setRealHighLimit(1,i);
        }
    }

    ~DTLZ8() {}


    const char *getDescription(){return "DTLZ8";}
};

#endif // TEST_FUNCTIONS_H
