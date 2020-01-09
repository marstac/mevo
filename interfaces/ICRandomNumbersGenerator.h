#ifndef ICRANDOMNUMBERSGENERATOR_H
#define ICRANDOMNUMBERSGENERATOR_H
namespace MEVO {

//! Interface class for random numbers generator
/** Provides functions for implementation of different random numbers generators.
  */
class ICRandomNumbersGenerator
{
public:

    /** default destrucutor */
    virtual ~ICRandomNumbersGenerator(){}

    /** pure virtual function for copying a random numbers generator object
        this function is implemented in inherited classes.
        @return an exact copy of an random numbers generator object.
      */
    virtual ICRandomNumbersGenerator* copy() const=0;

    /** pure virtual function for setting a seed value
        @param _seed value of a seed
    */
    virtual void setSeed(unsigned long _seed)=0;

    /** pure virtual function for generating a random real value
        @param _l low value of selected bound for random value
        @param _h high value of selected bound for random value
    */
    virtual double randomDouble(double _l, double _h)=0;

    /** pure virtual function for generating a random integer value
        @param _l low value of selected bound for random value
        @param _h high value of selected bound for random value
    */
    virtual int  randomInteger(int _l, int _h)=0;

    /** pure virtual function which returns Random numbers generator type
        this function is implemented in inherited classes.
        @return unique name of a random numbers generator
    */
    virtual const char* getDescripton()=0;
};
}
#endif // ICRANDOMNUMBERSGENERATOR_H
