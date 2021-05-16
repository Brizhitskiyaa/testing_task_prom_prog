//
// Created by akhtyamovpavel on 5/1/20.
//


#pragma once

#include <gtest/gtest.h>

#include <random>
#include <chrono>

class LeapTestCase : public ::testing::Test {
protected:
    LeapTestCase() :
        dist_(std::uniform_int_distribution<int>{1, 10000}),
        gen_(std::chrono::system_clock::now().time_since_epoch().count()) {
    }

    int GetNext() {
        return dist_(gen_);
    }

private:
    std::uniform_int_distribution<int> dist_;
    std::mt19937 gen_;
};


