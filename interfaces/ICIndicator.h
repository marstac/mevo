#ifndef ICINDICATOR_H
#define ICINDICATOR_H


#include <deque>
#include "ICPopulation.h"
namespace MEVO{
//! Interface class for indication of a population
/** Provides functions for implementation of indication of a population quality.
  */
class ICIndicator {
public:
    ~ICIndicator(){};

    /** Function returns name of an indicator
      @return algorithm name
     */
    virtual const char* getName()=0;

    /** calculates a value of an indicator for a population. function is implemented in derived classes
        @param _population a pointer to a population of individuals
        @param _rf a reference pareto front
        @return a vaule of an indicator
    */
    virtual double assessPopulation(ICPopulation* _population)=0;

    /** function compares a population with the reference front. function is implemented in derived classes
        @param _population a pointer to a population of individuals
        @return a vaule of an indicator
    */
    virtual double comparePopWithFront(ICPopulation* _population, std::deque<std::deque<double> > _rf)=0;

    /** performes a comparison of the two populations. function is implemented in derived classes
        @param _population a pointer to a population of individuals
        @param _rf a reference pareto front
        @return a vaule of an indicator
    */
    virtual double comparePopulations(ICPopulation* _population1, ICPopulation* _population2)=0;

    /** performes assesement of the pareto fronts. function is implemented in derived classes
        @param _front a deque to a solution (each solution is a vector of objective function values
        @return a vaule of an indicator
    */
    virtual double assessFront(std::deque<std::deque<double> > _front)=0;

    /** performes a comparison of the two fronts. function is implemented in derived classes
        @param _front1 first pareto front
        @param _front2 second pareto front
        @return a vaule of an indicator
    */
    virtual double compareFronts(std::deque<std::deque<double> > _front1, std::deque<std::deque<double> > _front2)=0;
};
}
#endif // ICINDICATOR_H
