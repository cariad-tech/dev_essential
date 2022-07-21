/**
 * @file
 * static_assert fallback implementation test
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

#include <a_util/base/delegate.h>

#include <gtest/gtest.h>

class TestType;
TestType& getTestType();

class TestType {
public:
    enum FunctionType {
        nullary_void_function = 0x00,
        nullary_value_function = 0x01,
        nullary_reference_function = 0x02,
        nullary_pointer_function = 0x03,
        unary_void_function = 0x04,
        unary_value_function = 0x05,
        unary_reference_function = 0x06,
        unary_pointer_function = 0x07,
        nullary_void_method = 0x08,
        nullary_value_method = 0x09,
        nullary_reference_method = 0x10,
        nullary_pointer_method = 0x11,
        unary_void_method = 0x12,
        unary_value_method = 0x13,
        unary_reference_method = 0x14,
        unary_pointer_method = 0x15
    };

    void nullaryVoid()
    {
        setLastCalledFunctionType(TestType::nullary_void_method);
    }

    TestType nullaryValue()
    {
        setLastCalledFunctionType(TestType::nullary_value_method);
        return getTestType();
    }

    TestType& nullaryReference()
    {
        setLastCalledFunctionType(TestType::nullary_reference_method);
        return getTestType();
    }

    TestType* nullaryPointer()
    {
        setLastCalledFunctionType(TestType::nullary_pointer_method);
        return &getTestType();
    }

    void unaryVoid(int)
    {
        setLastCalledFunctionType(TestType::unary_void_method);
    }

    TestType unaryValue(int)
    {
        setLastCalledFunctionType(TestType::unary_value_method);
        return getTestType();
    }

    TestType& unaryReference(int)
    {
        setLastCalledFunctionType(TestType::unary_reference_method);
        return getTestType();
    }

    TestType* unaryPointer(int)
    {
        setLastCalledFunctionType(TestType::unary_pointer_method);
        return &getTestType();
    }

public:
    static FunctionType getLastCalledFunctionType()
    {
        return getTestType()._function_type;
    }

    static void setLastCalledFunctionType(FunctionType function_type)
    {
        getTestType()._function_type = function_type;
    }

private:
    FunctionType _function_type;
};

TestType& getTestType()
{
    static TestType test_type_instance;
    return test_type_instance;
}

void nullaryVoid()
{
    TestType::setLastCalledFunctionType(TestType::nullary_void_function);
}

TestType nullaryValue()
{
    TestType::setLastCalledFunctionType(TestType::nullary_value_function);
    return getTestType();
}

TestType& nullaryReference()
{
    TestType::setLastCalledFunctionType(TestType::nullary_reference_function);
    return getTestType();
}

TestType* nullaryPointer()
{
    TestType::setLastCalledFunctionType(TestType::nullary_pointer_function);
    return &getTestType();
}

void unaryVoid(int)
{
    TestType::setLastCalledFunctionType(TestType::unary_void_function);
}

TestType unaryValue(int)
{
    TestType::setLastCalledFunctionType(TestType::unary_value_function);
    return getTestType();
}

TestType& unaryReference(int)
{
    TestType::setLastCalledFunctionType(TestType::unary_reference_function);
    return getTestType();
}

TestType* unaryPointer(int)
{
    TestType::setLastCalledFunctionType(TestType::unary_pointer_function);
    return &getTestType();
}

TEST(delegate_tests, TestNullaryFunctionCall)
{
    using namespace a_util; // experimental::NullaryDelegate

    {
        experimental::NullaryDelegate<void> delegate(nullaryVoid);
        delegate();
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::nullary_void_function);
    }

    {
        experimental::NullaryDelegate<TestType> delegate(nullaryValue);
        delegate();
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::nullary_value_function);
    }

    {
        experimental::NullaryDelegate<TestType&> delegate(nullaryReference);
        delegate();
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::nullary_reference_function);
    }

    {
        experimental::NullaryDelegate<TestType*> delegate(nullaryPointer);
        delegate();
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::nullary_pointer_function);
    }
}

TEST(delegate_tests, TestInvalidNullaryFunctionCall)
{
    using namespace a_util; // experimental::NullaryDelegate

    experimental::NullaryDelegate<TestType*> delegate(nullptr);
    EXPECT_FALSE(delegate);
    EXPECT_THROW(delegate(), std::runtime_error);
}

TEST(delegate_tests, TestUnaryFunctionCall)
{
    using namespace a_util; // experimental::UnaryDelegate

    {
        experimental::UnaryDelegate<void, int> delegate(unaryVoid);
        delegate(0);
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::unary_void_function);
    }

    {
        experimental::UnaryDelegate<TestType, int> delegate(unaryValue);
        delegate(0);
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::unary_value_function);
    }

    {
        experimental::UnaryDelegate<TestType&, int> delegate(unaryReference);
        delegate(0);
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::unary_reference_function);
    }

    {
        experimental::UnaryDelegate<TestType*, int> delegate(unaryPointer);
        delegate(0);
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::unary_pointer_function);
    }
}

TEST(delegate_tests, TestInvalidUnaryFunctionCall)
{
    using namespace a_util; // experimental::NullaryDelegate

    experimental::UnaryDelegate<TestType*, int> delegate(nullptr);
    EXPECT_FALSE(delegate);
    EXPECT_THROW(delegate(42), std::runtime_error);
}

TEST(delegate_tests, TestNullaryMethodCall)
{
    using namespace a_util; // experimental::NullaryDelegate

    TestType test_type;

    {
        experimental::NullaryDelegate<void> delegate(&TestType::nullaryVoid, test_type);
        delegate();
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::nullary_void_method);
    }

    {
        experimental::NullaryDelegate<TestType> delegate(&TestType::nullaryValue, test_type);
        delegate();
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::nullary_value_method);
    }

    {
        experimental::NullaryDelegate<TestType&> delegate(&TestType::nullaryReference, test_type);
        delegate();
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::nullary_reference_method);
    }

    {
        experimental::NullaryDelegate<TestType*> delegate(&TestType::nullaryPointer, test_type);
        delegate();
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::nullary_pointer_method);
    }
}

TEST(delegate_tests, TestInvalidNullaryMethodCall)
{
    using namespace a_util; // experimental::NullaryDelegate
    TestType test_type;
    typedef TestType* (TestType::*function_type)();

    experimental::NullaryDelegate<TestType*> delegate(function_type(nullptr), test_type);
    EXPECT_FALSE(delegate);
    EXPECT_THROW(delegate(), std::runtime_error);
}

TEST(delegate_tests, TestUnaryMethodCall)
{
    using namespace a_util; // experimental::UnaryDelegate
    TestType test_type;

    {
        experimental::UnaryDelegate<void, int> delegate(&TestType::unaryVoid, test_type);
        delegate(0);
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::unary_void_method);
    }

    {
        experimental::UnaryDelegate<TestType, int> delegate(&TestType::unaryValue, test_type);
        delegate(0);
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::unary_value_method);
    }

    {
        experimental::UnaryDelegate<TestType&, int> delegate(&TestType::unaryReference, test_type);
        delegate(0);
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::unary_reference_method);
    }

    {
        experimental::UnaryDelegate<TestType*, int> delegate(&TestType::unaryPointer, test_type);
        delegate(0);
        EXPECT_EQ(TestType::getLastCalledFunctionType(), TestType::unary_pointer_method);
    }
}

TEST(delegate_tests, TestInvalidUnaryMethodCall)
{
    using namespace a_util; // experimental::UnaryDelegate
    TestType test_type;
    typedef void (TestType::*function_type)(int);

    experimental::UnaryDelegate<void, int> delegate(function_type(nullptr), test_type);
    EXPECT_FALSE(delegate);
    EXPECT_THROW(delegate(42), std::runtime_error);
}
