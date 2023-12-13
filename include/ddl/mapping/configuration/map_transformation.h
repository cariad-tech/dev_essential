/**
 * @file
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 *
 */

#ifndef HEADER_MAP_TRANSFORMATION_H
#define HEADER_MAP_TRANSFORMATION_H

#include <a_util/result.h>
#include <a_util/strings.h>
#include <a_util/xml.h>

namespace ddl {
namespace mapping {

class MapConfiguration;

/**
 * cMapTransformation forms a base class for a single transformation
 */
class MapTransformationBase {
public:
    /**
     * CTOR
     * @param [in] config The configuration
     * @param [in] name The transformation name
     */
    MapTransformationBase(MapConfiguration* config, const std::string& name);

    /**
     * DTOR
     */
    virtual ~MapTransformationBase();

    /**
     * Returns the name of the transformation
     */
    const std::string& getName() const;

    /**
     * Returns the validity for the current description
     */
    bool isValid() const;

    /**
     * Polymorphic comparison method
     */
    virtual bool isEqual(const MapTransformationBase& other) const = 0;

    /**
     * Polymorphic clone method
     */
    virtual MapTransformationBase* clone() const = 0;

    /**
     * Polymorphic evaluation template method
     * @param [in] value The value to evaluate
     */
    virtual double evaluate(double value) const = 0;

private:
    /**
     * creates a polymorphic transformation instance from a dom element
     * @param [in] config The configuration
     * @param [in] dom_element The dom element from which to import
     * @param [out] destination The Transformation object to fill
     * @retval ERR_INVALID_ARG  Missing name or type
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    static a_util::result::Result createFromDOM(MapConfiguration* config,
                                                const a_util::xml::DOMElement& dom_element,
                                                MapTransformationBase*& destination);

    /**
     * creates a polymorphic transformation instance from a dom element
     * @param [in] config The mapping configuration
     * @param [in] name Name of the element
     * @param [in] type Type of the element
     * @param [out] destination The Transformation object to fill
     * @retval ERR_INVALID_ARG  Missing name or type
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    static a_util::result::Result create(MapConfiguration* config,
                                         const std::string& name,
                                         const std::string& type,
                                         MapTransformationBase*& destination);

    /**
     * Export transformation to a XML dom element
     *
     * @param [in] dom_element The dom element from which to import
     * @retval a_util::result::SUCCESS Everything went fine
     */
    a_util::result::Result writeToDOM(a_util::xml::DOMElement& dom_element) const;

    /**
     * Set Float values from Enumeration definition in DataDefinition-File
     * @retval ERR_INVALID_TYPE Inconsistency with ddl
     * @retval a_util::result::SUCCESS      Everything went fine
     **/
    a_util::result::Result setTypeFromDDL();

protected:
    /// @cond nodoc
    friend class MapConfiguration;
    MapConfiguration* _config;
    std::string _name;
    bool _is_valid;
    /// @endcond
};

/// MapPolynomTransformation represents a polynom transformation in the configuration api
/// as well as the actual implementation of the transformation in the evaluate method
class MapPolynomTransformation : public MapTransformationBase {
public:
    /**
     * CTOR
     * @param [in] config The configuration
     * @param [in] name The transformation name
     */
    MapPolynomTransformation(MapConfiguration* config, const std::string& name);

    /**
     * Returns parameter A of the polynomial
     */
    double getA() const;

    /**
     * Returns parameter A of the polynomial
     */
    double getB() const;

    /**
     * Returns parameter A of the polynomial
     */
    double getC() const;

    /**
     * Returns parameter A of the polynomial
     */
    double getD() const;

    /**
     * Returns parameter A of the polynomial
     */
    double getE() const;

    /**
     * Set Polynom coefficients
     * @param [in] coefs The list of coefficients
     * @retval ERR_INVALID_ARG  An attribute is not a float value
     * @retval a_util::result::SUCCESS      Everything went fine
     **/
    a_util::result::Result setCoefficients(const std::string coefs[5]);

    /**
     * @overload
     */
    bool isEqual(const MapTransformationBase& other) const;

    /**
     * @overload
     */
    double evaluate(double value) const;

