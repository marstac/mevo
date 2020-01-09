/*  Author:
  Marcin A. Stachura <m.stachura@mchtr.pw.edu.pl>

  Copyright (c) 2015 Marcin A. Stachura

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "CHyperVolumeIndicator.h"

namespace MEVO {
CHyperVolumeIndicator::CHyperVolumeIndicator() : CIndicator()
{
    FCoeff=1.1;
}
CHyperVolumeIndicator::~CHyperVolumeIndicator(){

}

const char *CHyperVolumeIndicator::getName()
{
    return "Hypervolume indicator";
}

double CHyperVolumeIndicator::assessPopulation(ICPopulation* _population){
    if (_population->getSize()==0) {
        return 0;
    }

    ICPopulation* p = _population->getNondominatedPopulation();

    std::deque<std::deque<double> > front=p->objMatrix();

    double res = assessFront(front);
    delete p;
    return res;
}

double CHyperVolumeIndicator::comparePopWithFront(ICPopulation* _population, std::deque<std::deque<double> > _rf) {

    std::deque<std::deque<double> > pfront=_population->objMatrix();
    return compareFronts(pfront, _rf);
}

double CHyperVolumeIndicator::comparePopulations(ICPopulation* _population1, ICPopulation* _population2){

    ICPopulation* p1 = _population1->getNondominatedPopulation();
    ICPopulation* p2 = _population2->getNondominatedPopulation();

    std::deque<std::deque<double> > p1front=p1->objMatrix();
    std::deque<std::deque<double> > p2front=p2->objMatrix();
    delete p1;
    delete p2;
    return compareFronts(p1front, p2front);
}

double CHyperVolumeIndicator::assessFront(std::deque<std::deque<double> > _front) {
    double res=0;
    if(_front.size()>1){
        bool clear=false;
        if (FRefPoint.size()==0){
            clear=true;
            FRefPoint.resize(_front[0].size(),FCoeff);
            _front = normalizedFront(_front);
        }
        res = calculate(_front);
        if (clear){clearReferencePoint();}
    }
    return res;
}

double CHyperVolumeIndicator::compareFronts(std::deque<std::deque<double> > _front1, std::deque<std::deque<double> > _front2){
    bool clear=false;
    if (FRefPoint.size()==0){
        std::deque<std::deque<double> > combfront;
        for (int i=0; i<_front1.size(); i++) {
            combfront.push_back(_front1[i]);
        }

        for (int i=0; i<_front2.size(); i++) {
            combfront.push_back(_front2[i]);
        }

        FRefPoint = getFrontMaxs(combfront);
        for(int i=0; i<FRefPoint.size();i++) {
            FRefPoint[i]*=FCoeff;
        }
        clear=true;
    }

    double a = calculate(_front1);
    double b = calculate(_front2);

    if(clear){clearReferencePoint();}
    return 1-a/b;
}

void CHyperVolumeIndicator::setReferencePoint(std::deque<double> _p) {
    FRefPoint = _p;
}


std::deque<double> CHyperVolumeIndicator::getReferencePoint() {
    return FRefPoint;
}

void CHyperVolumeIndicator::clearReferencePoint()
{
    FRefPoint.clear();
}

double CHyperVolumeIndicator::getFCoeff() const
{
    return FCoeff;
}

void CHyperVolumeIndicator::setFCoeff(double value)
{
    FCoeff = value;
}

double CHyperVolumeIndicator::calcOnePoint(std::deque<double> _p)
{
    double res=1;
    for(int i=0;i<_p.size();i++){
        double d = FRefPoint[i]-_p[i];
        if(d<0){d=0;}
        res*=d;
    }
    return res;
}

double CHyperVolumeIndicator::calculate(std::deque<std::deque<double> > _front)
{
    double res=0;
    if (_front.size()==1) {
       res = calcOnePoint(_front[0]);
    } else {
       res = calculateHypervolume(_front);
    }
    return res;
}

}

