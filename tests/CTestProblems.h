#ifndef CTESTPROBLEMS_H
#define CTESTPROBLEMS_H

#include "../mevo_headers.h"
#include "../gnuplot_i.hpp"

#include <iostream>
#include <iomanip>
#include <iostream>

#include <time.h>
#include <windows.h>

using namespace MEVO;

struct STestParams {
    double hv;
    double igd;
    double si;
    double ctime;
};

void plot(std::deque<std::deque<double> >* _front, ICPopulation* _lastPop);
void currentTime();

void configureAlgorithm(ICEvolOptimizer* _alg);
STestParams solveProblem(ICEvolOptimizer* _opt, ICProblem* _p, bool _q=true);

double mean(std::deque<double> _v);

double std_dev(std::deque<double> _v);

void testRun(ICEvolOptimizer* _opt, ICProblem* _p, unsigned int _cnt, bool _q=true);

void qualityTests(std::deque<ICEvolOptimizer*> _opt, unsigned int _cnt);

void setTN(ICEvolOptimizer* _opt, unsigned int _threadsNo);

void speedTest1(ICEvolOptimizer* _opt, unsigned int _threadsNo, unsigned int _var);

void speedTest2(ICEvolOptimizer* _opt, unsigned int _threadsNo, unsigned int _var, unsigned int _obj);

void speedTest3(ICEvolOptimizer* _opt, unsigned int _threadsNo, unsigned int _var, unsigned int _obj);

void speedTests(ICEvolOptimizer* _opt, unsigned int _var, unsigned int _obj);

std::string testsHeader();

void currentTime();
void tests();

std::vector<std::vector<double> > convert(std::deque<std::deque<double> > _deq);

void plot(std::deque<std::deque<double> >* _front, ICPopulation *_lastPop);



#endif // CTESTPROBLEMS_H
