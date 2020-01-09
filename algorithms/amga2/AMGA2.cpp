#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "AMGA2.h"
#include "../../utils/messages.cpp"
#include "../../utils/functions.cpp"

#include "../../mutators/CPolynomialMutator.h"
#include "../../crossovers/CDECrossover.h"

AMGA2::AMGA2() : CGeneticAlgorithm() , CAlgWithArchive() {

  minVar = NULL;
  maxVar = NULL;
  minIntVar = NULL;
  maxIntVar = NULL;
  numConstr=0;

  minObj=NULL;
  maxObj=NULL;
  this->setPopSize(16);

  setMutator(new MEVO::CPolynomialMutator);
  setCrossover(new MEVO::CDECrossover);

}

AMGA2::~AMGA2() {
  archivePopulation.clear();
  parentPopulation.clear();
  offspringPopulation.clear();
  combinedPopulation.clear();

  if (minVar) delete[](minVar);
  if (maxVar) delete[](maxVar);

  delete[](minObj);
  delete[](maxObj);

  if (minIntVar) delete[] minIntVar;
  if (maxIntVar) delete[] maxIntVar;

  if (numConstr > 0) {
    delete[](minConstr);
    delete[](maxConstr);
  }
}
bool AMGA2::checkConf() {
    if(CGeneticAlgorithm::checkConf()){
        return true;
    }
    if (getArchiveSize() < getPopSize()) {
        fireOnErrorNotify(MEVO::E_OPT_ARCH_SIZE);
        return false;
    }
    return true;
}

void AMGA2::alocateMemory() {
    archivePopulation.clear();
    parentPopulation.clear();
    offspringPopulation.clear();
    combinedPopulation.clear();

    if (minVar) {delete[](minVar); minVar=NULL;}
    if (maxVar) {delete[](maxVar); maxVar=NULL;}

    if (minObj) delete[](minObj);
    if (maxObj) delete[](maxObj);

    if (minIntVar) {delete[] minIntVar; minIntVar=NULL;}
    if (maxIntVar) {delete[] maxIntVar; maxIntVar=NULL;}

    if (numConstr > 0) {
      delete[](minConstr);
      delete[](maxConstr);
        minConstr=NULL;
        maxConstr=NULL;
    }

    FArchive->clearIndividuals();


    numVar = FProblem->getRealVarSize();
	if (numVar) {
      minVar = new double[numVar];
	  maxVar = new double[numVar];
  }
   numIntVar = FProblem->getIntegerVarSize();
   if (numIntVar) {
	  minIntVar = new double[numIntVar];
	  maxIntVar = new double[numIntVar];
  }

  for (register unsigned int i = 0; i < numVar; i++) {
    minVar[i] = FProblem->getRealLowLimit(i);
    maxVar[i] = FProblem->getRealHighLimit(i);
  }

  for (register unsigned int i = 0; i < numIntVar; i++) {
    minIntVar[i] = FProblem->getIntegerLowLimit(i);
    maxIntVar[i] = FProblem->getIntegerHighLimit(i);
  }

  numObj = FProblem->getObjectiveFncCnt();
  minObj = new double[numObj];
  maxObj = new double[numObj];
  for (register unsigned int i = 0; i < numObj; i++) {
    minObj[i] = maxObj[i] = 0.0;
  }
  numConstr = FProblem->getConstraintFncCnt();
  if (numConstr > 0) {
    minConstr = new double[numConstr];
    maxConstr = new double[numConstr];
    for (register unsigned int i = 0; i < numConstr; i++) {
      minConstr[i] = maxConstr[i] = 0.0;
    }
  }

  initialSize = FArchiveSize;
  eliteSize = FArchiveSize;

  populationSize = getPopSize();

  currentArchiveSize = 0;
  evaluationCounter = 0;
  isEvolutionEnded = false;
  archivePopulation.reserve(FArchiveSize);
  parentPopulation.reserve(populationSize);
  offspringPopulation.reserve(populationSize);
  combinedPopulation.reserve(populationSize + FArchiveSize);
  for (register unsigned int i = 0; i < FArchiveSize; i++) {
    archivePopulation.push_back(AMGA2Individual(numVar,numIntVar,numObj,numConstr));
    combinedPopulation.push_back(AMGA2Individual(numVar,numIntVar,numObj,numConstr));
  }
  for (register unsigned int i = 0; i < populationSize; i++) {
    parentPopulation.push_back(AMGA2Individual(numVar,numIntVar,numObj,numConstr));
    offspringPopulation.push_back(AMGA2Individual(numVar,numIntVar,numObj,numConstr));
    combinedPopulation.push_back(AMGA2Individual(numVar,numIntVar,numObj,numConstr));
  }


}

