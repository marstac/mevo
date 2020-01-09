#include "CTestProblems.h"
std::ofstream gFILE;
std::vector<std::vector<double> > convert(std::deque<std::deque<double> > _deq) {
    std::vector<std::vector<double> > result;
    for (int i=0; i< _deq[0].size(); i++) {
        std::vector<double> d;
        for (int j=0; j< _deq.size(); j++) {
            d.push_back(_deq[j][i]);
        }
        result.push_back(d);
    }
    return result;
}

void plot(std::deque<std::deque<double> >* _front, ICPopulation *_lastPop) {
    //return;
    std::vector<std::vector<double> > res= convert(_lastPop->objMatrix());
    std::vector<std::vector<double> > f;
    if(_front) f= convert(*_front);


    Gnuplot* plot = new Gnuplot("f1 / f2");
    plot->set_title("pareto front");
    plot->reset_plot();
    plot->set_grid();
    plot->set_xrange(0,3);
    plot->set_yrange(0,3);
    plot->set_zrange(0,3);
    int dim=res.size();
    if(_front) {
     if(dim==2) plot->set_style("points").plot_xy(f[0],f[1],"real");
     if(dim==3) plot->set_style("points").plot_xyz(f[0],f[1],f[2],"real");
    }
    if(dim==2)plot->set_style("points").plot_xy(res[0],res[1],"estimated");
    if(dim==3)plot->set_style("points").plot_xyz(res[0],res[1],res[2],"estimated");
    std::cin.get();
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

STestParams solveProblem(ICEvolOptimizer* _opt, ICProblem* _p, bool _q){

    Sleep(1000);
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


        plot(&front, lastPop);
        lastPop->save("tmp_res.txt");
    }

    delete lastPop;
    return p;
}
