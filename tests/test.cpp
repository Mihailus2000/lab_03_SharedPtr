// Copyright 2018 Your Name <your_email>

#include "gtest/gtest.h"
#include "SharedPtr.hpp"


TEST(TestSharedPtr, firstTest){
    int* IntValue= new int(12);
    SharedPtr pointer(IntValue);
    EXPECT_EQ(*(pointer), 12);
    EXPECT_EQ(pointer.get(),IntValue);
    EXPECT_EQ(pointer.use_count(), 1u);
    EXPECT_EQ(static_cast<bool>(pointer), true);
}

TEST(TestSharedPtr, secondtTest){

    struct testStruct{
        int8_t a = 11;
    };

    testStruct *obj = new testStruct;
    SharedPtr<testStruct> pointer;
    ASSERT_EQ(pointer.get(), nullptr);
//    ASSERT_EQ(pointer.use_count(), 0u);
    pointer = SharedPtr(obj);

    EXPECT_EQ(pointer.get(), obj);


}

class SharedPtrTest : public testing::Test{
protected:

    struct testStruct{
        SharedPtr<int> ptr;
    };

    void SetUp() override {
        numPtr = new int(21);
        objPtr = new testStruct;
        stringPtr = new std::string{"It is string"};
    }

    void TearDown() override {}

    std::string* stringPtr = nullptr;
    int* numPtr = nullptr;
    testStruct* objPtr = nullptr;

    SharedPtr<std::string> stringSharedPointer;
    SharedPtr<testStruct> structSharedPointer;
    SharedPtr<int> intPointer;

};

    TEST_F(SharedPtrTest, EmptyPointers){

    EXPECT_EQ(static_cast<bool>(stringSharedPointer), false);
    EXPECT_EQ(static_cast<bool>(structSharedPointer), false);
    EXPECT_EQ(static_cast<bool>(intPointer), false);

    EXPECT_EQ(stringSharedPointer.get(), nullptr);
    EXPECT_EQ(structSharedPointer.get(), nullptr);
    EXPECT_EQ(intPointer.get(), nullptr);
}

TEST_F(SharedPtrTest, testString){
    stringSharedPointer = SharedPtr{stringPtr};
    EXPECT_EQ(stringSharedPointer.use_count(), 1u);
    EXPECT_EQ(stringSharedPointer.get(), stringPtr);
    EXPECT_EQ(*(stringSharedPointer.get()), std::string("It is string"));
    EXPECT_EQ(stringSharedPointer->size(),  stringPtr->size());
}

TEST_F(SharedPtrTest, testStruct){
        structSharedPointer = SharedPtr{objPtr};
    EXPECT_EQ(structSharedPointer.use_count(), 1u);
    EXPECT_EQ(structSharedPointer.get(), objPtr);
    EXPECT_EQ(structSharedPointer->ptr, objPtr->ptr);
}

TEST_F(SharedPtrTest, testAssigmentOperator){
        intPointer = SharedPtr{numPtr};
        EXPECT_EQ(intPointer.get(), numPtr);
        EXPECT_EQ((*intPointer),*numPtr);
        EXPECT_EQ(intPointer.use_count(),1u);

        SharedPtr<int> newPtr(intPointer);
        EXPECT_EQ(newPtr.use_count(), 2u);
        EXPECT_EQ(newPtr.use_count(),2u);

        SharedPtr<int> newPtrMove = std::move(intPointer);

        EXPECT_EQ(newPtrMove.use_count(), newPtr.use_count());
        EXPECT_EQ(newPtrMove.use_count(), 2u);
        EXPECT_EQ(intPointer.get(), nullptr);

        *newPtrMove = 33;

        SharedPtr<int> PtrCopy(newPtrMove);
        EXPECT_EQ(PtrCopy.use_count(), newPtrMove.use_count());
        EXPECT_EQ(PtrCopy.use_count(), 3);
}

TEST_F(SharedPtrTest,testSwap){
    intPointer = SharedPtr(numPtr);

    EXPECT_EQ(intPointer.use_count(), 1u );

    structSharedPointer = SharedPtr(objPtr);
    ASSERT_EQ(structSharedPointer->ptr.get(), nullptr);
    ASSERT_NE(structSharedPointer->ptr.get(), intPointer.get());
    SharedPtr<int> pointerTest;
    pointerTest.swap(intPointer);
    EXPECT_EQ(*pointerTest, 21);
    EXPECT_EQ(static_cast<bool>(intPointer), false);
}

TEST_F(SharedPtrTest,selfSwap){
        stringSharedPointer = SharedPtr(stringPtr);
        stringSharedPointer.swap(stringSharedPointer);

        EXPECT_EQ(static_cast<bool>(stringSharedPointer), true);
        EXPECT_EQ(stringSharedPointer.use_count(), 1u);
        EXPECT_EQ(stringSharedPointer.get(), stringPtr);
        EXPECT_EQ(stringSharedPointer->size(), stringPtr->size());
}

TEST_F(SharedPtrTest, reset){
    stringSharedPointer = SharedPtr(stringPtr);
    stringSharedPointer.reset();
    EXPECT_EQ(static_cast<bool>(stringSharedPointer), false);

    intPointer = SharedPtr(numPtr);
    int* resetPtr = new int(333);
    intPointer.reset(resetPtr);

    EXPECT_EQ(intPointer.get(), resetPtr);
    EXPECT_EQ(intPointer.use_count(), 1u);
    EXPECT_EQ(*intPointer, *resetPtr);
    EXPECT_EQ(*intPointer, *resetPtr);



}