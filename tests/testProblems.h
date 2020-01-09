#ifndef CTESTPROBLEMS_H
#define CTESTPROBLEMS_H

#include <iostream>
#include <iomanip>
#include <iostream>

#include <time.h>
#include <windows.h>

#include "../mevo_headers.h"

std::ofstream gFILE;

using namespace MEVO;
struct STestParams {
    double hv;
    double igd;
    double si;
    double ctime;
};
void currentTime() {
    time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        std::cout << now->tm_hour<<":"<<now->tm_min<<":"<<now->tm_sec;
}

void configureAlgorithm(ICEvolOptimizer* _alg) {

    if (_alg->getMutator()){_alg->getMutator()->setProbability(0);}
    if (_alg->getIntMutator()){_alg->getIntMutator()->setProbability(0);}
    if (_alg->getRealMutator()){_alg->getRealMutator()->setProbability(0);}

    CGeneticAlgorithm* ga = dynamic_cast<CGeneticAlgorithm*>(_alg);
    if(ga) {
            CAlgWithArchive* awa = dynamic_cast<CAlgWithArchive*>(_alg);
            if(awa) {
                _alg->setPopSize(32);
                awa->setArchiveSize(96);
            } else {
                _alg->setPopSize(96);
            }
    } else {
        CAlgWithArchive* awa = dynamic_cast<CAlgWithArchive*>(_alg);
        if(awa) {
            _alg->setPopSize(96);
            awa->setArchiveSize(96);
        } else {
            _alg->setPopSize(96);
        }
    }



    ICStop* s=_alg->getStop();
    CHVStop* cs = dynamic_cast<CHVStop*>(s);
    if(cs) cs->setFrequency(100);
}

STestParams solveProblem(ICEvolOptimizer* _opt, ICProblem* _p, bool _q=true){
    STestParams p;

    _opt->setProblem(_p);
    configureAlgorithm(_opt);

    clock_t t1=clock();
    _opt->run();

    clock_t t2 = clock();
    p.ctime=double( t2 - t1 ) /  CLOCKS_PER_SEC;

    ICPopulation* lastPop = _opt->getLastPop()->getNondominatedPopulation();

    CProblemWithFront* pwf = dynamic_cast<CProblemWithFront*>(_p);
    if (pwf&&lastPop->getSize()&&_q) {
        std::deque<std::deque<double> > front = pwf->front();

        CSweepHVIndicator hvi;
        std::deque<double> rp = hvi.getFrontMaxs(front);
        for (int i=0; i<rp.size();i++){
            rp[i]*=1.1;
        }
        hvi.setReferencePoint(rp);
        p.hv=hvi.comparePopWithFront(lastPop, front);

        CGDIndicator gdi;
        p.igd = gdi.compareFronts(front, lastPop->objMatrix());

        CSpreadIndicator si;
        p.si = si.assessPopulation(lastPop);
    }

    delete lastPop;
    return p;
}

double mean(std::deque<double> _v) {
    double m=0;
    for (int i=0; i<_v.size();i++) {
        m+=_v[i];
    }
    m/=_v.size();
    return m;
}

double std_dev(std::deque<double> _v) {
    double m = mean(_v);
    double var=0;
    for (int i=0; i<_v.size();i++) {
        var+=(_v[i]-m)*(_v[i]-m);
    }
    var/=_v.size();
    var=sqrt(var);
    return var;
}