void AMGA2::run() {

  if (FPopSize<FProblem->getObjectiveFncCnt()) {
      FPopSize=FProblem->getObjectiveFncCnt()*2;
      FArchiveSize=2*FPopSize;
  }

  CGeneticAlgorithm::initVariables();
  alocateMemory();
  evolvePopulation();
}

void AMGA2::assignDiversityMetric(std::vector<AMGA2Individual>& population, std::list<unsigned int>& elite) {
  if (elite.size() <= 2) {
	for (std::list<unsigned int>::iterator iter = elite.begin(); iter != elite.end(); ++iter) {
      population[*iter].setCrowdingDistance(MEVO::DOUBLE_MAX);
    }
    return;
  }
  for (std::list<unsigned int>::iterator iter = elite.begin(); iter != elite.end(); ++iter) {
    population[*iter].setCrowdingDistance(0.0);
  }
  std::list<unsigned int> distinct;
  extractDistinctIndividuals(population, elite, distinct);
  if (distinct.size() <= 2) {
	for (std::list<unsigned int>::iterator iter = distinct.begin(); iter != distinct.end(); ++iter) {
      population[*iter].setCrowdingDistance(MEVO::DOUBLE_MAX);
    }
  } else {
    register unsigned int size = static_cast<unsigned int> (distinct.size());
    unsigned int *indexArray = new unsigned int[size];
    unsigned int **objArray = new unsigned int*[numObj];
    for (register unsigned int i = 0; i < numObj; i++) {
      objArray[i] = new unsigned int[size];
    }
    register unsigned int counter = 0;
	for (std::list<unsigned int>::iterator iter = distinct.begin(); iter != distinct.end(); ++iter) {
      indexArray[counter++] = *iter;
    }
    for (register unsigned int i = 0; i < numObj; i++) {
      for (register unsigned int j = 0; j < size; j++) {
        objArray[i][j] = indexArray[j];
      }
      quickSortObj(population, i, objArray[i], 0, size - 1);
    }
    for (register unsigned int j = 0; j < size; j++) {
      population[indexArray[j]].setCrowdingDistance(0.0);
    }
    register double l, r;
    for (register unsigned int i = 0; i < numObj; i++) {
      for (register unsigned int j = 0; j < size; j++) {
        if (population[objArray[i][size - 1]].normalObj[i] != population[objArray[i][0]].normalObj[i]) {
          register double val = population[objArray[i][size - 1]].normalObj[i] - population[objArray[i][0]].normalObj[i];
          if (j == 0) {
            r = (population[objArray[i][j + 1]].normalObj[i] - population[objArray[i][j]].normalObj[i]) / val;
            population[objArray[i][j]].setCrowdingDistance(population[objArray[i][j]].getCrowdingDistance() + r*r);
          } else if (j == size - 1) {
            l = (population[objArray[i][j]].normalObj[i] - population[objArray[i][j - 1]].normalObj[i]) / val;
            population[objArray[i][j]].setCrowdingDistance(population[objArray[i][j]].getCrowdingDistance() + l*l);
          } else {
            l = (population[objArray[i][j]].normalObj[i] - population[objArray[i][j - 1]].normalObj[i]) / val;
            r = (population[objArray[i][j + 1]].normalObj[i] - population[objArray[i][j]].normalObj[i]) / val;
            population[objArray[i][j]].setCrowdingDistance(population[objArray[i][j]].getCrowdingDistance() + l*r);
          }
        }
      }
	}
    delete[](indexArray);
    for (register unsigned int i = 0; i < numObj; i++) {
      delete[](objArray[i]);
    }
    delete[](objArray);
  }
  distinct.clear();
  return;
}

