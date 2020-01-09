#include "ctp_problems.h"
#include "../utils/functions.h"
#include <iostream>
#include <fstream>
std::deque<std::deque<double> > CTP1::front()
{
    double* x = new double [FRealVarSize];
    for (int i=0; i< FRealVarSize; i++) {
        x[i]=0.0;
    }
    std::deque<std::deque<double> > rf;
    for (int i=0; i< 1000; i++) {
        std::deque<double> ri;
        x[0]=(1.0/1000)*i;
        double f1=FObjectiveFncs[0]->evaluate(x, FRealVarSize, NULL, 0);
        double f2=FObjectiveFncs[1]->evaluate(x, FRealVarSize, NULL, 0);
        for (int j=0; j<FConstraintFncs.size(); j++){
            double c= FConstraintFncs[j]->evaluate(x, FRealVarSize, NULL, 0);
            if (c>0) {
                CTP1_C* fc = dynamic_cast<CTP1_C*>(FConstraintFncs[j]);
                double temp=fc->a*exp(-1*fc->b*f1);
                if(temp>f2){f2=temp;}
            }

        }
        ri.push_back(f1);
        ri.push_back(f2);
        rf.push_back(ri);
    }
    return rf;
}
std::deque<double> maxsss(int v, std::deque<std::deque<double> > &rf);
std::deque<std::deque<double> > CTP2_7::front()
{

    std::deque<std::deque<double> > rf;
    double a = FParameters->getReal("a");
    double b = FParameters->getReal("b");
    double c = FParameters->getReal("c");
    double d = FParameters->getReal("d");
    double e = FParameters->getReal("e");
    double theta =FParameters->getReal("theta");

    float eps=0.001;
    double dir=sin(theta)/cos(theta);
    double sl=e;

   //CTP3 && CTP4
//   if (b==10&&d==0.5 && c==1) {
//       a=0.1;
//       c=1;
//   }

   //CTP5
   if(b==10 && c==2){
      eps=0.001;
   }

   //CTP6
   if (a==40 && b==0.5) {
       dir = -1*cos(theta)/sin(theta);
       sl=1/sin(theta)+e;
       eps=0.001;
   }
   //CTP7

    if(a==40 && b==5){
        eps=0.0001;
        double f1, f2, nf;
        f1=0;
        f2=1;
        nf=f2;
        do {
            f2+=(nf-f2)*eps;
            float exp1 = (f2-e)*cos(theta) - f1*sin(theta);
            float exp2 = (f2-e)*sin(theta) + f1*cos(theta);
            exp2 = b*PI*pow(exp2,c);
            exp2 = fabs(sin(exp2));
            exp2 = a*pow(exp2,d);
            nf=(exp2+f1*sin(theta))/cos(theta)+e;
        } while (fabs(nf-f2)>eps);
        std::deque<double> ri;
        ri.push_back(f1);
        ri.push_back(f2);
        rf.push_back(ri);
    }

    double lf=DOUBLE_MAX;
    for(double x=-0.5; x<1.5; x+=0.001) {
        double y=a*pow(fabs(pow(sin(b*PI*x),c)),d);
        double f1=cos(theta)*x-sin(theta)*y;
        double f2=sin(theta)*x+cos(theta)*y+1;
        std::deque<double> ri;
        ri.push_back(f1);
        ri.push_back(f2);
        rf.push_back(ri);
    }




    return rf;
}


std::deque<double> maxsss(int v, std::deque<std::deque<double> > &rf) {
    double max = -1*MEVO::DOUBLE_MAX;
    double pos=v/100.0;
    std::deque<double> res;
    std::deque<std::deque<double> >::iterator maxit;
    for(std::deque<std::deque<double> >::iterator it = rf.begin(); it!=rf.end(); it++) {
        if((*it)[0]==pos && (*it)[1]>max){
            max=(*it)[1];
            res=*it;
            maxit=it;
        }
    }
    rf.erase(maxit);
    return res;
}