void testRun(ICEvolOptimizer* _opt, ICProblem* _p, unsigned int _cnt, bool _q=true) {
    std::deque<double> hv;
    std::deque<double> dt;
    std::deque<double> igd;
    std::deque<double> si;

    double minTime = MEVO::DOUBLE_MAX;
    double maxTime = MEVO::DOUBLE_MIN;

    double minHV = MEVO::DOUBLE_MAX;
    double maxHV = MEVO::DOUBLE_MIN;

    double minIGD = MEVO::DOUBLE_MAX;
    double maxIGD = MEVO::DOUBLE_MIN;

    double minSI = MEVO::DOUBLE_MAX;
    double maxSI = MEVO::DOUBLE_MIN;

    for (int i=0; i< _cnt; i++) {

        std::cout << "\nTEST RUN no. "<<i+1<<", started at: "; currentTime(); std::cout<<'\n';
        STestParams p = solveProblem(_opt, _p,_q);

        std::cout << "\n\nRun no. "<<i+1<<"\tCalculated HV: " <<p.hv <<'\n';

        if (p.ctime<minTime) minTime=p.ctime;
        if (p.ctime>maxTime) maxTime=p.ctime;

        dt.push_back(p.ctime);

       if(_q){
           if (p.hv<minHV) minHV=p.hv;
           if (p.hv>maxHV) maxHV=p.hv;


           if (p.igd<minIGD) minIGD=p.igd;
           if (p.igd>maxIGD) maxIGD=p.igd;

           if (p.hv<minSI) minSI=p.hv;
           if (p.hv>maxSI) maxSI=p.hv;


           hv.push_back(p.hv);
           igd.push_back(p.igd);
           si.push_back(p.si);}

        }

    int tc=1;
    CComputerMultiThread* cmp = dynamic_cast<CComputerMultiThread*>(_opt->getComputer());
    if(cmp) {
        tc=cmp->getThreadsNumber();
    }

    gFILE << _opt->getName() <<'\t'<<tc <<'\t'<< _p->getDescription()<<'\t';
    gFILE << _p->getRealVarSize()+_p->getIntegerVarSize()<<'\t';
    gFILE << _p->getObjectiveFncCnt()<<'\t';
    gFILE << _p->getConstraintFncCnt()<<'\t';
    if (_q) {
        gFILE <<mean(hv) << "\t" <<std_dev(hv) << "\t"<<minHV << "\t"<<maxHV<<'\t';
        gFILE <<mean(igd) << "\t" <<std_dev(igd) << "\t"<<minIGD << "\t"<<maxIGD<<'\t';
        gFILE <<mean(si) << "\t" <<std_dev(si) << "\t"<<minSI << "\t"<<maxSI<<'\t';
    }
    gFILE <<mean(dt) << "\t" <<std_dev(dt) << "\t"<<minTime << "\t"<<maxTime<<'\n';

    std::cout<<"\n\nSummary Results for " <<_p->getDescription()<<" problem solved by " << _opt->getName() << " algorithm:";
    if (_q) {
        std::cout<<"\n\nAverage HV: " <<mean(hv) << "\nstd. dev: " <<std_dev(hv) << "\nMin value: "<<minHV << "\nMax value: "<<maxHV;
        std::cout<<"\n\nAverage IGD: " <<mean(igd) << "\nstd. dev: " <<std_dev(igd) << "\nMin value: "<<minIGD << "\nMax value: "<<maxIGD;
        std::cout<<"\n\nAverage SI: " <<mean(si) << "\nstd. dev: " <<std_dev(si) << "\nMin value: "<<minSI << "\nMax value: "<<maxSI;
    }
    std::cout<<"\n\nAverage time: " <<mean(dt) << "\nstd. dev: " <<std_dev(dt) << "\nMin value: "<<minTime << "\nMax value: "<<maxTime<<'\n';
    delete _p;
}

void qualityTests(std::deque<ICEvolOptimizer*> _opt, unsigned int _cnt) {

    /** -------ZDT PROBLEMS -------- **/
    for (int i=0; i< _opt.size(); i++){
         testRun(_opt[i], new ZDT1(30), _cnt);
    }
    for (int i=0; i< _opt.size(); i++){
         testRun(_opt[i], new ZDT2(30), _cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new ZDT3(30), _cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new ZDT4(10), _cnt);
    }

    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new ZDT5(11), _cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new ZDT6(10), _cnt);
    }

    /** -------CTP PROBLEMS -------- **/
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new CTP1(2,2),_cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new CTP2_7(2, CTP2_7::CTP2),_cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new CTP2_7(2, CTP2_7::CTP3),_cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new CTP2_7(2, CTP2_7::CTP4),_cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new CTP2_7(2, CTP2_7::CTP5),_cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new CTP2_7(10, CTP2_7::CTP6),_cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new CTP2_7(2, CTP2_7::CTP7),_cnt);
    }

    /** -------DTLZ PROBLEMS -------- **/
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new DTLZ1(3,5), _cnt);
    }


    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new DTLZ2(3,10), _cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new DTLZ3(3,10), _cnt);
    }
    return;
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new DTLZ4(3,10), _cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new DTLZ5(3,10), _cnt);
    }
    for (int i=0; i< _opt.size(); i++){
        testRun(_opt[i], new DTLZ6(3,20), _cnt);
    }
}