void AMGA2::assignInfiniteDiversity(std::vector<AMGA2Individual> &population, std::list<unsigned int> &elite) {
  if (elite.size() <= 2) {
	for (std::list<unsigned int>::iterator iter = elite.begin(); iter != elite.end(); ++iter) {
      population[*iter].setCrowdingDistance(MEVO::DOUBLE_MAX);
    }
    return;
  }
  register unsigned int size = elite.size();
  unsigned int *indexArray = new unsigned int[size];
  register unsigned int counter = 0;
  for (std::list<unsigned int>::iterator iter = elite.begin(); iter != elite.end(); ++iter) {
    indexArray[counter++] = *iter;
  }
  register unsigned int index;
  register double minimumObjective;
  for (register unsigned int i = 0; i < numObj; i++) {
    index = indexArray[0];
    minimumObjective = population[index].normalObj[i];
    for (register unsigned int j = 0; j < size; j++) {
      if (minimumObjective > population[indexArray[j]].normalObj[i]) {
        index = indexArray[j];
        minimumObjective = population[index].normalObj[i];
      }
    }
    population[index].setCrowdingDistance(MEVO::DOUBLE_MAX);
  }
  delete[](indexArray);
  return;
}

void AMGA2::createInitialPopulation() {

  MEVO::CPopulation ipop;
  for (int i=0; i<initialSize;i++) {
      MEVO::ICIndividual* ind = &archivePopulation[i];
      ipop.addIndividual(ind);
  }
   initializePopulation(&ipop);
}

void AMGA2::createParentPopulation() {
  std::list<unsigned int> pool;
  std::list<unsigned int> elite;
  std::list<unsigned int> selectionPool;
  std::list<unsigned int> extreme;
  for (register unsigned int i = 0; i < currentArchiveSize; i++) {
    pool.push_back(i);
  }
  register unsigned int rank = 1;
  while (selectionPool.size() < populationSize) {
    extractBestRank(archivePopulation, pool, elite); // HERE we delete the resource
    //VALgrind
    for (std::list<unsigned int>::iterator iter = elite.begin(); iter != elite.end(); ++iter) {
      archivePopulation[*iter].rank = rank; // and here we reach for it again
      selectionPool.push_back(*iter);
    }
    rank++;
    elite.clear();
  }
  extractExtremeSolutions(archivePopulation, selectionPool, extreme);
  register unsigned int j = 0;
  for (std::list<unsigned int>::iterator iter = extreme.begin(); iter != extreme.end(); ++iter) {
    parentPopulation[j] = archivePopulation[*iter];
    j++;
    if (j== FPopSize) {break;}
  }
  fillDiversePopulation(archivePopulation, selectionPool, parentPopulation, j, populationSize - j);
  pool.clear();
  elite.clear();
  selectionPool.clear();
  extreme.clear();
  return;
}

void AMGA2::createOffspringPopulation() {
    MEVO::CPopulation ppop;
    MEVO::CPopulation opop;
  for (register unsigned int i = 0; i < populationSize; i++) {
      opop.addIndividual(&offspringPopulation[i]);
      ppop.addIndividual(&parentPopulation[i]);
      offspringPopulation[i].rank = parentPopulation[i].rank; //for rank based mutation
  }
  crossoverPopulation(&ppop, &opop);
  return;
}

void AMGA2::finalizePopulation() {
  std::list<unsigned int> pool;
  std::list<unsigned int> elite;

  MEVO::CPopulation resPop;

  for (register unsigned int i = 0; i < currentArchiveSize; i++) {
    if (archivePopulation[i].getConstrViolation() <= 0.0) {
      pool.push_back(i);
    }
  }
  if (pool.size() == 0) {
    currentArchiveSize = 0;
  } else {
    extractBestRank(archivePopulation, pool, elite);
    pool.clear();
    if (elite.size() > eliteSize) {
	  for (std::list<unsigned int>::iterator iter = elite.begin(); iter != elite.end(); ++iter) {
        archivePopulation[*iter].setCrowdingDistance(0.0);
      }
      assignInfiniteDiversity(archivePopulation, elite);
      extractENNSPopulation(archivePopulation, elite, pool, eliteSize);
      elite.clear();
	  for (std::list<unsigned int>::iterator iter = pool.begin(); iter != pool.end(); ++iter) {
        elite.push_back(*iter);
      }
      pool.clear();
    }
    currentArchiveSize = elite.size();
    register unsigned int i = 0;
	for (std::list<unsigned int>::iterator iter = elite.begin(); iter != elite.end(); iter++) {
      combinedPopulation[i] = archivePopulation[*iter];
      resPop.addIndividual(&combinedPopulation[*iter]);
      i++;
    }
  }
  pool.clear();
  elite.clear();
  setLastPop(&resPop);
  if (resPop.getSize()==0) {
    fireOnAfterStopNotify(MEVO::M_OPT_NO_FSOL, FLastPop);
  }
  return;
}

