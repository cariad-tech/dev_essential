/**
 * @file
 * OO DataDefinition Stream Object
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

#ifndef DD_DATA_MODEL_STREAMS_H_INCLUDED
#define DD_DATA_MODEL_STREAMS_H_INCLUDED

#include <ddl/datamodel/datamodel_base.h>
#include <ddl/datamodel/infomodel_base.h>
#include <ddl/dd/dd_common_types.h>
#include <ddl/utilities/dd_access_list.h>
#include <ddl/utilities/dd_access_map.h>
#include <ddl/utilities/dd_access_observer.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace ddl {

namespace dd {

namespace datamodel {

/**
 * @brief observable Stream DataDefinition object.
 *
 */
class Stream : public ModelSubject<Stream>,
               public utility::TypeAccessMapSubject<Stream>,
               public InfoMap {
public:
    /**
     * @brief no default CTOR.
     */
    Stream() = delete;
    /**
     * @brief move CTOR
     */
    Stream(Stream&&);
    /**
     * @brief move assignment operator
     *
     * @return Stream&
     */
    Stream& operator=(Stream&&);
    /**
     * @brief copy CTOR
     *
     * @param other the stream to copy from.
     */
    Stream(const Stream& other);
    /**
     * @brief copy assignment operator
     *
     * @param other the stream to copy from.
     * @return Stream&
     */
    Stream& operator=(const Stream& other);
    /**
     * @brief DTOR
     *
     */
    virtual ~Stream() = default;

    /**
     * @brief DataDefinition object class of datamodel to hold a streamstruct
     *
     */
    class Struct : public utility::TypeAccessListSubject<Struct>, public InfoMap {
    public:
        /**
         * @brief default CTOR
         *
         */
        Struct(){};
        /**
         * @brief copy CTOR
         *
         */
        Struct(const Struct&) = default;
        /**
         * @brief move CTOR
         *
         */
        Struct(Struct&&) = default;
        /**
         * @brief copy assignment operator.
         *
         * @return Struct&
         */
        Struct& operator=(const Struct&) = default;
        /**
         * @brief move assignment operator.
         *
         * @return Struct&
         */
        Struct& operator=(Struct&&) = default;
        /**
         * @brief CTOR
         *
         * @param name name of the stream struct (one single empty name is allowed (says xsd))
         * @param type_name the name of the type to use (must be a valid struct type)
         * @param byte_pos the first bytepos of the structtype (optional ... default is 0)
         */
        Struct(const std::string& name, const std::string& type_name, OptionalSize byte_pos = {});

        /**
         * @brief equality operator
         *
         * @param other the other streamstruct to compare the streamstruct with.
         * @return true the stream structs are equal
         * @return false the stream structs are not equal
         */
        bool operator==(const Struct& other) const;
        /**
         * @brief non-equality operator
         *
         * @param other the other streamstruct to compare the streamstruct with.
         * @return false the stream structs are equal
         * @return true the stream structs are not equal
         */
        bool operator!=(const Struct& other) const;

        /**
         * @brief Get the Name
         *
         * @return const std::string&
         */
        const std::string& getName() const;
        /**
         * @brief Set the Name
         *
         * @param name the name
         * @remark Is observable.
         * @throws may throw if the name is not unique within the container this struct is part of.
         */
        void setName(const std::string& name);

        /**
         * @brief Get the Type Name
         *
         * @return const std::string&
         */
        const std::string& getTypeName() const;
        /**
         * @brief Set the Type Name
         *
         * @param type_name the typename to set
         * @remark Is observable.
         */
        void setTypeName(const std::string& type_name);

        /**
         * @brief Get the Byte Pos
         *
         * @return OptionalSize (use 0 if not valid)
         */
        OptionalSize getBytePos() const;
        /**
         * @brief Set the Byte Pos
         *
         * @param byte_pos the byte pos to set
         * @remark Is observable.
         */
        void setBytePos(OptionalSize byte_pos);

    private:
        std::string _name;
        std::string _type_name;
        OptionalSize _byte_pos;
    };

    /**
     * @brief CTOR
     *
     * @param name name of the stream
     * @param stream_type_name  name of the stream type (may be a stucttype or a streammetatype)
     * @param description the description of the stream
     * @param structs vector of streamstructs that are added.
     * @throws ddl::dd::Error if a structs containing structs with the same name.
     */
    Stream(const std::string& name,
           const std::string& stream_type_name,
           const std::string& description = {},
           const std::vector<Struct>& structs = {});
    /**
     * @brief equality operator.
     *
     * @param other the other stream to compare this stream to.
     * @return true  the streams are equal.
     * @return false the streams are not equal.
     */
    bool operator==(const Stream& other) const;
    /**
     * @brief non equality operator.
     *
     * @param other the other stream to compare this stream to.
     * @return false the streams are equal.
     * @return true the streams are not equal.
     */
    bool operator!=(const Stream& other) const;

    /**
     * @brief Set the Name
     *
     * @param name the name
     * @remark This is observable.
     * @throws Throws if a stream with that name already exists in the container this stream is part
     * of.
     */
    void setName(const std::string& name);
    /**
     * @brief Get the Name
     *
     * @return const std::string&
     */
    const std::string& getName() const;
    /**
     * @brief Set the Stream Type Name
     *
     * @param stream_type_name the streamtype name (must be a structtype or a streammetatype)
     * @remark This is observable.
     */
    void setStreamTypeName(const std::string& stream_type_name);
    /**
     * @brief Get the Stream Type Name
     *
     * @return const std::string&
     */
    const std::string& getStreamTypeName() const;

    /**
     * @brief Get the Description
     *
     * @return const std::string&
     */
    const std::string& getDescription() const;
    /**
     * @brief Set the Description
     *
     * @param description the description to set
     * @remark This is observable.
     */
    void setDescription(const std::string& description);

public:
    /**
     * @brief Definiton of the container containts the streamstructs of a stream.
     *
     */
    using Structs = utility::TypeAccessList<Struct, Stream>;
    /// @cond nodoc
    friend Structs;
    /// @endcond nodoc

    /**
     * @brief Get the Structs object
     *
     * @return const Structs&
     */
    const Structs& getStructs() const;
    /**
     * @brief Get the Structs object
     *
     * @return Structs&
     */
    Structs& getStructs();

private:
    void notify(ModelEventCode code,
                utility::TypeAccessMapEventCode map_code,
                const std::string& additional_info);
    bool validateContains(const Structs::access_type& stream_struct) const;
    void notifyChangedListContent(utility::TypeAccessListEventCode code,
                                  Structs::access_type& stream_struct,
                                  const std::string& additional_info);
    const Structs::container_named_type* getNamedItemList() const;
    Structs::container_named_type* getNamedItemList();

    std::string _name;
    std::string _stream_type_name;
    std::string _description;
    Structs _structs;
};

} // namespace datamodel
} // namespace dd
} // namespace ddl

#endif // DD_DATA_MODEL_STREAMS_H_INCLUDED
