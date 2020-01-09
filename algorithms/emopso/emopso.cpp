//mopso con epsilon dominance y clusters
#include <iostream>
#include <assert.h>
#include <limits>

#include "../../utils/messages.h"

#include "../../utils/functions.h"


#include "emopso.h"
#include "efile.h"

EMOPSO::~EMOPSO(){
   if(archive) delete archive;
}

EMOPSO::EMOPSO() : CPSOAlgorithm(){

  setNumberOfClusters(0);

  setFlyTime(5);
  archive = NULL;
  FEps=0.01;
  FDiscretization=10;


}

void EMOPSO::setFlyTime(int _ft){
    if (_ft <= 0) {
       fireOnErrorNotify(MEVO::E_OPT_FT);
    } else {
       flyTime = _ft;
    }

}

void EMOPSO::initialize(){
    MEVO::CPopulation ipart;

   for(unsigned int _i(0);_i<FPopSize;_i++){
      ipart.addIndividual(&particles[_i]);
   }

   C1S.clear();
   C2S.clear();
   WS.clear();


   C1P.clear();
   C2P.clear();
   WP.clear();

   C1S.resize(FDiscretization+1, 1);
   C2S.resize(FDiscretization+1, 1);
   WS.resize(FDiscretization+1, 1);

   C1Sum=FDiscretization+1;
   C2Sum=FDiscretization+1;
   WSum=FDiscretization+1;

   C1P.resize(FDiscretization+1, 1.0/(FDiscretization+1));
   C2P.resize(FDiscretization+1, 1.0/(FDiscretization+1));
   WP.resize(FDiscretization+1, 1.0/(FDiscretization+1));

   for (int i=1; i<FDiscretization+1; i++) {
       C1P[i]+=C1P[i-1];
       C2P[i]+=C2P[i-1];
       WP[i]+=WP[i-1];
   }

   initializePopulation(&ipart);
   evaluatePopulation(&ipart);
    for(unsigned int i(0);i<FPopSize;i++){
    archive->add(particles[i],-1);
    }

}

void EFILE::clear() {

}

void EMOPSO::alocateMemory() {
    initVariables();
  FArchive->clearIndividuals();
  if (nclusters==0) {nclusters = FPopSize/8;}
  if(archive) delete archive;
  archive = new EFILE;
  archive->algorithm = this;
  particles.clear();
  for (unsigned int i = 0; i < FPopSize; i++){
      particles.push_back(MOPSOParticle(FProblem->getRealVarSize(),
                                        FProblem->getIntegerVarSize(),
                                        FProblem->getObjectiveFncCnt(),
                                        FProblem->getConstraintFncCnt(),
                                        W, C1, C2, this));
  }

  archive->init(FProblem->getRealVarSize()+FProblem->getIntegerVarSize(),
                FProblem->getObjectiveFncCnt(),
                nclusters,FArchiveSize, 2*FArchiveSize,
                FProblem->getConstraintFncCnt());

}

void EMOPSO::run() {
  alocateMemory();
  execute();

}


void EMOPSO::execute(){
  fireOnBeforeStartNotify("starting", this);
  initialize();
  MEVO::CPopulation spop;
  int gen=0;
  do {
    archive->updatematrix=true;
    archive->hierarchicalClustering();
    flight(gen);

    //setCurrentPop();
    spop.clearIndividuals();
    for (int j=0;j<archive->allnsolutions; j++) {
        spop.addIndividual(&archive->allsolutions[j]);
    }

    gen++;
    fireOnGenerationDoneNotify(gen, &spop);
  } while (!checkStop(&spop));
  output();
  fireOnAfterStopNotify("EMOPSO finished!", FLastPop);
}

void EMOPSO::flight(int _gen){

    MEVO::CPopulation epop;

    std::deque<MEVO::CParticle> leaders;
    MEVO::CPopulation resPop;

    for (int i = 0; i < archive->nsolutions; i++) {
        resPop.addIndividual(&archive->solutions[i]);
    }

    std::deque<double> epsi;
    if (FEps !=0) {
        epsi = archive->FEpsilon;
    }

    for(unsigned int _i(0);_i<FPopSize;_i++){
        //numer klastra od zera do nclusters - 1
        int whichcluster=(int)_i/(FPopSize/nclusters);
        int gbestselected=archive->selectClusteredRandomSolution(whichcluster);

        MOPSOParticle gbestarchparticle(archive->solutions[gbestselected]);

        leaders.push_back(gbestarchparticle);
        particles[_i].setLeader(&leaders[_i]);
        particles[_i].setEps(epsi);
        epop.addIndividual(&particles[_i]);
    }



    mutatePopulation(&epop);

    for(unsigned int _k(0);_k<flyTime;_k++){
        evaluatePopulation(&epop);
        for(unsigned int _i(0);_i<FPopSize;_i++){
            if (particles[_i].isBest()) {
                archive->add(particles[_i], (int)_i/(FPopSize/nclusters));
                //pops[(int)(double)_i/((double)FPopSize/nclusters)].addIndividual(particles[_i].copy());
                C1S[(particles[_i].getC1()-1.0)*FDiscretization/(3.0-1.0)]++;
                C2S[(particles[_i].getC2()-1.0)*FDiscretization/(3.0-1.0)]++;
                WS[(particles[_i].getW())*FDiscretization/(0.4)]++;
            }
        }

        C1Sum=0;
        C2Sum=0;
        WSum=0;
        for (int i=0; i<FDiscretization+1; i++) {
            C1Sum+=C1S[i];
            C2Sum+=C2S[i];
            WSum+=WS[i];
        }
        C1P[0]=(double)C1S[0]/C1Sum;
        C2P[0]=(double)C2S[0]/C2Sum;
        WP[0]=(double)WS[0]/WSum;
        for (int i=1; i<FDiscretization+1; i++) {
            C1P[i]=C1P[i-1]+(double)C1S[i]/C1Sum;
            C2P[i]=C2P[i-1]+(double)C2S[i]/C2Sum;
            WP[i]=WP[i-1]+(double)WS[i]/WSum;
        }

    }
    leaders.clear();
}


void EMOPSO::setNumberOfClusters(int n) {
    nclusters = n;
}
int EMOPSO::getNuberOfClusters() {
    return nclusters;
}


void EMOPSO::output(){
  //archive->finalSolutions();
  setCurrentPop();
}


void EMOPSO::setCurrentPop() {
      MEVO::CPopulation resPop;

      for (int i = 0; i < archive->solutions.size(); i++) {
          resPop.addIndividual(&archive->solutions[i]);
      }
      setLastPop(&resPop);
}

std::deque<double> *EMOPSO::getWP()
{
    return &WP;
}

void EMOPSO::setWP(const std::deque<double> &value)
{
    WP = value;
}

std::deque<double> *EMOPSO::getC2P()
{
    return &C2P;
}

void EMOPSO::setC2P(const std::deque<double> &value)
{
    C2P = value;
}

std::deque<double> *EMOPSO::getC1P()
{
    return &C1P;
}

void EMOPSO::setC1P(const std::deque<double> &value)
{
    C1P = value;
}

int EMOPSO::getFDiscretization() const
{
    return FDiscretization;
}

void EMOPSO::setFDiscretization(int value)
{
    FDiscretization = value;
}

