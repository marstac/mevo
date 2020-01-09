#include "CWithRNGen.h"
#include "../random_numbers/CRandomNumbersGenerator.h"
namespace MEVO {

CWithRNGen::CWithRNGen()
{
    iRandGen = new CRandomNumbersGenerator();
}
CWithRNGen::CWithRNGen(const CWithRNGen& _rg) {
   iRandGen = _rg.iRandGen->copy();
}

CWithRNGen::~CWithRNGen() {
    delete iRandGen;
}

void CWithRNGen::setRandomNumbersGenerator(ICRandomNumbersGenerator* _g) {
    if (iRandGen) {
        delete iRandGen;
    }
    iRandGen = _g;
}

ICRandomNumbersGenerator* CWithRNGen::getRandomNumbersGenerator() {
    return iRandGen;
}
}
