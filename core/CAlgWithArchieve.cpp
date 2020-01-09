

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

#include "CAlgWithArchieve.h"
#include "../utils/messages.h"
#include "../core/CPopulation.h"

namespace MEVO {
CAlgWithArchive::CAlgWithArchive()
{
    FArchiveSize=96;
    FArchive=new CPopulation();
}

CAlgWithArchive::~CAlgWithArchive() {
    FArchive->clearIndividuals();
    delete FArchive;
}

void CAlgWithArchive::setArchiveSize(unsigned int size) {
    if (size <= 0) {
    //   fireOnErrorNotify(MEVO::E_OPT_ARCH_SIZE);
    } else {
        FArchiveSize = size;
    }

}

unsigned int CAlgWithArchive::getArchiveSize() {
    return FArchiveSize;
}

ICPopulation*  CAlgWithArchive::getArchive() {
    return FArchive;
}
}