void AMGA2::evaluate_Population(std::vector<AMGA2Individual>& population, unsigned int size) {
  MEVO::CPopulation epop;
  for (register unsigned int i = 0; i < size; i++) {
    evaluationCounter++;
    population[i].setRank(0);
    population[i].setCrowdingDistance(0.0);
    evaluateIndividual(&population[i]);
    epop.addIndividual(&population[i]);
  }
  evaluatePopulation(&epop);
  return;
}

void AMGA2::evaluateIndividual(AMGA2Individual * individual) {
  individual->setEvalCounter(evaluationCounter);
  return;
}


void AMGA2::normalizePopulation(std::vector<AMGA2Individual>& population, unsigned int size) {
  for (register unsigned int i = 0; i < size; i++) {
    //change to normalize individual
    normalizeIndividualValues(&(population[i]));
  }
  return;
}


void AMGA2::normalizeIndividualValues(AMGA2Individual * individual) {
  double * obj = individual->getObj();
  double * normalObj = individual->getNormalObj();

  for (register unsigned int i = 0; i < numObj; i++) {
    if (maxObj[i] > minObj[i]) {
      normalObj[i] = (obj[i] + maxObj[i] - 2.0 * minObj[i]) / (maxObj[i] - minObj[i]);
    } else {
      normalObj[i] = 1.0;
    }
  }

  return;
}


void AMGA2::evolvePopulation() {

    if (!checkConf()) {
      fireOnErrorNotify(MEVO::E_OPT_TERM);
      return;
    }
  fireOnBeforeStartNotify("AMGA 2 Starting...", this);
  createInitialPopulation();
  currentArchiveSize = initialSize;

  evaluate_Population(archivePopulation, currentArchiveSize);

  updateObjectiveConstraintRange();
  normalizePopulation(archivePopulation, currentArchiveSize);

    int i=0;
    FArchive->clearIndividuals();
    for(int i =0; i<currentArchiveSize; i++) {
       FArchive->addIndividual(&archivePopulation[i]);
    }
    FCurrentGeneration=0;
  do {


    updateObjectiveConstraintRange();
    normalizePopulation(archivePopulation, currentArchiveSize);
    createParentPopulation();
    createOffspringPopulation();
    mutateOffspringPopulation();
    evaluate_Population(offspringPopulation, populationSize);
    normalizePopulation(offspringPopulation, populationSize);
    updateArchivePopulation();
    register unsigned int rankOneSize = 0;

    for (register unsigned int j = 0; j < currentArchiveSize; j++) {
       if (archivePopulation[j].rank == 1) {
        rankOneSize++;
      }
    }
    FCurrentGeneration++;

    fireOnGenerationDoneNotify(FCurrentGeneration, FArchive);

	if (FTerminated) {
		break;
	}

  } while (!checkStop(FArchive));

  finalizePopulation();
  fireOnAfterStopNotify("AMGA2 finished!",FLastPop);
  return;
}

