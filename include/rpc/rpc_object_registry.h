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
@endverbatim
 */

#ifndef PKG_RPC_OBJECTSERVER_REGISTRY_H_INCLUDED
#define PKG_RPC_OBJECTSERVER_REGISTRY_H_INCLUDED

#include <a_util/concurrency/shared_mutex.h>
#include <a_util/result/result_type.h>
#include <rpc/rpc_server.h>

#include <map>

namespace rpc {

/**
 * An RPC Server that receives calls via HTTP.
 */
class cRPCObjectsRegistry : public IRPCObjectsRegistry {
public: // types
    /// Type granting thread safe access to an object of type @ref rpc::IRPCObject
    typedef std::pair<std::unique_ptr<a_util::concurrency::shared_mutex>, IRPCObject*> tRPCItem;

public:
    /**
     * @copydoc IRPCObjectsRegistry::RegisterRPCObject
     */
    virtual a_util::result::Result RegisterRPCObject(const char* strName, IRPCObject* pObject);

    /**
     * @copydoc IRPCObjectsRegistry::UnregisterRPCObject
     */
    virtual a_util::result::Result UnregisterRPCObject(const char* strName);

    /**
     * Implements thread safe access to the @ref rpc::IRPCObject object
     */
    class cLockedRPCObject final {
        typedef std::pair<a_util::concurrency::shared_mutex*, IRPCObject*> tRPCRawItem;
        tRPCRawItem m_oItem;

    public:
        /// Default CTOR
        cLockedRPCObject() = default;
        /**
         * Construct with an @ref tRPCItem and lock the shared mutex
         * @param[in] oItem The rpc item to gain exclusive access on
         */
        cLockedRPCObject(const tRPCItem& oItem);
        /**
         * DTOR, unlocking any shared lock if previously locked
         */
        ~cLockedRPCObject();
        /**
         * Create a copy from @c other and add lock to the shared mutex
         * @param[in] other Other object to create *this from
         */
        cLockedRPCObject(const cLockedRPCObject& other);
        /**
         * Copy assignment from @c other
         * @param[in] other Other object to assign *this from
         */
        cLockedRPCObject& operator=(const cLockedRPCObject& other);
        /// Default move construction
        cLockedRPCObject(cLockedRPCObject&&) = default;
        /// Default move assignment
        cLockedRPCObject& operator=(cLockedRPCObject&&) = default;
        /**
         * Pointer like access to rpc object protected by *this via a shared lock
         * @return Pointer to locked rpc object. Might be @c nullptr.
         */
        IRPCObject* operator->();
        /**
         * Bool conversion operator
         * @return @c true if the rpc item is not @c nullptr, @c false otherwise.
         */
        operator bool() const;
    };

    /**
     * Get thread safe rpc object access by the rpc objects name
     * @param[in] strName Name of the rpc object
     * @return The locked rpc object, might be empty if the name could not be looked up.
     */
    virtual cLockedRPCObject GetRPCObject(const char* strName) const;

private:
    mutable a_util::concurrency::shared_mutex m_oObjectsLock;
    typedef std::map<std::string, tRPCItem> tRPCObjects;
    tRPCObjects m_oRPCObjects;
};

} // namespace rpc

#endif // PKG_RPC_OBJECTSERVER_REGISTRY_H_INCLUDED
