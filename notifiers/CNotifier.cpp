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

#include "CNotifier.h"

#include "../core/CGeneticAlgorithm.h"
#include "../core/CAlgWithArchieve.h"
#include "../core/CPSOalgorithm.h"

namespace MEVO {
CNotifier::CNotifier()
{
    FGenerations=0;
}

CNotifier::~CNotifier() {

}

std::string CNotifier::initialReport(ICEvolOptimizer *_opt) {
    std::stringstream report;

    CEvolOptimizer* eo = dynamic_cast<CEvolOptimizer*>(_opt);

    report << "TECHNICAL INFO:\n";
    report<<"Computation type: \t\t";
    report<<_opt->getComputer()->getName();

    report <<"\nRandom numbers generator: \t";
    report << eo->getRandomNumbersGenerator()->getDescripton();

    report << "\n\nPROBLEM:\t" << _opt->getProblem()->getDescription();
    report <<"\nNumber of real variables:\t"<<_opt->getProblem()->getRealVarSize();
    report <<"\nNumber of integer variables:\t"<<_opt->getProblem()->getIntegerVarSize();
    report<<"\nObjective functions: ";
    for (int i=0; i< _opt->getProblem()->getObjectiveFncCnt(); i++) {
        report<<'\n'<<i+1<<".\t";
        report << _opt->getProblem()->getObjectiveFnc(i)->getDescription();
    }
    if (_opt->getProblem()->getConstraintFncCnt()) {
        report<<"\nConstraint functions: ";
        for (int i=0; i< _opt->getProblem()->getConstraintFncCnt(); i++) {
            report<<'\n'<<i+1<<' ';
            report << _opt->getProblem()->getConstraintFnc(i)->getDescription();
        }
    }

    report << "\n\nALGORITHM:\t\n";
    report<<"Procedure:\t\t\t"<<_opt->getName();


    report<<"\nParameters:";
    report<<"\nPopulation size:\t\t";
    report<<_opt->getPopSize();

    CAlgWithArchive* ar = dynamic_cast<CAlgWithArchive*>(_opt);
    if (ar) {
        report<< "\nArchive size:\t\t"<<ar->getArchiveSize();
    }

    report<<"\nInitializer:\t\t\t";
    if(_opt->getInitialPopulation()) {
        report<<"population loaded from file, size: ";
        report<<_opt->getInitialPopulation()->getSize();
        if (_opt->getInitialPopulation()->getSize()<_opt->getPopSize()) {
            report<<_opt->getInitializer()->getDescripton();
        }
    } else {
       report<<_opt->getInitializer()->getDescripton();
    }

    if (_opt->getMutator()) {
        report << "\nMutation type:\t\t\t" << _opt->getMutator()->getDescripton();
    }
    if (_opt->getRealMutator()&&_opt->getProblem()->getRealVarSize()) {
        report<< "\nReal variables mutation type:\t" <<_opt->getRealMutator()->getDescripton();
    }
    if (_opt->getIntMutator()&&_opt->getProblem()->getIntegerVarSize()) {
        report<< "\nInteger variables mutation type:\t" <<_opt->getIntMutator()->getDescripton();
    }
    CGeneticAlgorithm* ga = dynamic_cast<CGeneticAlgorithm*>(_opt);
    if (ga) {
        if (ga->getCrossover()) {
            report << "\nCrossover type:\t\t\t" << ga->getCrossover()->getDescripton();
        }
        if (ga->getRealCrossover()&&ga->getProblem()->getRealVarSize()) {
            report<< "\nReal variables crossover type:\t" <<ga->getRealCrossover()->getDescripton();
        }
        if (ga->getIntCrossover()&&ga->getProblem()->getIntegerVarSize()) {
            report<< "\nInteger variables crossover type:\t" <<ga->getIntCrossover()->getDescripton();
        }

    }
    report <<"\nEpsilon dominance coefficient:\t" <<_opt->getEpsilon();

    CPSOAlgorithm* pso = dynamic_cast<CPSOAlgorithm*>(_opt);
    if (pso) {
        report<<"\nSocial acceleration:\t";
        report<<pso->getSocialAcc();
        report<<"\nCognitive acceleration:\t";
        report<<pso->getCognitiveAcc();
        report<<"\nInertia:\t";
        report<<pso->getInertia();
    }
    report << "\nStop Condition:\t\t"<<_opt->getStop()->getDescription();

    return report.str();
}
void CNotifier::startTimer() {
    FBegin_time = clock();
}
double CNotifier::stopTimer() {
   FEnd_time = clock();
   return double( FEnd_time - FBegin_time ) /  CLOCKS_PER_SEC;
}
}
