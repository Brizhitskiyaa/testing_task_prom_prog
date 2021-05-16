//
// Created by akhtyamovpavel on 5/1/20.
//

#include "AddTestCase.h"
#include "Functions.h"

TEST_F(AddTestCase, TestAdd) {
    EXPECT_EQ(Add(1, 2), 3);
}

TEST_F(AddTestCase, TestNegative) {
    EXPECT_EQ(Add(-1, -2), -3);
}