void AMGA2::extractBestRank(std::vector<AMGA2Individual>& population, std::list<unsigned int>& pool, std::list<unsigned int>& elite) {
  std::deque<double>* epss = NULL;
  std::deque<double> epsi;
  if (FEps !=0) {
      MEVO::CPopulation pop;
      for (int i=0; i<population.size(); i++) {
          pop.addIndividual(&(population[i]));
      }
      epsi = pop.calcEpsi(FEps);
      epss=&epsi;
  }


  std::list<unsigned int> remains;
  std::list<unsigned int>::iterator iter;
  register int flag = 0;
  register int index1 = 0;
  register int index2 = 0;
  bool stop = true;
  if (pool.empty() ){
    return;
  }
  elite.push_back(pool.front());
  pool.pop_front(); // memory corruption - checked with valgrignd
  while (pool.size() > 0) {
    index1 = pool.front();
    pool.pop_front();
    stop = false;
    flag = -1;
    iter = elite.begin();
    while (stop == false && iter != elite.end()) {
      index2 = *iter;
      flag = population[index1].checkDominance(&(population[index2]), epss);
      if (flag == 1) {
        remains.push_back(index2);
        iter = elite.erase(iter);
      } else if (flag == -1) {
        stop = true;
        ++iter;
      } else {
        ++iter;
      }
    }
    if (flag == 0 || flag == 1) {
      elite.push_back(index1);
    } else {
      remains.push_back(index1);
    }
  }
  pool.clear();
  for (iter = remains.begin(); iter != remains.end(); ++iter) {
    pool.push_back(*iter);
  }
  remains.clear();
  return;
}

void AMGA2::extractDistinctIndividuals(std::vector<AMGA2Individual> &population, std::list<unsigned int> &elite, std::list<unsigned int> &distinct) {
  register unsigned int size = static_cast<unsigned int> (elite.size());
  unsigned int* indexArray = new unsigned int[size];
  register unsigned int i = 0;
  for (std::list<unsigned int>::iterator iter = elite.begin(); iter != elite.end(); ++iter) {
    indexArray[i++] = *iter;
  }
  quickSortObj(population, 0, indexArray, 0, size - 1);
  register unsigned int current = indexArray[0];
  distinct.push_back(current);
  for (register unsigned int i = 1; i < size; i++) {
    register bool flag = true;
    for (register unsigned int j = 0; j < numObj; j++) {
      if (population[indexArray[i]].normalObj[j] != population[current].normalObj[j]) {
        flag = false;
      }
    }
    if (!flag) {
      current = indexArray[i];
      distinct.push_back(current);
    }
  }
  delete[](indexArray);
  return;
}

