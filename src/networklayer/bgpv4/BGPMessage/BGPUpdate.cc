//
// Copyright (C) 2010 Helene Lageber
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "BGPUpdate.h"

Register_Class(BGPUpdate)

void BGPUpdate::setWithdrawnRoutesArraySize(unsigned int size)
{
    unsigned short delta_size = size - getWithdrawnRoutesArraySize();
    unsigned short delta_bytes = delta_size * 5; // 5 = Withdrawn Route length
    setByteLength(getByteLength() + delta_bytes);
}

unsigned short BGPUpdate::computePathAttributesBytes(const BGPUpdatePathAttributeList& pathAttrs)
{
    unsigned short nb_path_attr = 2 + pathAttrs.getAsPathArraySize()
        + pathAttrs.getLocalPrefArraySize()
        + pathAttrs.getAtomicAggregateArraySize();

    // BGPUpdatePathAttributes (4)
    unsigned short contentBytes = nb_path_attr * 4;
    // BGPUpdatePathAttributesOrigin (1)
    contentBytes += 1;
    // BGPUpdatePathAttributesASPath
    for (unsigned int i=0; i<pathAttrs.getAsPathArraySize(); i++)
        contentBytes += 2 + pathAttrs.getAsPath(i).getLength(); // type (1) + length (1) + value
    // BGPUpdatePathAttributesNextHop (4)
    contentBytes += 4;
    // BGPUpdatePathAttributesLocalPref (4)
    contentBytes = 4 * pathAttrs.getLocalPrefArraySize();
    return contentBytes;
}

void BGPUpdate::setPathAttributeList(const BGPUpdatePathAttributeList& pathAttrs)
{
    unsigned int old_bytes = getPathAttributeListArraySize()==0 ? 0 : computePathAttributesBytes(getPathAttributeList(0));
    unsigned int delta_bytes = computePathAttributesBytes(pathAttrs) - old_bytes;

    setPathAttributeListArraySize(1);
    BGPUpdate_Base::setPathAttributeList(0, pathAttrs);

    setByteLength(getByteLength() + delta_bytes);
}

void BGPUpdate::setNLRI(const BGPUpdateNLRI& NLRI_var)
{
    setByteLength(getByteLength() + 5); //5 = NLRI (length (1) + IPAddress (4))
    BGPUpdate_Base::NLRI_var = NLRI_var;
}