    /// nodoc
    MapTransformationBase* clone() const;

private:
    /**
     * Initializes the transformation from a target-dom element
     * @param [in] dom_element The dom element from which to import
     * @retval ERR_INVALID_ARG  An attribute is not a float value
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result loadFromDom(const a_util::xml::DOMElement& dom_element);

    /**
     * Export transformation to a XML dom element
     *
     * @param [in] dom_element The dom element to be written
     * @retval a_util::result::SUCCESS Everything went fine
     */
    a_util::result::Result writeToDOM(a_util::xml::DOMElement& dom_element) const;

private:
    /// @cond nodoc
    friend class MapTransformationBase;
    double _a, _b, _c, _d, _e;
    /// @endcond
};

/// MapEnumTableTransformation represents a transformation between to enumerations
class MapEnumTableTransformation : public MapTransformationBase {
public: // types
#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

    /// Alias for the conversion map
    using ConversionMap = std::map<int64_t, int64_t>;

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

public:
    /**
     * CTOR
     * @param [in] config The configuration
     * @param [in] name The transformation name
     */
    MapEnumTableTransformation(MapConfiguration* config, const std::string& name);

    /**
     * Returns the name of the source enumeration
     */
    const std::string& getEnumFrom() const;

    /**
     * Returns the name of the target enumeration
     */
    const std::string& getEnumTo() const;

    /**
     * Set the name of the target and source enumerations
     * @param [in] enum_from input enumeration name
     * @param [in] enum_to output enumeration name
     * @retval ERR_INVALID_TYPE Enums not found in ddl
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result setEnums(const std::string& enum_from, const std::string& enum_to);

    /**
     * Returns default value
     */
    int64_t getDefault() const;

    /**
     * Returns default value as string
     */
    std::string getDefaultStr() const;

    /**
     * Set default value as string
     * @param [in] default_value string for default value
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result setDefault(const std::string& default_value);

    /**
     * Returns conversion list
     */
    const ConversionMap& getConversions() const;

    /**
     * Returns conversion list as string
     */
    const std::map<std::string, std::string>& getConversionsStr() const;

    /**
     * Add a conversion as string
     * @param [in] from string for source value
     * @param [in] to string for target value
     * @retval a_util::result::SUCCESS      Everything went fine
     **/
    a_util::result::Result addConversion(const std::string& from, const std::string& to);

    /**
     * Remove a conversion as string
     * @param [in] from string for source value
     * @retval a_util::result::SUCCESS      Everything went fine
     **/
    a_util::result::Result removeConversion(const std::string& from);

    /**
     * @overload
     */
    bool isEqual(const MapTransformationBase& other) const;

    /**
     * @overload
     */
    double evaluate(double value) const;

    /// nodoc
    MapTransformationBase* clone() const;

private:
    /**
     * Initializes the transformation from a target-dom element
     * @param [in] dom_element The dom element from which to import
     * @retval ERR_INVALID_ARG  Missing attribute
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result loadFromDom(const a_util::xml::DOMElement& dom_element);

    /**
     * Export transformation to a XML dom element
     *
     * @param [in] dom_element The dom element to be written
     * @retval a_util::result::SUCCESS Everything went fine
     */
    a_util::result::Result writeToDOM(a_util::xml::DOMElement& dom_element) const;

    /**
     * Convert string values to integer values
     *
     * The integer values were not filled before, as the ddl is necessary for this
     *
     * @retval ERR_INVALID_TYPE Inconsistency with ddl
     * @retval a_util::result::SUCCESS      Everything went fine
     **/
    a_util::result::Result convertValuesWithDDL();

    /**
     * Set the name of the target and source enumerations
     * @param [in] enum_from input enumeration name
     * @param [in] enum_to output enumeration name
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result setEnumsStr(const std::string& enum_from, const std::string& enum_to);

    /**
     * Set default value as string
     * @param [in] default_value string for default value
     * @retval a_util::result::SUCCESS      Everything went fine
     */
    a_util::result::Result setDefaultStr(const std::string& default_value);

    /**
     * Add a conversion as string
     * @param [in] from string for source value
     * @param [in] to string for target value
     * @retval a_util::result::SUCCESS      Everything went fine
     **/
    a_util::result::Result addConversionStr(const std::string& from, const std::string& to);

private:
    friend class MapTransformationBase;
    std::string _enum_from;
    std::string _enum_to;
    std::string _default_value;
    typedef std::map<std::string, std::string> MapStrConversionList;
    MapStrConversionList _conversions;
    int64_t _default_int;
    ConversionMap _conversions_int;
};

/// Public composite types used in the mapping::dd namespace
typedef std::vector<MapTransformationBase*> MapTransformationList;

} // namespace mapping
} // namespace ddl

#endif // HEADER_MAP_TRANSFORMATION_H
