/*----------------------------------------------------------------------------------------------------------------------------
Program Name: Factorial Overflow Detection
Date: 2021-07-26
Author: Julia Manuel
Purpose: Creates a calculation class that has a template parameter that indicates the data type to do the calculations on.
----------------------------------------------------------------------------------------------------------------------------*/

#include <chrono>
#include <limits>
#include <locale>

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
using namespace chrono;

template <class TypeMultiply>
bool isSafeMultiply(TypeMultiply mult01, TypeMultiply mult02, TypeMultiply& multResult) {
   if ((mult01 == 0) || (mult02 == 0)) {
      multResult = 0;
      return true;
   }
   multResult = mult01 * mult02;
   if (mult01 == (multResult / mult02))
      return true;
   return false;
}

template <class TypeMultiply>
void calculateTimeFactorialLoopFunc(void) {

   auto timeStart = steady_clock::now();

   TypeMultiply multiplier = 1,
      factorialResult = 1;

   while (isSafeMultiply <TypeMultiply>(multiplier, factorialResult, factorialResult)) {
      cout << setw(3) << multiplier << setw(27) << factorialResult << endl;
      multiplier++;
   }

   auto timeElapsed = duration_cast<nanoseconds> (steady_clock::now() - timeStart);

   cout << endl;
   cout << "Unsigned Overflow At : " << setw(15) << multiplier << endl;
   cout << "Time Elapsed (nano)  : " << setw(15) << timeElapsed.count() << endl;

}//calculateTimeFactorialLoopFunc

template <class TypeMultiply>
void factorialRecursiveFunc(TypeMultiply& multiplier, TypeMultiply factorialResult) {

   if (isSafeMultiply <TypeMultiply>(factorialResult, multiplier, factorialResult)) {
      cout << setw(3) << multiplier << setw(27) << factorialResult << endl;
      factorialRecursiveFunc(++multiplier, factorialResult);
   }
   return;

}//factorialRecursive

template <class TypeMultiply>
void calculateTimeFactorialRecursiveFunc() {

   auto timeStart = steady_clock::now();

   TypeMultiply mult01 = 1;
   factorialRecursiveFunc <TypeMultiply>(mult01, 1);

   auto timeElapsed = duration_cast<nanoseconds> (steady_clock::now() - timeStart);

   cout << endl;
   cout << "Unsigned Overflow At : " << setw(15) << mult01 << endl;
   cout << "Time Elapsed (nano)  : " << setw(15) << timeElapsed.count() << endl;

}//calculateTimeFactorialRecursiveFunc()

template <typename TYPESIZE>
class FactorialCalculationClass {

private:
   unsigned typeSizeBits;

public:
   FactorialCalculationClass(unsigned typeSize) {
      typeSizeBits = typeSize;
   }

   void executeFactorialForType() {

      cout <<
         typeSizeBits <<
         " bit unsigned Factorial Loop" << endl <<
         "------------------------------" << endl;
      calculateTimeFactorialLoopFunc <TYPESIZE>();
      cout << endl << endl;

      cout <<
         typeSizeBits <<
         " bit unsigned Factorial Recursion" << endl <<
         "-----------------------------------" << endl;
      calculateTimeFactorialRecursiveFunc <TYPESIZE>();
      cout << endl << endl;

   }

};// template <class TYPESIZE> class factorialCalculationClass

int main() {
   // condition cout set to local digit separation comas (USA)
   cout.imbue(locale(""));

   FactorialCalculationClass <uint16_t> factorialCalculateUint16Obj(16);
   FactorialCalculationClass <uint32_t> factorialCalculateUint32Obj(32);
   FactorialCalculationClass <uint64_t> factorialCalculateUint64Obj(64);

   factorialCalculateUint16Obj.executeFactorialForType();
   factorialCalculateUint32Obj.executeFactorialForType();
   factorialCalculateUint64Obj.executeFactorialForType();

   cout << "Press enter key to end"; cin.ignore(); cin.get();

   return 0;
}