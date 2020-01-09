#include "dtlz_problems.h"

std::deque<std::deque<double> > dtlz1front(unsigned int size) {
    std::deque<std::deque<double> > front;
    std::deque<double> d;
    double den =size*2;
    for (int x =0; x<=size; x++){
            for (int y=0; y<=x; y++) {
                std::deque<double> di;
                di.push_back((x-y)/den);
                di.push_back(x/den-di[0]);
                di.push_back(0.5-di[0]-di[1]);
                front.push_back(di);
            }
        }
    return front;
}



std::deque<std::deque<double> > dtlz2_4front(unsigned int size) {
    std::deque<std::deque<double> > front;
    std::deque<double> d;
    d.push_back(0);
    d.push_back(0);
    d.push_back(1);
    front.push_back(d);
        for (double j=1; j<size; j++){
            for (int i=0; i<=j; i++) {
                std::deque<double> di;
                double u= i/(double)j*PI/2;
                double v=(double)j/(size-1)*PI/2;
                di.push_back(cos(u)*sin(v));
                di.push_back(sin(u)*sin(v));
                di.push_back(cos(v));
                front.push_back(di);
            }
        }
    return front;
}

std::deque<std::deque<double> > dtlz5_6front(unsigned int size) {
    std::deque<std::deque<double> > front;
    std::deque<double> d;
    d.push_back(0);
    d.push_back(0);
    d.push_back(1);
    front.push_back(d);
            for (double j=1; j<size; j++){
                for (int i=j; i<=j; i++) {
                    std::deque<double> di;
                    double u= i/(double)j/2*PI/2;
                    double v=(double)j/(size-1)*PI/2;
                    di.push_back(cos(u)*sin(v));
                    di.push_back(sin(u)*sin(v));
                    di.push_back(cos(v));
                    front.push_back(di);
                }
            }
    return front;
}


double DTLZ1_F::calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
    double g;
    if(FNo==1) {
        g = calculate_G(_pf, _nf);
        FProblem->getVariables()->setReal("g", g);
    } else {
       g=FProblem->getVariables()->getReal("g");
    }

    double res =  0.5 * (1 + g);
    for (unsigned int j = FObj-FNo; j >= 1; j--) {
        res*=_pf[j-1];
    }
    if (FNo>1) {
        res*= 1-(_pf[FObj-FNo]);
    }
    return res;
}

double DTLZ2_F::calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
    double g;
    if(FNo==1) {
        g = calculate_G(_pf, _nf);
        FProblem->getVariables()->setReal("g", g);
    } else {
       g=FProblem->getVariables()->getReal("g");
    }

    double res =  (1 + g);
    for (unsigned int j = FObj-FNo; j >= 1; j--) {
        res*=cos(_pf[j-1]*PI/2);
    }
    if (FNo>1) {
        res*= sin(_pf[FObj-FNo]*PI/2);
    }
    return res;
}

double DTLZ3_F::calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
    double g;
    if(FNo==1) {
        g = calculate_G(_pf, _nf);
       FProblem->getVariables()->setReal("g", g);
    } else {
       g=FProblem->getVariables()->getReal("g");
    }

    double res =  1 + g;
    for (unsigned int j = FObj-FNo; j >= 1; j--) {
        res*=cos(_pf[j-1]*PI/2);
    }
    if (FNo>1) {
        res*= sin(_pf[FObj-FNo]*PI/2);
    }
    return res;
}

double DTLZ4_F::calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
    double g;
    std::deque<double> metaVar(_nf, 0.0);
    if(FNo==1) {
        double alpha = FProblem->getParameters()->getReal("alpha");
        for(int i=0; i<_nf; i++) {
            metaVar[i] = pow(_pf[i], alpha);
        }
        g = calculate_G(_pf, _nf);
       FProblem->getVariables()->setReal("g", g);
       FProblem->getVariables()->setReal("metaVar", metaVar);
    } else {
       g=FProblem->getVariables()->getReal("g");
       metaVar = FProblem->getVariables()->getRealVector("metaVar");
    }

    double res =  1 + g;
    for (unsigned int j = FObj-FNo; j >= 1; j--) {
        res*=cos(metaVar[j-1]*PI/2);
    }
    if (FNo>1) {
        res*= sin(metaVar[FObj-FNo]*PI/2);
    }
    return res;
}

double DTLZ5_F::calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
    double g;

    std::deque<double> metaVar(FObj-1, 0.0);

    if(FNo==1) {
        g = calculate_G(_pf, _nf);
        double t = PI /(4 * (1 + g));
        metaVar[0]=_pf[0]*PI/2;
        for(int i=2; i<FObj; i++) {
            metaVar[i-1] = t * (1+2*g*_pf[i-1]);
        }
        FProblem->getVariables()->setReal("g", g);
        FProblem->getVariables()->setReal("metaVar", metaVar);
     } else {
        g=FProblem->getVariables()->getReal("g");
        metaVar = FProblem->getVariables()->getRealVector("metaVar");
     }

    double res =  1 + g;
    for (unsigned int j = FObj-FNo; j >= 1; j--) {
        res*=cos(metaVar[j-1]);
    }
    if (FNo>1) {
        res*= sin(metaVar[FObj-FNo]);
    }
    return res;
}

double DTLZ6_F::calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
    double g;

    std::deque<double> metaVar(FObj-1, 0.0);


    if(FNo==1) {
        g = calculate_G(_pf, _nf);
        double t = PI /(4 * (1 + g));
        metaVar[0]=_pf[0]*PI/2;
        for(int i=2; i<FObj; i++) {
            metaVar[i-1] = t * (1+2*g*_pf[i-1]);
        }

        FProblem->getVariables()->setReal("g", g);
        FProblem->getVariables()->setReal("metaVar", metaVar);
     } else {
        g=FProblem->getVariables()->getReal("g");
        metaVar = FProblem->getVariables()->getRealVector("metaVar");
     }

    double res =  1 + g;
    for (unsigned int j = FObj-FNo; j >= 1; j--) {
        res*=cos(metaVar[j-1]);
    }
    if (FNo>1) {
        res*= sin(metaVar[FObj-FNo]);
    }
    return res;
}

double DTLZ7_F::calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
    double g;

    if(FNo==FObj) {
        double g=calculate_G(_pf, _nf);
        double h = calculate_H(_pf, _nf, g);
        return (1+g)*h;
    } else {
        return _pf[FNo-1];
    }

    return 0;
}
