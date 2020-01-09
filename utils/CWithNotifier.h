#ifndef CWITHNOTIFIER_H
#define CWITHNOTIFIER_H

#include <deque>

#include "../interfaces/ICNotifier.h"

namespace MEVO {

//! an auxilliary class for objects which have to communicate with a user
/** Class contains a notifier object and functions implementing the communication
  */
class CWithNotifiers
{
public:
    /** Constructor*/
    CWithNotifiers();

    /** Destructor*/
    ~CWithNotifiers();

    /** Notifier setter
        @param _notifier pointer to a notifier object
    */
    void addNotifier(ICNotifier* _notifier);

    /** Function clears and deletes all notifiers
    */
    void clearNotifiers();

    /** Notifier getter*/
    ICNotifier* getNotifier(unsigned int _i);

    /** On before optimization start notifications firerer
        @param _name a string which is passed to notifier
        @param _opt pointer to an optimizer object
    */
    void fireOnBeforeStartNotify(const char* _name, ICEvolOptimizer* _opt);

   /** On after stop notifications firerer
        @param _msg message from optimizer
        @param _name message from stop criterion after optimization was stopped
   */
    void fireOnAfterStopNotify(const char*_msg, ICPopulation* _fpop);

    /** Notifications shown after error detection
   @param _name error description
   */
    void fireOnErrorNotify(const char* _error);

   /** Notifications shown after generation evaluation
   @param _g generation number
   @param _pop pointer to a population object
   */
   void fireOnGenerationDoneNotify(unsigned int _g,  ICPopulation* _pop);

protected:
   /** Pointer to the notifier object*/
   std::deque<ICNotifier*> FNotifiers;
};
}
#endif // CWITHNOTIFIER_H
