#ifndef MAIN_APPLICATION_MISC_MISC_H_
#define MAIN_APPLICATION_MISC_MISC_H_

#include <string>

int getIntExponent(const double d);
double getBase(const double num);
std::string getIntExponentAsString(const double num);
std::string getBaseAsString(const double num);

std::string toStringWithNumDecimalPlaces(const double input_val, const size_t n = 6);
std::string formatNumber(const double num, const size_t n = 6);

#endif  // MAIN_APPLICATION_MISC_MISC_H_
