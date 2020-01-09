#ifndef ICQUALITYFNC_H
#define ICQUALITYFNC_H


namespace MEVO {
class ICProblem;
//! Interface class for various functions
/** Provides functions for implementation of different mathematical functions.
  */
class ICFunction
{
  public:
    /** Virtual destructor */
    virtual ~ICFunction() {};

    /** Virtual copy function. It should return the exact copy of the ICFunction object. It has to be defined!
    */
    virtual ICFunction* copy()=0;

    /** This method is called by an evaluator object. Calls calculate function and sets an evaluated function value in a problem object.
         This method have to be implemented in the derived classsses!
        @param _pf vector of double values of the decision variables
        @param _nf size of vector of double values of the decision variables
        @param _pi vector of integer values of the decision variables
        @param _ni size of vector of integer values of the decision variables
        @return double value of a calucated objective function
     */
    virtual double evaluate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni)=0;

    /** Description of selected algorithm
    @return brief description of a fuction
     */
    virtual const char *getDescription()=0;

    virtual void setProblem(ICProblem *_p)=0;

protected:
    /** This method calculates value of a function for given parameters. Might be used as objective or constraint function.
         This method have to be implemented in the derived classsses!
        @param _pf vector of double values of the decision variables
        @param _nf size of vector of double values of the decision variables
        @param _pi vector of integer values of the decision variables
        @param _ni size of vector of integer values of the decision variables
        @return double value of a calucated objective function
     */
    virtual double calculate(double* _pf, unsigned int _nf, int* _pi, unsigned int _ni)=0;

};
};
#endif
