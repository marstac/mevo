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


#include "CPopulation.h"
#include "CIndividual.h"

#include <limits>

#include <string>
#include <stdlib.h>

#include <fstream>
#include <iostream>
namespace MEVO {

CPopulation::CPopulation() {
     FIndividuals.clear();
}

CPopulation::~CPopulation(){
    FIndividuals.clear();
}


CPopulation::CPopulation(const CPopulation& _src) {
    for(int i = 0; i< _src.FIndividuals.size(); i++){
        FIndividuals.push_back(_src.FIndividuals[i]);
    }
}

CPopulation& CPopulation::operator+=(CPopulation& _src)  {
    for (int i=0; i< _src.getSize(); i++) {
        FIndividuals.push_back(_src.FIndividuals[i]);
    }
    return *this;
}

void CPopulation::deleteIndividuals() {
    for(int i=0; i<FIndividuals.size(); i++) {
        delete FIndividuals[i];
    }
    FIndividuals.clear();
}

void CPopulation::clearIndividuals() {
    FIndividuals.clear();
}

ICPopulation* CPopulation::copy() {
    ICPopulation* pop = new CPopulation();
    for(int i=0; i<FIndividuals.size(); i++) {
        ICIndividual* ind = FIndividuals[i]->copy();
        pop->addIndividual(ind);
    }
    return pop;
}

void CPopulation::addIndividual(ICIndividual *_i) {
    FIndividuals.push_back(_i);
}

ICIndividual* CPopulation::getIndividual(unsigned int _i) const {
    if(_i<FIndividuals.size()){
        return FIndividuals[_i];
    }
    return NULL;
}

unsigned int CPopulation::getSize() {
    return FIndividuals.size();
}

std::deque<std::deque<double> > CPopulation::objMatrix() {
    std::deque<std::deque<double> > pop;
    for (int i=0; i< FIndividuals.size(); i++){
        std::deque<double> ind;
        for(int j=0; j<FIndividuals[i]->getNumObj();j++){
            ind.push_back(FIndividuals[i]->getObj()[j]);
        }
        pop.push_back(ind);
    }
    return pop;
}

std::deque<std::deque<double> > CPopulation::varMatrix(){
    std::deque<std::deque<double> > pop;
    for (int i=0; i< FIndividuals.size(); i++){
        std::deque<double> ind;
        for(int j=0; j<FIndividuals[i]->getNumRealVar();j++){
            ind.push_back(FIndividuals[i]->getRealVar()[j]);
        }

        for(int j=0; j<FIndividuals[i]->getNumIntVar();j++){
            ind.push_back(FIndividuals[i]->getIntVar()[j]);
        }

        pop.push_back(ind);
    }
    return pop;
}


std::deque<double> CPopulation::calcMinimalValues() {
    std::deque<double> mins;
    mins.resize(FIndividuals[0]->getNumObj(),std::numeric_limits<double>::max());
    for (int i =0; i<FIndividuals.size();i++) {
        ICIndividual* indi = FIndividuals[i];
        for (int j=0;j<indi->getNumObj();j++){
            if (indi->getObj()[j] < mins[j]) {
                mins[j] = indi->getObj()[j];
            }
        }
    }
    return mins;
}


std::deque<double> CPopulation::calcMaximalValues() {
    std::deque<double> maxs;
    maxs.resize(FIndividuals[0]->getNumObj(),-1* std::numeric_limits<double>::max());
    for (int i =0; i<FIndividuals.size();i++) {
        ICIndividual* indi = FIndividuals[i];
        for (int j=0;j<indi->getNumObj();j++){
            if (indi->getObj()[j] > maxs[j]) {
                maxs[j] = indi->getObj()[j];
            }
        }
    }
    return maxs;
}

std::deque<double> CPopulation::calcEpsi(double _eps){
    std::deque<double> mins = calcMinimalValues();
    std::deque<double> maxs = calcMaximalValues();
    std::deque<double> epsi(FIndividuals[0]->getNumObj(), 0.0);
    for (int i=0; i<epsi.size();i++) {
        epsi[i]=_eps*(maxs[i]-mins[i]);
    }
    return epsi;
}

ICPopulation* CPopulation::getNondominatedPopulation(double _eps) {
    ICPopulation* resPop = new CPopulation();
    for(int i = 0; i< FIndividuals.size(); i++){
        ICIndividual* indi = FIndividuals[i];
        bool add=true;
        for(int j=0;j<FIndividuals.size();j++) {
            if(i!=j) {
                ICIndividual* indj = FIndividuals[j];
                if (indi->checkDominance(indj)<0) {
                    add=false;
                    break;
                }
            }
        }
        if (add) {
            bool exist =false;
            for (int j=0; j<resPop->getSize();j++) {
                ICIndividual* indj = resPop->getIndividual(j);
                if((*indi)==(*indj)){
                    exist=true;
                    break;
                }
            }
            if(!exist){
                resPop->addIndividual(indi);
            }
        }
   }
    return resPop;
}

ICPopulation *CPopulation::getFeasiblePopulation()
{
    ICPopulation* resPop = new CPopulation();
    for(int i = 0; i< FIndividuals.size(); i++){
        ICIndividual* indi = FIndividuals[i];
        if(indi->getConstrViolation()<=0){
            resPop->addIndividual(indi);
        }
    }
    return resPop;
}


void CPopulation::normalizePopulation(std::deque<double>* _max, std::deque<double>* _min) {
    std::deque<double> max;
    std::deque<double> min;
    if (_max==NULL) {
        max = calcMaximalValues();
        _max = &max;
    }
    if (_min==NULL) {
        min = calcMinimalValues();
        _min = &min;
    }

    for (int i=0; i<FIndividuals.size(); i++) {
        for (int j = 0; j< FIndividuals[i]->getNumObj();j++){
            FIndividuals[i]->getObj()[j] = (FIndividuals[i]->getObj()[j]-(*_min)[j])/((*_max)[j]-(*_min)[j]);
        }

    }
}

void CPopulation::denormalizePopulation(std::deque<double> _max, std::deque<double> _min) {
    for (int i=0; i<FIndividuals.size(); i++) {
        for (int j = 0; j< FIndividuals[i]->getNumObj();j++){
            FIndividuals[i]->getObj()[j] = FIndividuals[i]->getObj()[j]*(_max[j]-_min[j]) + _min[j];
        }
    }
}

void CPopulation::invertPopulation(std::deque<double>* _max, std::deque<double>* _min) {
    normalizePopulation(_max, _min);
    for (int i=0; i<FIndividuals.size(); i++) {
        for (int j = 0; j< FIndividuals[i]->getNumObj();j++){
            FIndividuals[i]->getObj()[j] = 1-FIndividuals[i]->getObj()[j];
        }
    }
}

void CPopulation::save(std::string _fname) {
    if (FIndividuals.size()!=0) {
        ICIndividual* first = FIndividuals[0];

        std::ofstream file(_fname.c_str());
        file<<std::setprecision(10);

        std::string text ="rvar_";
        for (int i=0; i< first->getNumRealVar(); i++) {

            file << text << i<<'\t';
        }

        text ="ivar_";
        for (int i=0; i< first->getNumIntVar(); i++) {

            file << text << i<< '\t';
        }
        text ="constr_";
        for (int i=0; i< first->getNumConstr(); i++) {

            file << text << i<< '\t';
        }
        text ="obj_";
        for (int i=0; i< first->getNumObj(); i++) {

             file << text << i<< '\t';
        }
        file<<'\n';
        for (int i=0; i<FIndividuals.size();i++) {
            ICIndividual* ind = FIndividuals[i];
            for (int j=0; j< ind->getNumRealVar(); j++) {
                file << ind->getRealVar()[j] << '\t';
            }
            for (int j=0; j< ind->getNumIntVar(); j++) {
                file << ind->getIntVar()[j] << '\t';
            }
            for (int j=0; j< ind->getNumConstr(); j++) {
                file << ind->getConstr()[j] << '\t';
            }

            for (int j=0; j< ind->getNumObj();j++) {
                file << ind->getObj()[j] << '\t';
            }
            file<<'\n';
        }
        file.close();
    }
}

bool CPopulation::load(std::string _fname){
    std::ifstream file(_fname.c_str());
    std::string text;
    int rvar=0;
    int ivar=0;
    int obj=0;
    int cnstr=0;

    if (file.is_open()) {
        std::getline(file, text);
        int pos=text.length();

        pos = text.rfind("rvar", pos);
        if(pos>0) rvar++;
        while (pos>0) {
            rvar++;
            pos = text.rfind("rvar", pos-4);
        }

        pos=text.length();
        pos = text.rfind("ivar", pos);
        if(pos>0&&rvar==0) ivar++;
        while (pos>0) {
            ivar++;
            pos = text.rfind("ivar", pos-4);
        }

        pos=text.length();
        pos = text.rfind("constr", pos);
        //if(pos>0) cnstr++;
        while (pos>0) {
            cnstr++;
            pos = text.rfind("constr", pos-6);
        }

        pos=text.length();
        pos = text.rfind("obj", pos);
        //if(pos>0) obj++;
        while (pos>0) {
            obj++;
            pos = text.rfind("obj", pos-3);
        }
        while (!file.eof()) {
            CIndividual* ind = new CIndividual(rvar,ivar,obj,cnstr);
            for (int i=0;i< rvar; i++) {
                file>>ind->getRealVar()[i];
            }
            for (int i=0;i< ivar; i++) {
                file>>ind->getIntVar()[i];
            }
            for (int i=0;i< cnstr; i++) {
                file>>ind->getConstr()[i];
            }
            for (int i=0;i< obj; i++) {
                file>>ind->getObj()[i];
            }
            if(!file.eof()) {
                FIndividuals.push_back(ind);
            } else {
                delete ind;
            }
        }
        return true;
    }
    return false;
}

}
