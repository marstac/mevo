#include "CGDIndicator.h"

namespace MEVO {

CGDIndicator::CGDIndicator()
{

}

CGDIndicator::~CGDIndicator()
{

}

const char *CGDIndicator::getName()
{
    return "Generalized distance indicator";
}

double CGDIndicator::assessPopulation(ICPopulation *_population)
{
    return 0;
}

double CGDIndicator::comparePopWithFront(ICPopulation *_population, std::deque<std::deque<double> > _rf)
{
    std::deque<std::deque<double> > pfront=_population->objMatrix();
    return compareFronts(pfront, _rf);
}

double CGDIndicator::comparePopulations(ICPopulation *_population1, ICPopulation *_population2)
{
    ICPopulation* p1 = _population1->getNondominatedPopulation();
    ICPopulation* p2 = _population2->getNondominatedPopulation();

    std::deque<std::deque<double> > p1front=p1->objMatrix();
    std::deque<std::deque<double> > p2front=p2->objMatrix();
    delete p1;
    delete p2;
    return compareFronts(p1front, p2front);
}

double CGDIndicator::assessFront(std::deque<std::deque<double> > _front)
{
    return 0;
}

double CGDIndicator::compareFronts(std::deque<std::deque<double> > _front1, std::deque<std::deque<double> > _front2)
{

    double res=0;
    for (int i=0; i< _front1.size(); i++) {
        double dist = DOUBLE_MAX;
        for (int j=0; j<_front2.size(); j++) {
            double d=0;
            for (int k=0; k<_front1[i].size();k++){
                d+=pow(_front1[i][k]-_front2[j][k], 2);
            }
            d=sqrt(d);
            if(d<dist) {
                dist=d;
            }
        }
        res+=dist;
    }
    res/=_front1.size();
    return res;
}

}