void AMGA2::extractENNSPopulation(const std::vector<AMGA2Individual> &mixedPopulation, std::list<unsigned int> &pool, std::list<unsigned int> &elite, unsigned int desiredEliteSize) {
  register unsigned int poolSize = pool.size();
  register unsigned int mixedSize = mixedPopulation.size();
  register unsigned int numInf = 0;
  for (std::list<unsigned int>::iterator iter = pool.begin(); iter != pool.end(); ++iter) {
    if ((mixedPopulation[*iter]).getCrowdingDistance() == MEVO::DOUBLE_MAX) {
      numInf++;
	}
  }
  if (desiredEliteSize <= numInf) {
	for (std::list<unsigned int>::iterator iter = pool.begin(); iter != pool.end(); ++iter) {
      if (mixedPopulation[*iter].getCrowdingDistance() == MEVO::DOUBLE_MAX) {
        elite.push_back(*iter);
        if (elite.size() == desiredEliteSize) {
          return;
        }
      }
    }
  }
  for (std::list<unsigned int>::iterator iter = pool.begin(); iter != pool.end(); ++iter) {
    elite.push_back(*iter);
  }
  pool.clear();
  if (desiredEliteSize == elite.size()) {
    return;
  }
  double **distance;
  unsigned int *indexArray;
  int *originalArray;
  distance = new double*[poolSize];
  indexArray = new unsigned int[poolSize];
  originalArray = new int[mixedSize];
  for (register unsigned int i = 0; i < poolSize; i++) {
    distance[i] = new double[poolSize];
    indexArray[i] = 0;
    for (register unsigned int j = 0; j < poolSize; j++) {
      distance[i][j] = 0.0;
    }
  }
  for (register unsigned int i = 0; i < mixedSize; i++) {
    originalArray[i] = -1;
  }
  register unsigned int counter = 0;
  for (std::list<unsigned int>::iterator iter = elite.begin(); iter != elite.end(); ++iter) {
    indexArray[counter] = *iter;
    originalArray[indexArray[counter]] = counter;
    counter++;
  }
  std::vector<DistanceMatrix> distArray;
  distArray.reserve(((poolSize - 1) * poolSize) / 2);
  for (register unsigned int i = 0; i < poolSize; i++) {
    distance[i][i] = 0.0;
    for (register unsigned int j = i + 1; j < poolSize; j++) {
      distance[j][i] = distance[i][j] = calcEuclideanDistance(mixedPopulation[indexArray[i]], mixedPopulation[indexArray[j]]);
      distArray.push_back(DistanceMatrix(indexArray[i], indexArray[j], distance[i][j]));
    }
  }
  sort(distArray.begin(), distArray.end());
  std::vector<DistanceMatrix>::iterator distIterator = distArray.begin();
  while (elite.size() > desiredEliteSize && distIterator != distArray.end()) {
	register int index = -1;
    register int index1 = -1;
    register int index2 = -1;
    register double dist1 = 0.0;
    register double dist2 = 0.0;
    do {
      DistanceMatrix temp = *distIterator;
      ++distIterator;
      index1 = temp.index1;
      index2 = temp.index2;
    } while ((originalArray[index1] == -1 || originalArray[index2] == -1) && distIterator != distArray.end());
    if (distIterator == distArray.end()) {
      break;
    }
    if (mixedPopulation[index1].getCrowdingDistance() == MEVO::DOUBLE_MAX && mixedPopulation[index2].getCrowdingDistance() == MEVO::DOUBLE_MAX) {
      // do nothing
    } else if (mixedPopulation[index1].getCrowdingDistance() == MEVO::DOUBLE_MAX) {
      elite.remove(index2);
      pool.push_back(index2);
      originalArray[index2] = -1;
    } else if (mixedPopulation[index2].getCrowdingDistance() == MEVO::DOUBLE_MAX) {
      elite.remove(index1);
      pool.push_back(index1);
      originalArray[index1] = -1;
    } else {
      dist1 = MEVO::DOUBLE_MAX;
	  std::list<unsigned int>::iterator iter;
      for (iter = elite.begin(); iter != elite.end(); ++iter) {
        index = *iter;
        if (index != index1 && index != index2) {
          if (dist1 > distance[originalArray[index1]][originalArray[index]]) {
            dist1 = distance[originalArray[index1]][originalArray[index]];
          }
        }
      }
      dist2 = MEVO::DOUBLE_MAX;
      for (iter = elite.begin(); iter != elite.end(); ++iter) {
        index = *iter;
        if (index != index1 && index != index2) {
          if (dist2 > distance[originalArray[index2]][originalArray[index]]) {
            dist2 = distance[originalArray[index2]][originalArray[index]];
          }
        }
      }
      if (dist1 < dist2) {
        elite.remove(index1);
        pool.push_back(index1);
        originalArray[index1] = -1;
      } else {
        elite.remove(index2);
        pool.push_back(index2);
        originalArray[index2] = -1;
      }
    }
  }
  if (elite.size() > desiredEliteSize) {
    while (elite.size() > desiredEliteSize) {
      pool.push_back(elite.front());
      elite.pop_front();
    }
  }
  delete[](indexArray);
  delete[](originalArray);
  for (register unsigned int i = 0; i < poolSize; i++) {
    delete[](distance[i]);
  }
  delete[](distance);
  return;
}

void AMGA2::extractExtremeSolutions(const std::vector<AMGA2Individual> &population, std::list<unsigned int> &pool, std::list<unsigned int> &extreme) {
  unsigned int *extremeSolution = new unsigned int[numObj];
  std::vector<std::list<unsigned int>::iterator> iterVector;
  iterVector.reserve(numObj);
  for (register unsigned int i = 0; i < numObj; i++) {
    iterVector.push_back(pool.begin());
  }
  std::list<unsigned int>::iterator iter;
  for (register unsigned int i = 0; i < numObj; i++) {
    iter = pool.begin();
    extremeSolution[i] = *iter;
	iterVector[i] = iter;
    ++iter;
    while (iter != pool.end()) {
      if (population[*iter].normalObj[i] > population[extremeSolution[i]].normalObj[i]) {
        extremeSolution[i] = *iter;
        iterVector[i] = iter;
      }
      ++iter;
    }
  }
  for (register unsigned int i = 0; i < numObj; i++) {
    for (register unsigned int j = i + 1; j < numObj; j++) {
      if (extremeSolution[j] != numEvaluations && extremeSolution[i] != numEvaluations) {
        if (extremeSolution[j] == extremeSolution[i]) {
          extremeSolution[j] = numEvaluations;
        }
      }
    }
  }
  for (register unsigned int i = 0; i < numObj; i++) {
    if (extremeSolution[i] != numEvaluations) {
      pool.erase(iterVector[i]);
      extreme.push_back(extremeSolution[i]);
    }
  }
  iterVector.clear();
  delete[](extremeSolution);
  return;
}

