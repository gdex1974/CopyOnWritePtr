#include "cow_ptr.h"
#include <gtest/gtest.h>

TEST(CopyOnWritePtrTest, DefaultConstructor)
{
    nonstd::cow_ptr<int> emptyPtr;
    EXPECT_FALSE(emptyPtr);
}

TEST(CopyOnWritePtrTest, DereferenceOperator)
{
    auto valuePtr = nonstd::make_cow<int>(42);
    EXPECT_EQ(*valuePtr, 42);
}

TEST(CopyOnWritePtrTest, CopyConstructor)
{
    auto valuePtr = nonstd::make_cow<int>(42);
    const nonstd::cow_ptr<int> copiedPtr(valuePtr);
    EXPECT_EQ(*copiedPtr, 42);
    EXPECT_TRUE(valuePtr == copiedPtr);
}

TEST(CopyOnWritePtrTest, MoveConstructor)
{
    auto valuePtr = nonstd::make_cow<int>(42);
    const nonstd::cow_ptr<int> movedPtr(std::move(valuePtr));
    EXPECT_EQ(*movedPtr, 42);
    EXPECT_FALSE(valuePtr);
}

// Test the assignment operator
TEST(CopyOnWritePtrTest, AssignmentOperator)
{
    auto valuePtr = nonstd::make_cow<int>(42);
    nonstd::cow_ptr<int> assignedPtr;
    assignedPtr = valuePtr;
    EXPECT_TRUE(assignedPtr == valuePtr);
    EXPECT_EQ(*assignedPtr, 42);
    EXPECT_FALSE(assignedPtr == valuePtr);
}

// Test the move assignment operator
TEST(CopyOnWritePtrTest, MoveAssignmentOperator)
{
    auto valuePtr = nonstd::make_cow<int>(42);
    nonstd::cow_ptr<int> movedPtr;
    movedPtr = std::move(valuePtr);
    EXPECT_EQ(*movedPtr, 42);
    EXPECT_FALSE(valuePtr);
}

// Test the arrow operator
TEST(CopyOnWritePtrTest, ArrowOperator)
{
    int* ptr = new int(42);
    nonstd::cow_ptr<int> copyOnWritePtr(ptr);
    EXPECT_EQ(copyOnWritePtr.operator->(), ptr);
}

// Test the equality operator
TEST(CopyOnWritePtrTest, EqualityOperator)
{
    const auto copyOnWritePtr = nonstd::make_cow<int>(42);
    {
        const nonstd::cow_ptr<int> equalPtr(copyOnWritePtr);
        EXPECT_TRUE(copyOnWritePtr == equalPtr);
    }
    EXPECT_EQ(*copyOnWritePtr, 42);
}

// Test the inequality operator
TEST(CopyOnWritePtrTest, InequalityOperator)
{
    const auto valuePtr = nonstd::make_cow<int>(42);
    const nonstd::cow_ptr<int> unequalPtr(new int(99));
    EXPECT_TRUE(valuePtr != unequalPtr);
}

// Test the reset function
TEST(CopyOnWritePtrTest, ResetFunction)
{
    auto copyOnWritePtr = nonstd::make_cow<int>(42);
    copyOnWritePtr.reset(new int(100));
    EXPECT_EQ(*copyOnWritePtr, 100);
}

TEST(CopyOnWritePtrTest, ResetFunctionWithNullptr)
{
    auto copyOnWritePtr = nonstd::make_cow<int>(42);
    copyOnWritePtr.reset();
    EXPECT_FALSE(copyOnWritePtr);
}

TEST(CopyOnWritePtrTest, Get)
{
    auto copyOnWritePtr = nonstd::make_cow<int>(42);
    EXPECT_EQ(*copyOnWritePtr.get(), 42);
    nonstd::cow_ptr<int> newPtr = copyOnWritePtr;
    EXPECT_TRUE(copyOnWritePtr.get() == newPtr.get());
    *newPtr.getMutable() = 10;
    EXPECT_EQ(*newPtr.get(), 10);
    EXPECT_EQ(*copyOnWritePtr.get(), 42);
}