void setTN(ICEvolOptimizer* _opt, unsigned int _threadsNo) {
    CStopOnGen* sog = new CStopOnGen();
    sog->setMaxGenerations(10);
    _opt->setStop(sog);
    if (_threadsNo>1){
        CComputerMultiThread* c = new CComputerMultiThread();
        c->setThreadsNumber(_threadsNo);
        _opt->setComputer(c);
    } else {
        CComputer* c = new CComputer();
        _opt->setComputer(c);
    }
    CWithNotifiers* ntf = dynamic_cast<CWithNotifiers*>(_opt);
    ntf->clearNotifiers();
}

void speedTest1(ICEvolOptimizer* _opt, unsigned int _threadsNo, unsigned int _var) {
    setTN(_opt, _threadsNo);
    testRun(_opt, new ZDT1(30*_var), 1, false);
}

void speedTest2(ICEvolOptimizer* _opt, unsigned int _threadsNo, unsigned int _var, unsigned int _obj) {
    setTN(_opt, _threadsNo);
    testRun(_opt, new CTP1(2*_var,3*_obj),1, false);
}

void speedTest3(ICEvolOptimizer* _opt, unsigned int _threadsNo, unsigned int _var, unsigned int _obj) {
    setTN(_opt, _threadsNo);
    testRun(_opt, new DTLZ1(3*_obj,5*_var), 1, false);
}

void speedTests(ICEvolOptimizer* _opt, unsigned int _var, unsigned int _obj) {

    speedTest1(_opt,1, 1);
    speedTest1(_opt,2, 1);
    speedTest1(_opt,4, 1);

    speedTest2(_opt,1, 1,1);
    speedTest2(_opt,2, 1,1);
    speedTest2(_opt,4, 1,1);

    speedTest3(_opt,1, 1,1);
    speedTest3(_opt,2, 1,1);
    speedTest3(_opt,4, 1,1);

    for (int i=1; i<=3;i++) {
        speedTest1(_opt,1, pow(_var,i));
        speedTest1(_opt,2, pow(_var,i));
        speedTest1(_opt,4, pow(_var,i));
    }

//    for (int i=1; i<=3;i++) {
//        speedTest2(_opt,1, 1,_obj*i);
//        speedTest2(_opt,2, 1,_obj*i);
//        speedTest2(_opt,4, 1,_obj*i);
//    }
//    for (int i=1; i<=3;i++) {
//        speedTest3(_opt, 1,1,_obj*i);
//        speedTest3(_opt, 2,1,_obj*i);
//        speedTest3(_opt, 4,1,_obj*i);
//    }
}



std::string testsHeader(){
    std::string header;
    header+="Algorithm\tNo. of threads\tProblem\tVar No.\tObj. No.\tCnstr. No\tAverage HV:\tstd. dev:\tMin value:\tMax value";
    header+="\tAverage IGD:\tstd. dev:\tMin value:\tMax value";
    header+="\tAverage SI:\tstd. dev:\tMin value:\tMax value";
    header+="\tAverage time:\tstd. dev:\ttMin value:\tMax value:\n";
    return header;
}

std::string qualityHeader(){
    std::string header;
    header+="Algorithm\tNo. of threads\tProblem\tVar No.\tObj. No.\tCnstr.";
    header+="\tAverage time:\tstd. dev:\ttMin value:\tMax value:\n";
    return header;
}

void tests() {

    std::deque<ICEvolOptimizer*> optimizers;

    optimizers.push_back(new NSGA2);
    optimizers.push_back(new OmniOptimizer);
    optimizers.push_back(new AMGA2);
    optimizers.push_back(new EMOPSO);
    gFILE.open("results_quality.txt");
    gFILE << std::setprecision(5);
    gFILE <<testsHeader();
    qualityTests(optimizers,10);

    gFILE.close();


    gFILE.open("results_speed.txt");
    gFILE << std::setprecision(5);
    gFILE <<qualityHeader();

    speedTests(optimizers[0],10, 50);

    gFILE.close();

    for (int i=0; i<optimizers.size();i++){
        delete optimizers[i];
    }
    optimizers.clear();
}




#endif // CTESTPROBLEMS_H