void AMGA2::fillBestPopulation(std::vector<AMGA2Individual> &mixedPopulation, unsigned int mixedLength, std::vector<AMGA2Individual> &population, unsigned int populationLength) {
  std::list<unsigned int> pool;
  std::list<unsigned int> elite;
  std::list<unsigned int> filled;
  register unsigned int rank = 1;
  for (register unsigned int i = 0; i < mixedLength; i++) {
    pool.push_back(i);
  }
  for (std::list<unsigned int>::iterator iter = pool.begin(); iter != pool.end(); ++iter) {
    mixedPopulation[*iter].setCrowdingDistance(0.0);
  }
  while (filled.size() < populationLength) {

    extractBestRank(mixedPopulation, pool, elite);
	for (std::list<unsigned int>::iterator iter = elite.begin(); iter != elite.end(); ++iter) {
      mixedPopulation[*iter].rank = rank;
    }
    if (rank == 1) {
      assignInfiniteDiversity(mixedPopulation, elite);
    }
    rank++;
    if (elite.size() + filled.size() < populationLength) {
	  for (std::list<unsigned int>::iterator iter = elite.begin(); iter != elite.end(); ++iter) {
        filled.push_back(*iter);
      }
      elite.clear();
    } else {
	  std::list<unsigned int> temp;
      extractENNSPopulation(mixedPopulation, elite, temp, populationLength - filled.size());
	  for (std::list<unsigned int>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
        filled.push_back(*iter);
      }
      temp.clear();
    }
  }
  register unsigned int i = 0;
  for (std::list<unsigned int>::iterator iter = filled.begin(); iter != filled.end(); ++iter) {
    population[i++] = mixedPopulation[*iter];
  }
  pool.clear();
  elite.clear();
  filled.clear();
  return;
}

void AMGA2::fillDiversePopulation(std::vector<AMGA2Individual> &mixedPopulation, std::list<unsigned int> &pool, std::vector<AMGA2Individual> &population, unsigned int startLocation, unsigned int desiredSize) {
  assignDiversityMetric(mixedPopulation, pool);
  register unsigned int poolSize = pool.size();
  unsigned int *indexArray = new unsigned int[poolSize];
  register unsigned int j = 0;
  for (std::list<unsigned int>::iterator iter = pool.begin(); iter != pool.end(); ++iter) {
    indexArray[j++] = *iter;
  }
  quickSortDiversity(mixedPopulation, indexArray, 0, poolSize - 1);
  for (register unsigned int i = 0; i < desiredSize; i++) {
    population[startLocation + i] = mixedPopulation[indexArray[poolSize - 1 - i]];
  }
  delete[](indexArray);
  return;
}

void AMGA2::mutateOffspringPopulation() {

 MEVO::CPopulation epop;
  for (unsigned int i=0; i< offspringPopulation.size();i++) {
      epop.addIndividual(&offspringPopulation[i]);
  }
  mutatePopulation(&epop);
}

void AMGA2::updateArchivePopulation() {
  if (currentArchiveSize + populationSize <= FArchiveSize) {
    for (register unsigned int j = currentArchiveSize, i = 0; i < populationSize; i++, j++) {
      archivePopulation[j] = offspringPopulation[i];
    }
    currentArchiveSize += populationSize;
  } else {
    for (register unsigned int i = 0; i < currentArchiveSize; i++) {
      combinedPopulation[i] = archivePopulation[i];
    }
    for (register unsigned int i = 0; i < populationSize; i++) {
      combinedPopulation[currentArchiveSize + i] = offspringPopulation[i];
    }
    fillBestPopulation(combinedPopulation, currentArchiveSize + populationSize, archivePopulation, FArchiveSize);
    currentArchiveSize = FArchiveSize;
  }
  for (register unsigned int i = 0; i < currentArchiveSize; i++) {
      archivePopulation[i].setCrowdingDistance(0.0);
  }
  FArchive->clearIndividuals();
  for(int i =0; i<currentArchiveSize; i++) {
     FArchive->addIndividual(&archivePopulation[i]);
  }

  return;
}

