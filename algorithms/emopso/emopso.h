#ifndef EMOPSO_H
#define EMOPSO_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>

#include "../../core/CPopulation.h"
#include "../../core/CPSOalgorithm.h"
#include "../../core/CAlgWithArchieve.h"

#include "Particle.h"
class EFILE;



class EMOPSO : virtual public MEVO::CPSOAlgorithm, virtual public MEVO::CAlgWithArchive {

public:
  EMOPSO();
  ~EMOPSO();

  void run();
  bool checkConf(){if (CPSOAlgorithm::checkConf()){return true;} return false;};
  virtual const char* getName() {return "EMOPSO";}
  void setNumberOfClusters(int n);
  int getNuberOfClusters();

  void setFlyTime(int _ft);
  int getFDiscretization() const;
  void setFDiscretization(int value);

  std::deque<double>* getC1P();
  void setC1P(const std::deque<double> &value);

  std::deque<double>* getC2P();
  void setC2P(const std::deque<double> &value);

  std::deque<double>* getWP();
  void setWP(const std::deque<double> &value);

private :
  void alocateMemory();


  void initialize();
  void flight(int _gen);
  void execute();

  std::deque<MOPSOParticle> particles;
  unsigned int nclusters;
  EFILE* archive;

  unsigned int flyTime;
  void output();
  void setCurrentPop();

  int FDiscretization;
  std::deque<int> C1S;
  std::deque<int> C2S;
  std::deque<int> WS;

  int C1Sum;
  int C2Sum;
  int WSum;

  std::deque<double> C1P;
  std::deque<double> C2P;
  std::deque<double> WP;


};


#endif

