#ifndef ICEVOLOPTIMIZER_H
#define ICEVOLOPTIMIZER_H


#include "ICStop.h"
#include "ICProblem.h"
#include "ICComputer.h"
#include "ICEvaluator.h"
#include "ICInitializer.h"
#include "ICMutator.h"

namespace MEVO {

class ICEvolOptimizer {
public:
  /** default destructor
  */
  virtual ~ICEvolOptimizer(){};

   /** Function initating fields of this, basic class
  */
  virtual void initVariables()=0;


  /** Sets a number of the individuals in the population
   @param _v number of the individuals in the population
  */
  virtual void setPopSize(unsigned int _v)=0;

  /**
   Returns number of the individuals in the population
   @return number of the individuals in the population
  */
  virtual unsigned int getPopSize()=0;

  /** Returns no. of current generation for which the EA will let the population evolve
  @return number of generations
  */
  virtual unsigned int getCurrentGeneration()=0;

  /** Function starts optimization*/
   virtual void run()=0;

   /** Function check algorithm configuration
  @return true if an algorithm is properly configured
      false if it is not
   */
  virtual bool checkConf()=0;

   /** Function terminates calculations before assumed calculations */
   virtual void terminate()=0;

   /** Function returns termination status
    @return flag value
    */
   virtual bool isTerminated()=0;

   /** Function returns name of an algorithm
     @return algorithm name
    */
   virtual const char* getName()=0;

    /** Function sets last population evaluated
    @param _p last population evaluated by an algorithm
    */
   virtual void setLastPop(ICPopulation* _p)=0;

   /** Function returns last evaluated population
     @return last population of individuals
     */
   virtual ICPopulation* getLastPop()=0;

   /** sets computer object
    @param _c pointer to a computer object
    */
   virtual void setComputer(ICComputer* _c)=0;

   /** gets computer object
    @return pointer to a computer object
    */
   virtual ICComputer* getComputer()=0;

   /** sets evaluator object
    @param _e pointer to an evaluator object
    */
   virtual void setEvaluator(ICEvaluator* _e)=0;

   /** function evaluates a population
     calls evaluator object
    @param _population reference to a population of individuals
    */
   virtual void evaluatePopulation(ICPopulation* _population)=0;

   /** Methods sets a single initializer for both integer and real coded variables. The genotype will be treated as one vector!
     This methods deletes separate initializers for real and integer coded variables!
     @param _i pointer to an initializer object
     */
   virtual void setInitializer(ICInitializer* _i)=0;

    /** Method for mutation
      @param _population a population that will be mutated

    */
    virtual void mutatePopulation(ICPopulation *_population)=0;

    /** Methods sets a single mutator for both integer and real coded variables. The genotype will be treated as one vector!
      This methods deletes separate mutators for real and integer coded variables!
      @param _c pointer to a mutator object
      */
    virtual void setMutator(ICMutator* _m)=0;

    /** Methods sets a separate mutators for integer and real coded variables. The genotype will be treated as two vectors (Real coded and integer coded)!
      This methods deletes a single mutator for real and integer coded variables!
      This method works only if both arguments do not point to NULL!
      @param pointer to a mutator object
      */
    virtual void setSeparateMutators(ICMutator* _rm, ICMutator* _im)=0;


   /** A coeficcient for epsilon domination calculations setter
    @param _eps epsilon value
    */
   virtual void setEpsilon(double _eps)=0;

   /** A coeficcient for epsilon domination calculations getter
    @return _eps epsilon value
    */
   virtual double getEpsilon()=0;

   /** Methods sets a separate initializers for integer and real coded variables. The genotype will be treated as two vectors (Real coded and integer coded)!
     This methods deletes  a single initializer for real and integer coded variables!
     This method works only if both arguments do not point to NULL!
     @param pointer to an initializer object
     */
   virtual void setSeparateInitializers(ICInitializer* _ri, ICInitializer* _ii)=0;

   /** Method returns a pointer to an initializer object for both integer and real coded variables
       @return pointer to mutator object
   */
   virtual ICInitializer* getInitializer()=0;

   /** Method returns a pointer to an initializer object for integer coded variables
       @return pointer to mutator object
   */
   virtual ICInitializer* getIntInitializer()=0;

   /** Method returns a pointer to an initializer object for real coded variables
       @return pointer to mutator object
   */
   virtual ICInitializer* getRealInitializer()=0;

    /** Method returns a pointer to a mutator object for both integer and real coded variables
        @return pointer to mutator object
    */
    virtual ICMutator* getMutator()=0;

    /** Method returns a pointer to a mutator object for integer coded variables
        @return pointer to mutator object
    */
    virtual ICMutator* getIntMutator()=0;

    /** Method returns a pointer to a mutator object for real coded variables
        @return pointer to mutator object
    */
    virtual ICMutator* getRealMutator()=0;

   /** Function sets initial population
    @param _pop pointer to an initial population
    */
   virtual void setInitialPopulation(ICPopulation* _pop)=0;

   /** Function gets initial population
    @return pointer to an initial population
    */
   virtual ICPopulation* getInitialPopulation()=0;

   /** function creates a random initial population
        calls initalizer object
       @param _population reference to a population of individuals
    */
   virtual void initializePopulation(ICPopulation* _population)=0;

   /** Function sets stop condition object
    @param _s pointer to a stop condition object
    */
   virtual void setStop(ICStop* _s)=0;

   /** Function returns stop condition object
    @return pointer to a stop condition object
    */
   virtual ICStop* getStop()=0;

   /** This function provides calculation of stop condition
   @param _population reference to a last population generated by an algorithm
   @return boolean value of stop criterion (TRUE if the algorithm is to be finished)
   */
   virtual bool checkStop(ICPopulation* _population)=0;

   /** Function returns an optimization problem
    @return pointer to an optimization problem
    */
   virtual ICProblem *getProblem()const=0;

   /** Function sets an optimization problem
    @param pointer to an optimization problem
    */
   virtual void setProblem(ICProblem *_p)=0;
};
}
#endif // ICEVOLOPTIMIZER_H
