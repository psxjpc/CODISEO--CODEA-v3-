/** 
   @library conversions

   @brief Implements a number of functions to convers data-types

   This library is intended to have a list of function related to
   conversions.

   @author Group of Intelligent Computing - Universidad de La Laguna - 2008
*/


#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <string>
#include <cassert>
#include <sstream>


/**
   Function that receives a std::string and return a value converted to T.
   @param const std::string& is a constant reference to the string value we want to convert.
   @return T value with the given value converted.
   @note this is specially useful when we read plain-text files with data.
   \code
      // This is just an example of its use.
      std::string stringNumber = "120";
      double doubleNumber = fromStringTo<double>(stringNumber);
   \endcode
*/
template <typename T>
inline T fromStringTo (const std::string& inputString)
{
   T result;
   std::istringstream stream(inputString);

   // This must be done by launching an exception.
   assert(stream >> result); 
   return result;
}

/**
   Function that receives a value (T) and return a std::string containing that value.
   @param const T& is a constant reference to the value we want to convert.
   @return std::string with the given value.
   \code
      // This is just an example of its use.
      double doubleNumber = 120.05;
      std::string stringNumber = somethingToString<std::string>(doubleNumber);
   \endcode
*/
template <typename T>
inline std::string somethingToString(const T& something)
{
   std::ostringstream outputStream;
   outputStream << something;
   return outputStream.str();
}


inline double atod(char* number)
{
    double result = 0.0;
    std::string inputString(number);
    std::istringstream stream(inputString);
    assert(stream >> result);
    return result;
}

#endif
