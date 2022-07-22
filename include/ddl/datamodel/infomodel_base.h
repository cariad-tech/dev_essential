/**
 * @file
 * OO DataDefinition Redesign - Data model info extension
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

#ifndef DD_DATA_MODEL_INFO_H_INCLUDED
#define DD_DATA_MODEL_INFO_H_INCLUDED

#include <a_util/preprocessor/detail/disable_warnings.h>
#include <ddl/dd/dd_error.h>
#include <ddl/utilities/dd_access_observer.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace ddl {

namespace dd {

namespace datamodel {

/**
 * @brief Basic Info interface can be added to the @ref InfoMap
 */
class IInfo {
public:
    /**
     * @brief virtual DTOR for IInfo object
     *
     */
    virtual ~IInfo() = default;

    /**
     * @brief Get the Info Type ID
     *
     * @return uint8_t the Type ID
     */
    virtual uint8_t getInfoType() const = 0;
};

/**
 * @brief Helper template class to create valid IInfo objects
 *
 * @tparam INFO_T the info class type this IInfo interface instance is implemented for.
 */
template <typename INFO_T>
class Info : public IInfo {
protected:
    /**
     * @brief Get the Info Type object
     *
     * @return uint8_t returns the type for the info
     * @see
     */
    uint8_t getInfoType() const override
    {
        return INFO_T::INFO_TYPE_ID;
    }
};

/**
 * @brief Info Map for the datamodel to hold a set of optional @ref IInfo instances.
 * Only one instance of one Info type is possible.
 */
class InfoMap {
public:
    /**
     * @brief CTOR
     */
    InfoMap() = default;
    /**
     * @brief copy CTOR
     * @remark InfoMaps will be never copied!
     */
    InfoMap(const InfoMap&){};
    /**
     * @brief move CTOR
     * @remark InfoMaps will be never moved!
     */
    InfoMap(InfoMap&&){};
    /**
     * @brief copy assignment operator
     * @remark InfoMaps will be never copied!
     * @return InfoMap&
     */
    InfoMap& operator=(const InfoMap&)
    {
        return *this;
    };
    /**
     * @brief move assignment operator
     * @remark InfoMaps will be never moved!
     * @return InfoMap&
     */
    InfoMap& operator=(InfoMap&&)
    {
        return *this;
    };
    /**
     * @brief Get the Info Pointer
     *
     * @tparam INFO_T The Info type class type to retrieve.
     * @return const INFO_T*
     */
    template <typename INFO_T>
    const INFO_T* getInfo() const
    {
        return dynamic_cast<const INFO_T*>(getInfo(INFO_T::INFO_TYPE_ID));
    }
    /**
     * @brief Get the Info Pointer
     *
     * @tparam INFO_T The Info type class type to retrieve.
     * @return INFO_T*
     */
    template <typename INFO_T>
    INFO_T* getInfo()
    {
        return dynamic_cast<INFO_T*>(getInfo(INFO_T::INFO_TYPE_ID));
    }
    /**
     * @brief Set the Info object as shared pointer.
     *
     * @tparam INFO_T The Info type class type to set.
     * @param info the info pointer to set.
     * @throws ddl::dd::Error if samething went wrong while setting the info.
     */
    template <typename INFO_T>
    void setInfo(const std::shared_ptr<INFO_T>& info)
    {
        std::shared_ptr<IInfo> info_downcast = info;
        if (info_downcast->getInfoType() == INFO_T::INFO_TYPE_ID) {
            setInfo(info_downcast);
        }
        else {
            throw Error(
                "InfoMap::setInfo",
                "Registration not possible, runtime info is differnt then compiletime info");
        }
    }

private:
    IInfo* getInfo(uint8_t info_type)
    {
        auto found = _infos.find(info_type);
        if (found != _infos.end()) {
            return found->second.get();
        }
        else {
            return nullptr;
        }
    }
    const IInfo* getInfo(uint8_t info_type) const
    {
        auto found = _infos.find(info_type);
        if (found != _infos.cend()) {
            return found->second.get();
        }
        else {
            return nullptr;
        }
    }
    void setInfo(const std::shared_ptr<IInfo>& info)
    {
        _infos[info->getInfoType()] = info;
    }

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

    std::unordered_map<uint8_t, std::shared_ptr<IInfo>> _infos;

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
};

} // namespace datamodel
} // namespace dd
} // namespace ddl

#endif // DD_DATA_MODEL_INFO_H_INCLUDED
