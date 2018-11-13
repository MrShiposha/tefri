#ifndef TEST_TEFRI_H
#define TEST_TEFRI_H

#include <iostream>
#include <cmath>

#include "../release/tefri.hpp"

#define TEST(condition, message) \
    do { if(!(condition)) { std::cout << __FILE__ << ":" << __LINE__ << " (" << #condition << ") failed: " << message << std::endl; return false; } } while(0)

#define TEST_FRACTION_NEAR(first_value, second_value, epsilon, message) \
    do { if((std::abs(first_value - second_value)) >= (epsilon)) { std::cout << __FILE__ << ":" << __LINE__ << ' ' << #first_value << " != " << #second_value << " with epsilon = " << #epsilon << ": " << message << std::endl; return false; } } while(0)

class TestTefri
{
public:
    virtual ~TestTefri() {}

    virtual bool test() = 0;
};

#endif // TEST_TEFRI_H