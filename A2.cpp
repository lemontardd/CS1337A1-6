#include <stdio.h>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;


bool ReadIntoDataArray(ifstream& inputFileStreamObj, unsigned& maxElements, int dataArray[], int* pointerArray[]) {


   if (!(inputFileStreamObj >> maxElements))
      return true;

   for (unsigned i = 0; i < maxElements; i++) {
      inputFileStreamObj >> dataArray[i];
      pointerArray[i] = & dataArray[i];
   }

   return(false);
}


void DisplayDataArray(int dataArray[], unsigned maxElements) {
   cout << "Data array elements are:" << endl;
   for (unsigned i = 0; i < maxElements; i++)
      cout << dataArray[i] << setw(6);
   cout << endl << endl;
}


void DisplayPointerArray(int* pointerArray[], unsigned maxElements) {

   cout << "The pointer array of sorted pointer elements:" << endl;
   for (unsigned i = 0; i < maxElements; i++)
      cout << *pointerArray[i] << setw(6);
   cout << endl << endl;
}


void SwapIntPtr(int*& a, int*& b) {
   int* temp = a;
   a = b;
   b = temp;
}


void Sorting(int* pointerArray[], unsigned maxElements) {

   for(unsigned i = 0; i < maxElements; i++)
      for(unsigned j = i + 1; j < maxElements; j++)
         if(*pointerArray[j] < *pointerArray[i])
            SwapIntPtr(pointerArray[j], pointerArray[i]);

}


int main() {
   const string INPUT_FILE_NAME = "arrayData.txt";
   const unsigned MAX_ARRAY_SIZE = 10;
   unsigned maxElements = 0;

   int dataArray[MAX_ARRAY_SIZE];
   int* pointerArray[MAX_ARRAY_SIZE];

   ifstream inputFileStreamObj;

   inputFileStreamObj.open(INPUT_FILE_NAME, ios::in);
   if (inputFileStreamObj.fail()) {
      cout << "File " << INPUT_FILE_NAME << " could not be opened!" << endl;
      cout << "Press the enter key once or twice to continue..."; cin.ignore(); cin.get();
      exit(EXIT_FAILURE);
   }

   while (!ReadIntoDataArray(inputFileStreamObj, maxElements, dataArray, pointerArray)) {
      Sorting(pointerArray, maxElements);
      DisplayPointerArray(pointerArray, maxElements);
      DisplayDataArray(dataArray, maxElements);
      cout << endl << "Press the enter key once or twice to continue..."; cin.ignore(); cin.get();
      cout << endl << endl;
   }

   inputFileStreamObj.close();

   return EXIT_SUCCESS;
}
