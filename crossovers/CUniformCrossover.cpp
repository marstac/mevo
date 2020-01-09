#include "CUniformCrossover.h"
namespace MEVO {
CUniformCrossover::CUniformCrossover() : CCrossover()
{

}

CUniformCrossover::~CUniformCrossover() {

}

void CUniformCrossover::init(ICEvolOptimizer* _opt) {
    CCrossover::init(_opt);
}
ICComputation* CUniformCrossover::copy() {
    CUniformCrossover* c = new CUniformCrossover();
    c->copyParams(this);
    return c;
}

const char* CUniformCrossover::getDescripton(){
    std::stringstream desc;
    desc << "Uniform Crossover" << "\tProbability:\t"<<FProb;
    return desc.str().c_str();
}

void CUniformCrossover::copyParams(CCrossover* _c) {
    CCrossover::copyParams(_c);
}

void CUniformCrossover::crossover(ICPopulation *_parentPopulation, ICPopulation *_offspringPopulation) {
    int no=_offspringPopulation->getSize();
    int np = _parentPopulation->getSize()-1;
    for (int i=0; i<no; i+=2) {
        ICIndividual* child1 = _offspringPopulation->getIndividual(i);
        ICIndividual* child2 = _offspringPopulation->getIndividual(i+1);

        ICIndividual* parent1;
        ICIndividual* parent2;
        do {
            parent1 = _parentPopulation->getIndividual(iRandGen->randomInteger(0, np));
            parent2 = _parentPopulation->getIndividual(iRandGen->randomInteger(0, np));
        } while (parent1==parent2);

        crossTwoIndividuals(parent1,parent2,child1, child2);
    }
}
void CUniformCrossover::crossTwoIndividuals(ICIndividual *_parent1, ICIndividual *_parent2, ICIndividual *_child1, ICIndividual *_child2) {
    if (iRandGen->randomDouble(0,1) <= FProb) {
        for (int i=0; i<FRealVarSize;i++) {
            if(iRandGen->randomDouble(0,1)<0.5) {
                _child1->getRealVar()[i] = _parent1->getRealVar()[i];
                _child2->getRealVar()[i] = _parent2->getRealVar()[i];
            } else {
                _child2->getRealVar()[i] = _parent1->getRealVar()[i];
                _child1->getRealVar()[i] = _parent2->getRealVar()[i];
            }
        }
        for (int i=0; i<FIntVarSize;i++) {
            if(iRandGen->randomDouble(0,1)<0.5) {
                _child1->getIntVar()[i] = _parent1->getIntVar()[i];
                _child2->getIntVar()[i] = _parent2->getIntVar()[i];
            } else {
                _child2->getIntVar()[i] = _parent1->getIntVar()[i];
                _child1->getIntVar()[i] = _parent2->getIntVar()[i];
            }
        }
    }
}
}
