/**
 * @file
 * RPC Protocol declaration.
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef PKG_RPC_RPC_SERVER_H_INCLUDED
#define PKG_RPC_RPC_SERVER_H_INCLUDED

#include "a_util/result.h"

namespace rpc {

/** @cond INTERNAL_DOCUMENTATION */
_MAKE_RESULT(0, NoError);
_MAKE_RESULT(1, AlreadyRegistered);
_MAKE_RESULT(2, NotFound);
_MAKE_RESULT(3, StartupFailed);
_MAKE_RESULT(4, InvalidURL);
_MAKE_RESULT(5, InvalidCall);
_MAKE_RESULT(6, FatalError);
/** @endcond */

/**
 * Interface for returning a response to a remote call.
 */
class IResponse {
public:
    /**
     * Sets the response data
     * @param[in] strResponse The reponse.
     * @param[in] nResponseSize The size of the response
     */
    virtual void Set(const char* strResponse, size_t nResponseSize) = 0;
};

/**
 * Interface of an object that support remote calls
 */
class IRPCObject {
public:
    /**
     * Handles a remote call.
     * @param[in] strRequest The call request.
     * @param[in] nRequestSize The size of the call request.
     * @param[out] oResponse The interface for returning the response.
     * @return Standard result.
     */
    virtual a_util::result::Result HandleCall(const char* strRequest,
                                              size_t nRequestSize,
                                              IResponse& oResponse) = 0;
};

/**
 * Interface for an RPC server that handles multiple RPC objects
 */
class IRPCObjectsRegistry {
public:
    /**
     * Register an RPC object
     * @param[in] strName The name of the object.
     * @param[in] pObject The object.
     * @return Standard result.
     */
    virtual a_util::result::Result RegisterRPCObject(const char* strName, IRPCObject* pObject) = 0;

    /**
     * Unregisters an RPC object
     * @param[in] strName The name of the object.
     * @return Standard result.
     */
    virtual a_util::result::Result UnregisterRPCObject(const char* strName) = 0;
};

} // namespace rpc

#endif // PKG_RPC_RPC_SERVER_H_INCLUDED
