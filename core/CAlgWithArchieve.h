#ifndef CALGWITHARCHIEVE_H
#define CALGWITHARCHIEVE_H


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

#include "../interfaces/ICPopulation.h"

namespace MEVO {

//! class for algorithms with archieve
/** Class for algorithms with archieve. Implements an archive and access methods to it.

  */
class CAlgWithArchive
{
public:

    /** Constructor */
    CAlgWithArchive();

    /** Destructor */
    virtual ~CAlgWithArchive();

    /** function sets maximum archieve size. Value = 0 means that archieve size is unlimited
      @param _size maximum archieve size
      */
    virtual void setArchiveSize(unsigned int _size);

    /** function returns maximum archieve size. Value = 0 means that archieve size is unlimited
      @return maximum archieve size
      */
    virtual unsigned int getArchiveSize();

    /** function returns a reference to archieve queue.
        @return reference to archieve deque
    */
    ICPopulation* getArchive();

protected:
    /** Archieve of individuals*/
    ICPopulation* FArchive;

    /** Maximum allowed size of an archieve*/
   unsigned int FArchiveSize;
};
}
#endif // CALGWITHARCHIEVE_H
