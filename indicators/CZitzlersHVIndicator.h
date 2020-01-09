#ifndef CZITZLERSHVINDICATOR_H
#define CZITZLERSHVINDICATOR_H

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



#include "CHyperVolumeIndicator.h"
#include "../utils/functions.h"

//! class for indication of a population in terms of hypervolume
/** class for indication of a population in terms of hypervolume. Original algorithm proposed by Zitzler.
  */
namespace MEVO {
class CZitzlersHVIndicator : public CHyperVolumeIndicator
{
public:
    /** Constructor */
    CZitzlersHVIndicator();


    /** Destructor */
    ~CZitzlersHVIndicator();

    /** Function returns name of an indicator
      @return algorithm name
     */
    virtual const char* getName();
protected:

    double calculateHypervolume(std::deque< std::deque<double> > &_front);

    void move(std::deque< std::deque<double> > &_front);

    double calculateHypervolume(std::deque< std::deque<double> > &front, int _pts, int _obj);
    bool dominates(std::deque<double> _s1, std::deque<double> _s2, int _objNo);
    void swap(std::deque< std::deque<double> >& _front, int _i, int _j);
    int reduceNondominatedSet(std::deque<std::deque<double> > &_front, int _noPoints, int _obj, double _threshold);
    int filterNondominatedSet(std::deque<std::deque<double> > &_front, int _noPoints, int _objNo);
    double surfaceUnchangedTo(std::deque< std::deque<double> > &_front, int _noPoints, int _obj);
};
}

#endif // CZITZLERSHVINDICATOR_H
