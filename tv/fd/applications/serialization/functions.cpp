#include "functions.h"

#include <iostream>
#include <string>
#include <vector>

void doMath_WritingToFile(const float q, const std::vector<double>& vec, const std::string& s)
{
    // Serialization begin
    /*
    __FILE__, __func__
    LOG_TO_FILE(q, vec, s);

    #define LOG_TO_FILE(...) serialization::logToFile(__FILE__, __func__, __VA_ARGS__)

    */
    // Serialization end

    // Do math...
}

void doMath_ReadingFromFile(const float _q, const std::vector<double>& _vec, const std::string& _s)
{
    float q;
    std::vector<double> vec;
    std::string s;

    // Deerialization begin

    /*
    READ_FROM_FILE(q, vec, s);
    
    define LOG_TO_FILE(...) serialization::readFromFile(__FILE__, __func__, __VA_ARGS__)

    */
    // Serialization end

    // Do math...
}
