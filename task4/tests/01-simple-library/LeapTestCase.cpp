//
// Created by akhtyamovpavel on 5/1/20.
//

#include "LeapTestCase.h"

#include <Functions.h>
#include <vector>

TEST_F(LeapTestCase, NonPositive) {
  for (size_t i = 0; i < 10; ++i) {
    EXPECT_THROW(IsLeap(GetNext() * -1), std::invalid_argument);
  }
  EXPECT_THROW(IsLeap(0), std::invalid_argument);
}

TEST_F(LeapTestCase, MultipleFourHundred) {
  for (size_t i = 0; i < 10; ++i) {
    EXPECT_TRUE(IsLeap(GetNext() * 400));
  }
}

TEST_F(LeapTestCase, MultipleHundred) {
  EXPECT_FALSE(IsLeap(2100));
  EXPECT_FALSE(IsLeap(2300));
  EXPECT_FALSE(IsLeap(2200));
}

TEST_F(LeapTestCase, NonDivisibleByHundredOrFour) {
  EXPECT_FALSE(IsLeap(2001));
  EXPECT_FALSE(IsLeap(2099));
}

TEST_F(LeapTestCase, MultipleFour) {
  EXPECT_TRUE(IsLeap(2004));
  EXPECT_TRUE(IsLeap(2012));
  EXPECT_TRUE(IsLeap(1616));
}

TEST_F(LeapTestCase, Random) {
  for (size_t i = 0; i < 10; ++i) {
    auto year = GetNext();

    if (year % 4 == 0 || year % 400 == 0) {
      EXPECT_TRUE(IsLeap(year));
    } else {
      EXPECT_FALSE(IsLeap(year));
    }

  }
}

TEST_F(LeapTestCase, GetMonthDaysFebruary) {
  auto year = GetNext();
  while (IsLeap(year)) {
    year = GetNext();
  }
  auto leap_year = year * 4;

  ASSERT_TRUE(!IsLeap(year));
  ASSERT_TRUE(IsLeap(leap_year));

  EXPECT_EQ(GetMonthDays(year, 2), 28);
  EXPECT_EQ(GetMonthDays(leap_year, 2), 29);
}

TEST_F(LeapTestCase, GetMonthDaysBoundaries) {
  auto year = GetNext();

  EXPECT_THROW(GetMonthDays(year, 13), std::invalid_argument);
  EXPECT_THROW(GetMonthDays(year, 0), std::invalid_argument);
}

TEST_F(LeapTestCase, GetMonthDaysThirtyOne) {
  auto year = GetNext();
  std::vector<int> months = {1, 3, 5, 7, 8, 10, 12};

  for (auto i : months) {
    EXPECT_EQ(GetMonthDays(year, i), 31);
  }
}
TEST_F(LeapTestCase, GetMonthDaysThirty) {
  auto year = GetNext();
  std::vector<int> months = {4, 6, 9, 11};

  for (auto i : months) {
    EXPECT_EQ(GetMonthDays(year, i), 30);
  }
}