double AMGA2::calcEuclideanDistance(const AMGA2Individual &a, const AMGA2Individual &b) const {
  register double distance = 0.0;
  register double temp;
  for (register unsigned int i = 0; i < numObj; i++) {
    temp = (a.normalObj[i] - b.normalObj[i]);
    distance += temp*temp;
  }
  return (distance / numObj);
}

void AMGA2::quickSortDiversity(const std::vector<AMGA2Individual>& population, unsigned int* diverseArray, int left, int right) const {
  register int index = 0;
  register int temp = 0;
  register int i = 0;
  register int j = 0;
  register double pivot = 0.0;
  if (left < right) {
    pivot = population[diverseArray[right]].getCrowdingDistance();
    i = left - 1;
    for (j = left; j < right; j++) {
      if (population[diverseArray[j]].getCrowdingDistance() <= pivot) {
        i += 1;
        temp = diverseArray[j];
        diverseArray[j] = diverseArray[i];
        diverseArray[i] = temp;
      }
    }
    index = i + 1;
    temp = diverseArray[index];
    diverseArray[index] = diverseArray[right];
    diverseArray[right] = temp;
    quickSortDiversity(population, diverseArray, left, index - 1);
    quickSortDiversity(population, diverseArray, index + 1, right);
  }
  return;
}

void AMGA2::quickSortObj(const std::vector<AMGA2Individual>& population, unsigned int objCount, unsigned int* objArray, int left, int right) const {
  register int index = 0;
  register int temp = 0;
  register int i = 0;
  register int j = 0;
  register double pivot = 0.0;
  if (left < right) {
    pivot = population[objArray[right]].normalObj[objCount];
    i = left - 1;
    for (j = left; j < right; j++) {
      if (population[objArray[j]].normalObj[objCount] <= pivot) {
        i += 1;
        temp = objArray[j];
        objArray[j] = objArray[i];
        objArray[i] = temp;
      }
    }
    index = i + 1;
    temp = objArray[index];
    objArray[index] = objArray[right];
    objArray[right] = temp;
    quickSortObj(population, objCount, objArray, left, index - 1);
    quickSortObj(population, objCount, objArray, index + 1, right);
  }
  return;
}

void AMGA2::updateObjectiveConstraintRange() {
  for (register unsigned int i = 0; i < numObj; i++) {
    minObj[i] = archivePopulation[0].getObj()[i];
    maxObj[i] = archivePopulation[0].getObj()[i];
    for (register unsigned int j = 1; j < currentArchiveSize; j++) {
      if (archivePopulation[j].getObj()[i] < minObj[i]) {
        minObj[i] = archivePopulation[j].getObj()[i];
      }
      if (archivePopulation[j].getObj()[i] > maxObj[i]) {
        maxObj[i] = archivePopulation[j].getObj()[i];
      }
    }
  }
  if (numConstr > 0) {
    for (register unsigned int i = 0; i < numConstr; i++) {
      minConstr[i] = archivePopulation[0].getConstr()[i];
      maxConstr[i] = archivePopulation[0].getConstr()[i];
      for (register unsigned int j = 0; j < currentArchiveSize; j++) {
        if (archivePopulation[j].getConstr()[i] < minConstr[i]) {
          minConstr[i] = archivePopulation[j].getConstr()[i];
        }
        if (archivePopulation[j].getConstr()[i] > maxConstr[i]) {
          maxConstr[i] = archivePopulation[j].getConstr()[i];
        }
      }
    }
  }
  for (register unsigned int i = 0; i < currentArchiveSize; i++) {
    archivePopulation[i].rank = 0;
    archivePopulation[i].setCrowdingDistance(0.0);
  }
  return;
}



