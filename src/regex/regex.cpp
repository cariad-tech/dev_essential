/**
 * @file
 * Regex library
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#include "a_util/regularexpression.h"
using namespace a_util;

// using this in member initialization list
#pragma warning(disable : 4355)

// cRegularExpression implementation
class cRegularExpression::cImplementation {
public:
    cRegularExpression* pSelf;

    cImplementation(cRegularExpression* pSelf) : pSelf(pSelf)
    {
    }
};

cRegularExpression::cRegularExpression() : pImpl(new cImplementation(this))
{
}

cRegularExpression::~cRegularExpression()
{
}

cRegularExpression::cRegularExpression(const cRegularExpression& oOther)
    : pImpl(new cImplementation(*oOther.pImpl))
{
    pImpl->pSelf = this;
}

cRegularExpression& cRegularExpression::operator=(const cRegularExpression& oOther)
{
    if (&oOther != this) {
        pImpl.reset(new cImplementation(*oOther.pImpl));
        pImpl->pSelf = this;
    }

    return *this;
}
