#ifndef CCONSOLENOTIFIER_H
#define CCONSOLENOTIFIER_H

/*  Author:
  Marcin A. Stachura <m.stachura@mchtr.pw.edu.pl>

  Copyright (c) 2015 Marcin A. Stachura

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include "CNotifier.h"
namespace MEVO {
class CConsoleNotifier : public CNotifier
{
public:
    CConsoleNotifier();
    ~CConsoleNotifier();
    /** Notifications shown before optimization start
        @param _name message from optimizer
        @param _opt pointer to an optimizer object
     */
    virtual void onBeforeStart(const char* _name, ICEvolOptimizer* _opt);

    /** Notifications shown after optimization start
    @param _msg message from optimizer
    @param _fpop pointer to a final population object
     */
    virtual void onAfterStop(const char*_msg, ICPopulation* _fpop);

    /** Notifications shown after error detection
    @param _name error description
     */
    virtual void onError(const char* _error);

    /** Notifications shown after generation evaluation
    @param _g generation number
    @param _pop pointer to a population object
     */
    virtual void onGenerationDone(unsigned int _g, ICPopulation* _pop);
    unsigned int getFFrequency() const;
    void setFFrequency(unsigned int value);

private:
    unsigned int FFrequency;
};
}
#endif // CCONSOLENOTIFIER_H
