#ifndef ICNOTIFIER_H
#define ICNOTIFIER_H


#include "ICPopulation.h"

namespace MEVO {
//! Basic class interface for notifiers
/** Interface class
	Provides methods for notifier class methods
	This class is designed to present messages from the optimizer class
 */

class ICEvolOptimizer;

class ICNotifier {
  public:

	/** Default destructor */
    virtual ~ICNotifier(){}

	/** Notifications shown before optimization start
        @param _name message from optimizer
        @param _opt pointer to an optimizer object
	 */
    virtual void onBeforeStart(const char* _name, ICEvolOptimizer* _opt)=0;

	/** Notifications shown after optimization start
    @param _msg message from optimizer
    @param _fpop pointer to a final population object
	 */
    virtual void onAfterStop(const char*_msg, ICPopulation* _fpop)=0;

	/** Notifications shown after error detection
    @param _name error description
	 */
	virtual void onError(const char* _error)=0;

	/** Notifications shown after generation evaluation
    @param _g generation number
    @param _pop pointer to a population object
	 */
    virtual void onGenerationDone(unsigned int _g, ICPopulation* _pop)=0;

};
};
#endif
