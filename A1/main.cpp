  
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

const unsigned MAX_MATRIX_SIZE = 20;
const unsigned WIDTH_DISPLAY = 12;

bool readMatrixFromFile (int[][MAX_MATRIX_SIZE], unsigned&, unsigned&, ifstream&);
void displayMatrix (int[][MAX_MATRIX_SIZE], unsigned, unsigned);
void displayCalcMatrixSums (int[][MAX_MATRIX_SIZE], unsigned, unsigned, int[MAX_MATRIX_SIZE]);
void symmetryCheckMatrix (int[], unsigned, unsigned);
void sortMatrixRows (int[][MAX_MATRIX_SIZE], unsigned, unsigned);

int main()
{
   // opens file "matrixes.txt"
   unsigned height, width;
   int matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE], colSums[MAX_MATRIX_SIZE];
   string INPUT_FILE_NAME = "matrixes.txt";
   ifstream inputFileStreamObj;

   // checks if file opened
   inputFileStreamObj.open(INPUT_FILE_NAME, ios::in);
   if (inputFileStreamObj.fail()) {
   cout << "File " << INPUT_FILE_NAME << " could not be opened." << endl;
   cout << endl << "Press the enter key once or twice to continue..." << endl;
   cin.ignore();
   cin.get();
   exit(1);
   }

   // displays input, sorted, added values and vertical additive symmetry in "matrixes.txt"
   do {
      if (readMatrixFromFile(matrix, height, width, inputFileStreamObj))
         break;
      cout << "Input:" << endl;
      displayMatrix(matrix, height, width);
      displayCalcMatrixSums(matrix, height, width, colSums);
      symmetryCheckMatrix(colSums, width, height);
      sortMatrixRows(matrix, width, height);
      cout << "Sorted: " << endl;
      displayMatrix(matrix, height, width);
      cout << endl << "Press the enter key once or twice to continue..." << endl;
      cin.ignore();
      cin.get();
   } while (true);

   return 0;
}

// function inputs values from a specified matrix from file "matrixes.txt"
bool readMatrixFromFile(int matrix[][MAX_MATRIX_SIZE], unsigned& height, unsigned& width, ifstream& inputFileStreamObj) {
   if (!(inputFileStreamObj >> height))
      return true;
   inputFileStreamObj >> width;

   for (unsigned row = 0; row < height; row++)
      for (unsigned col = 0; col < width; col++)
         inputFileStreamObj >> matrix[row][col];

   return(false);
}

// function displays a specified matrix from "matrixes.txt"
void displayMatrix(int matrix[][MAX_MATRIX_SIZE], unsigned height, unsigned width) {
   for (unsigned row = 0; row < height; row++) {
      for (unsigned col = 0; col < width; col++)
      cout << setw(WIDTH_DISPLAY) << matrix[row][col];
      cout << endl;
   }
}

// function calculates and displays the sums of values a specified matrix in "matrixes.txt"
void displayCalcMatrixSums(int matrix[][MAX_MATRIX_SIZE], unsigned height, unsigned width, int colSums[MAX_MATRIX_SIZE]) {
   for (unsigned col = 0; col < width; col++)
      colSums[col] = 0;
   for (unsigned col = 0; col < width; col++)
      for (unsigned row = 0; row < height; row++)
         colSums[col] += matrix[row][col];
   cout << "Sums: " << endl;
   for (unsigned col = 0; col < width; col++)
      cout << setw(WIDTH_DISPLAY) << colSums[col];
   cout << endl << endl;

   return;
}

// function checks and displays vertical additive symmetry of a specified matrix in "matrixes.txt"
void symmetryCheckMatrix(int colSums[], unsigned width, unsigned height) {
   cout << "Vertical additive symmetry: ";
   for (unsigned col = 0; col < width / 2; col++) {
      int sumLeftMostCol = 0, sumRightMostCol = 0;
      sumLeftMostCol += colSums[col];
      sumRightMostCol += colSums[width - col - 1];
      if (sumLeftMostCol != sumRightMostCol) {
         cout << "No" << endl << endl;
         return;
      }
   }
   cout << "Yes" << endl << endl;

   return;
}

// function sorts and displays a specified matrix in "matrixes.txt"
void sortMatrixRows(int matrix[][MAX_MATRIX_SIZE], unsigned width, unsigned height) {
   for (unsigned row = 0; row < height; row++)
      for (unsigned col = 0; col < (width - 1); col++)
         for (unsigned seek = 0; seek < (width - col - 1); seek++)
            if (matrix[row][seek] > matrix[row][seek + 1]) {
               int temp = matrix[row][seek];
               matrix[row][seek] = matrix[row][seek + 1];
               matrix[row][seek + 1] = temp;
            }
}
