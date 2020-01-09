#ifndef TEST_FRONTS_H
#define TEST_FRONTS_H

#include <deque>
#include <math.h>
#include <set>
#include "../random_numbers/CRandomNumbersGenerator.h"
#include <cstdlib>
 std::deque<std::deque<double> > zdt1_front(unsigned int _size) {
    std::deque<std::deque<double> > rf;
    for (unsigned int i=0; i<= _size; i++) {
        std::deque<double> ri;
        double f1=(1.0/_size)*i;
        double f2 = 1 - sqrt(f1/1.0);
        ri.push_back(f1);
        ri.push_back(f2);
        rf.push_back(ri);
    }
    return rf;
 }

 std::deque<std::deque<double> > zdt2_front(unsigned int _size) {
    std::deque<std::deque<double> > rf;
    for (unsigned int i=0; i<= _size; i++) {
        std::deque<double> ri;
        double f1=(1.0/_size)*i;
        double f2 = 1 - (f1/1.0)*(f1/1.0);
        ri.push_back(f1);
        ri.push_back(f2);
        rf.push_back(ri);
    }
    return rf;
 }

 std::deque<std::deque<double> > zdt3_front(unsigned int _size) {
    std::deque<std::deque<double> > rf;
    double lm=1;
    for (unsigned int i=0; i<= _size; i++) {
        std::deque<double> ri;
        double f1=(1.0/_size)*i;
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

 std::deque<std::deque<double> > zdt4_front(unsigned int _size) {
    std::deque<std::deque<double> > rf;

    for (unsigned int i=0; i<= _size; i++) {
        std::deque<double> ri;
        double f1=(1.0/_size)*i;
        double f2 = 1 - sqrt(f1);

        ri.push_back(f1);
        ri.push_back(f2);
        rf.push_back(ri);

    }
    return rf;
 }

 std::deque<std::deque<double> > zdt5_front() {
    std::deque<std::deque<double> > rf;
    for (unsigned int i=0; i<= 30; i++) {
        std::deque<double> ri;
        double f1=i+1;
        double f2 = 10*1/f1;

        ri.push_back(f1);
        ri.push_back(f2);
        rf.push_back(ri);

    }
    return rf;
 }

 std::deque<std::deque<double> > zdt6_front(unsigned int _size) {
     std::deque<std::deque<double> > rf;

     for (unsigned int i=0; i<= _size; i++) {
         std::deque<double> ri;
         if((1.0/_size)*i<0.2807753191) {continue;}
         double f1=(1.0/_size)*i;
         double f2 = 1 - pow(f1,2);

         ri.push_back(f1);
         ri.push_back(f2);
         rf.push_back(ri);

     }
     return rf;
 }
#endif // TEST_FRONTS_H
