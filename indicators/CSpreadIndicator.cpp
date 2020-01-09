#include "CSpreadIndicator.h"
#include "../core/CPopulation.h"
#include "../core/CIndividual.h"
namespace MEVO {
CSpreadIndicator::CSpreadIndicator()
{

}

CSpreadIndicator::~CSpreadIndicator()
{

}

const char *CSpreadIndicator::getName()
{
    return "Spread indicator";
}

double CSpreadIndicator::assessPopulation(ICPopulation *_population)
{
    if (_population->getSize()<2) {
        return 0;
    }
    std::deque<ICIndividual* > pop;
    for (int i=0;i<_population->getSize();i++){
        pop.push_back(_population->getIndividual(i));
        pop[i]->setCrowdingDistance(0);
    }
    for(int i =0; i<pop[0]->getNumObj();i++) {
        quickSortPopulation(pop,i,0, pop.size()-1);

        double d0 = pow(2*(pop[1]->getObj()[i] - pop[0]->getObj()[i]),2);
        pop[0]->setCrowdingDistance(pop[0]->getCrowdingDistance()+d0);

        double dn = pow(2*(pop[pop.size()-1]->getObj()[i] - pop[pop.size()-2]->getObj()[i]),2);
        pop[pop.size()-1]->setCrowdingDistance(pop[pop.size()-1]->getCrowdingDistance()+dn);
        double d=0;
        for(int j=1; j<pop.size()-1;j++){
            d =pow(pop[j+1]->getObj()[i] - pop[j-1]->getObj()[i],2);
            pop[j]->setCrowdingDistance(pop[j]->getCrowdingDistance()+d);
        }
    }
    double avg=0;
    for(int i=0;i<pop.size();i++) {
        pop[i]->setCrowdingDistance(sqrt(pop[i]->getCrowdingDistance()));
        avg+=pop[i]->getCrowdingDistance();
    }
    avg/=pop.size();

    double delta = 0;
    for(int i=0;i<pop.size();i++) {
        delta+=fabs(pop[i]->getCrowdingDistance()-avg);
    }
    delta/=pop.size();
    delta/=avg;
    return delta;
}

double CSpreadIndicator::comparePopWithFront(ICPopulation *_population, std::deque<std::deque<double> > _rf)
{
    double a = assessPopulation(_population);
    double b = assessFront(_rf);
    return 1-a/b;
}

double CSpreadIndicator::comparePopulations(ICPopulation *_population1, ICPopulation *_population2)
{
    double a = assessPopulation(_population1);
    double b = assessPopulation(_population2);
    return 1-a/b;
}

double CSpreadIndicator::assessFront(std::deque<std::deque<double> > _front)
{
    CPopulation* pop=new CPopulation;
    for(int i=0;i<_front.size();i++){
        CIndividual* ind = new CIndividual(0,0,_front[0].size(),0);
        for(int j=0; j<ind->getNumObj();j++){
            ind->getObj()[j]=_front[i][j];
        }
        pop->addIndividual(ind);
    }
    double res = assessPopulation(pop);
    pop->deleteIndividuals();
    delete pop;
    return res;
}

double CSpreadIndicator::compareFronts(std::deque<std::deque<double> > _front1, std::deque<std::deque<double> > _front2)
{
    double a = assessFront(_front1);
    double b = assessFront(_front2);
    return 1-a/b;
}

void CSpreadIndicator::quickSortPopulation(std::deque<ICIndividual* > &_population, int _fn, int _l, int _h) {
    if(_l<_h) {
        int p = partition(_population, _fn, _l, _h);
        quickSortPopulation(_population, _fn, _l, p-1);
        quickSortPopulation(_population, _fn, p+1, _h);
    }
}

int CSpreadIndicator::partition(std::deque<ICIndividual* > &_population, int _fn, int _l, int _h){
    double pivot = _population[_h]->getObj()[_fn];
    int i=_l;
    for(int j=_l;j<_h;j++){
        if(_population[j]->getObj()[_fn]<=pivot) {
           ICIndividual* tmp = _population[i];
            _population[i] = _population[j];
            _population[j] = tmp;
            i+=1;
        }
    }
    ICIndividual* tmp = _population[i];
    _population[i] = _population[_h];
    _population[_h] = tmp;
    return i;
}
}
