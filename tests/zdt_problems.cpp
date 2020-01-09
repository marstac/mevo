#include "zdt_problems.h"

double ZDT5_F1::calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
    int sum=1;
    for (int i=0; i<30; i++) {
        sum+=_pi[i];
    }
    return sum;
}

double ZDT5_F2::calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
    double g = calculate_G(_pi,_ni);
    double h = 1.0/FProblem->getObjectiveFncValue(0);
    return g * h;
}

double ZDT6_F1::calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
    double res=1-exp(-4*_pf[0])*pow(sin(6*PI*_pf[0]),6);
    return res;
}

double ZDT6_F2::calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni) {
    double g = calculate_G(_pf,_nf);
    double f1=FProblem->getObjectiveFncValue(0);
    double h = 1.0-(f1/g)*(f1/g);
    return g * h;
}

std::deque<std::deque<double> > ZDT1::front() {
   std::deque<std::deque<double> > rf;
   for (unsigned int i=0; i<= 1000; i++) {
       std::deque<double> ri;
       double f1=(1.0/1000)*i;
       double f2 = 1 - sqrt(f1/1.0);
       ri.push_back(f1);
       ri.push_back(f2);
       rf.push_back(ri);
   }
   return rf;
}

std::deque<std::deque<double> > ZDT2::front() {
   std::deque<std::deque<double> > rf;
   for (unsigned int i=0; i<= 1000; i++) {
       std::deque<double> ri;
       double f1=(1.0/1000)*i;
       double f2 = 1 - (f1/1.0)*(f1/1.0);
       ri.push_back(f1);
       ri.push_back(f2);
       rf.push_back(ri);
   }
   return rf;
}

std::deque<std::deque<double> > ZDT3::front() {
   std::deque<std::deque<double> > rf;
   double lm=1;
   for (unsigned int i=0; i<= 1000; i++) {
       std::deque<double> ri;
       double f1=(1.0/1000)*i;
       double f2 = 1 - sqrt(f1)-f1/1*sin(10.0*3.14159265358979323846*f1);
       if (f2 <= lm) {
           ri.push_back(f1);
           ri.push_back(f2);
           rf.push_back(ri);
           lm=f2;
       }
   }
   return rf;
}

std::deque<std::deque<double> > ZDT4::front() {
   std::deque<std::deque<double> > rf;

   for (unsigned int i=0; i<= 1000; i++) {
       std::deque<double> ri;
       double f1=(1.0/1000)*i;
       double f2 = 1 - sqrt(f1);

       ri.push_back(f1);
       ri.push_back(f2);
       rf.push_back(ri);

   }
   return rf;
}

std::deque<std::deque<double> > ZDT5::front() {
   std::deque<std::deque<double> > rf;
   for (unsigned int i=0; i<= 30; i++) {
       std::deque<double> ri;
       double f1=i+1;
       double f2 = 10*1.0/f1;

       ri.push_back(f1);
       ri.push_back(f2);
       rf.push_back(ri);

   }
   return rf;
}

std::deque<std::deque<double> > ZDT6::front() {
    std::deque<std::deque<double> > rf;

    for (unsigned int i=0; i<= 1000; i++) {
        std::deque<double> ri;
        if((1.0/1000)*i<0.2807753191) {continue;}
        double f1=(1.0/1000)*i;
        double f2 = 1 - pow(f1,2);

        ri.push_back(f1);
        ri.push_back(f2);
        rf.push_back(ri);

    }
    return rf;
}
