#ifndef CEVALUATOR_H
#define CEVALUATOR_H
#include "../interfaces/ICEvaluator.h"
#include "../interfaces/ICProblem.h"
#include "../interfaces/ICMutator.h"
namespace MEVO {

//! Base class for evaluation of a population
/** Provides functions for implementation of evaluation task.
  */
class CEvaluator : public ICEvaluator
{
public:
    /** Constructor */
    CEvaluator();

    /** Destructor */
    virtual ~CEvaluator();

    /** function for initializing a computation object
            copies all objective and constraints functions from evolutionary algorithm object
            @param _opt pointer to a evolutionary algorithm.
          */
    virtual void init(ICEvolOptimizer* _opt);

    /** function which returns computation type
        @return unique name of a computation type
    */
    virtual const char* getDescripton(){return "Population Evaluator";}

    /** function for execution of a evaluation of population.
        evaluates all individuals in a population.
        @param _params population to evaluate
      */
    virtual void execute(void* _params);

    /** function for copying a evaluation object
        copies all objective and constraint functions
        @return an exact copy of an evaluation object.
      */
    virtual ICComputation* copy();

    /** function for evaluation of a population
         evaluates all individuals in a population.
         @param _population reference to a population
    */
    virtual void evaluatePopulation(ICPopulation* _population);

protected:
    /** This method calculates objective function value
      @param _pf vector of double values of the decision variables
      @param _nf size of vector of double values of the decision variables
      @param _pi vector of integer values of the decision variables
      @param _ni size of vector of integer values of the decision variables
      @param _i number of the calculated objective functions
      @return double value of a calucated objective function
    */
    double calculateObjectiveFnc(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni, unsigned int _i);

    /** This method calculates constraint function value
      @param _pf vector of double values of the decision variables
      @param _nf size of vector of double values of the decision variables
      @param _pi vector of integer values of the decision variables
      @param _ni size of vector of integer values of the decision variables
      @param _i number of the calculated objective functions
      @return double value of a calucated objective function
    */
    double calculateConstraintFnc(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni, unsigned int _i);

    /** function for evaluaing a single individual.
      calculates all objective and constraint functions values.
      @param _ind pointer to an individual
      */
    void evaluateIndividual(ICIndividual* _ind);

    /*Pointer to a problem object*/
    ICProblem* FProblem;

};
}
#endif // CEVALUATOR_H
