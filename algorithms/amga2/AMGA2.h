#ifndef AMGA2_H
#define AMGA2_H


#include <vector>
#include <string>
#include <iostream>
#include <list>

#include "Individual.h"

#include "../../core/CGeneticAlgorithm.h"
#include "../../core/CAlgWithArchieve.h"

#include "../../core/CPopulation.h"

/** Archive-based Micro Genetic Algorithm 2*/
class AMGA2 : virtual public MEVO::CGeneticAlgorithm, virtual public MEVO::CAlgWithArchive {
  public:
    /** Archive-based Micro Genetic Algorithm 2 constructor*/
    AMGA2();
    ~AMGA2();
    virtual const char* getName() { return "AMGA2";}
    virtual void run();

    bool checkConf();

  private:


    void alocateMemory();
    void evolvePopulation();
  private:
	//void printPopulation();
    void assignDiversityMetric(std::vector<AMGA2Individual> &population, std::list<unsigned int> &elite);
    void assignInfiniteDiversity(std::vector<AMGA2Individual> &population, std::list<unsigned int> &elite);
    void createInitialPopulation();
    void createParentPopulation();
    void createOffspringPopulation();
    void finalizePopulation();
    void evaluate_Population(std::vector<AMGA2Individual>& population, unsigned int size);
    void evaluateIndividual(AMGA2Individual * individual);
    void normalizePopulation(std::vector<AMGA2Individual>& population, unsigned int size);
    void normalizeIndividualValues(AMGA2Individual * individual);
    void extractBestRank(std::vector<AMGA2Individual>& population, std::list<unsigned int>& pool, std::list<unsigned int>& elite);
    void extractDistinctIndividuals(std::vector<AMGA2Individual> &population, std::list<unsigned int> &elite, std::list<unsigned int> &distinct);
    void extractENNSPopulation(const std::vector<AMGA2Individual> &population, std::list<unsigned int> &pool, std::list<unsigned int> &elite, unsigned int desiredEliteSize);
    void extractExtremeSolutions(const std::vector<AMGA2Individual> &population, std::list<unsigned int> &pool, std::list<unsigned int> &extreme);
    void fillBestPopulation(std::vector<AMGA2Individual> &mixedPopulation, unsigned int mixedLength, std::vector<AMGA2Individual> &population, unsigned int desiredBestSize);
    void fillDiversePopulation(std::vector<AMGA2Individual> &mixedPopulation, std::list<unsigned int> &pool, std::vector<AMGA2Individual> &population, unsigned int startLocation, unsigned int desiredSize);
    void mutateOffspringPopulation();
    void updateArchivePopulation();
    void quickSortObj(const std::vector<AMGA2Individual>& population, unsigned int objCount, unsigned int* objArray, int left, int right) const;
    void quickSortDiversity(const std::vector<AMGA2Individual>& population, unsigned int* diversityArray, int left, int right) const;
    void updateObjectiveConstraintRange();
   // void printObjectives(const std::vector<AMGA2Individual>& population, unsigned int size, const std::string& fileName) const;
    double calcEuclideanDistance(const AMGA2Individual &a, const AMGA2Individual &b) const;

  private:
    unsigned int numVar;

	unsigned int numIntVar;

	double *minIntVar;
	double *maxIntVar;

	unsigned int numObj;
    unsigned int numConstr;
	double *minVar;
	double *maxVar;
    double *minObj;
    double *maxObj;
	double *minConstr;
    double *maxConstr;
    unsigned int initialSize;
    unsigned int populationSize;
    unsigned int eliteSize;
    unsigned int numEvaluations;

    unsigned int evaluationCounter;
    std::vector<AMGA2Individual> offspringPopulation;
    std::vector<AMGA2Individual> parentPopulation;
    std::vector<AMGA2Individual> archivePopulation;
    std::vector<AMGA2Individual> combinedPopulation;
    unsigned int currentArchiveSize;
    bool isEvolutionEnded;

    class DistanceMatrix {
      public:
        int index1;
        int index2;
        double distance;

        DistanceMatrix() : index1(-1), index2(-1), distance(0.0) {
        }

        DistanceMatrix(int m_index1, int m_index2, double m_distance) : index1(m_index1), index2(m_index2), distance(m_distance) {
        }

        DistanceMatrix(const DistanceMatrix& rhs) : index1(rhs.index1), index2(rhs.index2), distance(rhs.distance) {
        }

        ~DistanceMatrix() {
        }

        const DistanceMatrix & operator=(const DistanceMatrix& rhs) {
          if (this == &rhs) {
            return (*this);
          }
          distance = rhs.distance;
          index1 = rhs.index1;
          index2 = rhs.index2;
          return (*this);
        }

        bool operator<(const DistanceMatrix& o) const {
          if (distance < o.distance) {
            return (true);
          } else if (distance > o.distance) {
            return (false);
          } else if (index1 < o.index1) {
			return (true);
          } else if (index1 > o.index1) {
            return (false);
          } else if (index2 < o.index2) {
            return (true);
          } else if (index2 > o.index2) {
            return (false);
          } else {
            return (false);
          }
        }
    };
};

#endif
