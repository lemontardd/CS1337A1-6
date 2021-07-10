#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

#include "HotDogStandsClass.h"
#include "TransactionsClass.h"

using namespace std;

const string HOTDOG_STANDS_FILE_NAME_STR = "InfoHotDogStands.txt";
const string TRANSACTIONS_FILE_NAME_STR = "InfoHotDogStandsTransactions.txt";
const string GLOBAL_SOLD_FILE_NAME_STR = "InfoGlobalSold.txt";

ostream& operator<<(ostream& leftSideOutputStream, const HotDogStandsClass& hotDogStandObj);

ostream& operator<<(ostream& leftSideOutputStream, const TransactionsClass& transactionsObj);

void CheckFileStreamOpen(string globalSoldFileNameStr, ifstream& inFile);

void ReadInFromGlobalSoldFile(string globalSoldFileNameStr);

void ReadInFromHotDogStandsFileTo
(string hotDogStandsFileNameStr, HotDogStandsClass*& hotDogStandsAry,
   unsigned& standsLineCount);

void ReadInFromTransactionsFileTo
(string transactionsFileNameStr, TransactionsClass*& transactionsAry, unsigned& transactionsLineCount);

void displayStands(HotDogStandsClass* hotDogStandsAry, unsigned totalNoHotDogStandsUns);

unsigned getMatchingHotDogStandsIndexUns
(HotDogStandsClass* hotDogStandsAry, unsigned totalNoHotDogStandsUns,
   TransactionsClass* transcationsAry, unsigned transcationsNoUns);

void processTransactions
(HotDogStandsClass*& hotDogStandsAry, unsigned totalNoHotDogStandsUns,
   TransactionsClass* transactionsAry, unsigned totalNoTransactionsUns);

void UpdateHotDogStandsFile
(string hotDogStandsFileNameStr, HotDogStandsClass* hotDogStandsAry,
   unsigned totalNoHotDogStandsUns);

void UpdateGlobalSoldFile(string globalSoldFileNameStr);

int main() {

   cout << setprecision(2) << fixed << showpoint;

   HotDogStandsClass* hotDogStandsAry = nullptr;
   unsigned hotDogStandsCountUns;
   TransactionsClass* transactionsAry = nullptr;
   unsigned transactionsCountUns;

   ReadInFromGlobalSoldFile(GLOBAL_SOLD_FILE_NAME_STR);
   ReadInFromHotDogStandsFileTo
   (HOTDOG_STANDS_FILE_NAME_STR, hotDogStandsAry, hotDogStandsCountUns);
   ReadInFromTransactionsFileTo
   (TRANSACTIONS_FILE_NAME_STR, transactionsAry, transactionsCountUns);

   cout << "Initial states of stands:" << endl;
   cout << "=========================" << endl << endl;
   displayStands(hotDogStandsAry, hotDogStandsCountUns);

   cout << "Process Transactions:" << endl;
   cout << "=====================" << endl << endl;
   processTransactions(hotDogStandsAry, hotDogStandsCountUns, transactionsAry, transactionsCountUns);

   cout << "Final states of stands:" << endl;
   cout << "=======================" << endl << endl;
   displayStands(hotDogStandsAry, hotDogStandsCountUns);

   UpdateHotDogStandsFile(HOTDOG_STANDS_FILE_NAME_STR, hotDogStandsAry, hotDogStandsCountUns);
   UpdateGlobalSoldFile(GLOBAL_SOLD_FILE_NAME_STR);

   cout << "Please press enbter key once or twice to end..."; cin.ignore(); cin.get();

   return EXIT_SUCCESS;
}

ostream& operator<<(ostream& leftSideOutputStream, const HotDogStandsClass& hotDogStandObj) {
   leftSideOutputStream <<
      "Stand Id: " + hotDogStandObj.getIdStr() << endl <<
      "Adress: " + hotDogStandObj.getAddressStr() << endl <<
      "Price: " << hotDogStandObj.getPriceFl() << endl <<
      "Inventory: " << hotDogStandObj.getInventoryAmountUns() << endl <<
      "Store Sold: " << hotDogStandObj.getSoldAmountUns()
      << " at $" << hotDogStandObj.getPriceFl() << " ea." << endl;
   return leftSideOutputStream;
   }

ostream& operator<<(ostream& leftSideOutputStream, const TransactionsClass& transactionsObj) {
leftSideOutputStream <<
"Transaction Stand Id: " + transactionsObj.getIdentificationStr() << endl <<
"Transations ID: " + transactionsObj.getTransactionIdStr() << endl <<
"Transaction Argument: " << transactionsObj.getTransactionArgumentUns() << endl;
return leftSideOutputStream;
}

void CheckFileStreamOpen(string globalSoldFileNameStr, ifstream& inFile) {
   if (inFile.fail()) {
      cout << "File " << globalSoldFileNameStr << "could not be opened !" << endl;
      cout << endl << "Press the enter key once or twice to continue..."  << endl; cin.ignore(); cin.get();
         exit(EXIT_FAILURE);
   }
}

void ReadInFromGlobalSoldFile(string globalSoldFileNameStr) {

   ifstream inFile(globalSoldFileNameStr);
   CheckFileStreamOpen(globalSoldFileNameStr, inFile);
   inFile >> HotDogStandsClass::globalSoldCountUns;
   inFile.close();
};

void ReadInFromHotDogStandsFileTo
(string hotDogStandsFileNameStr, HotDogStandsClass*& hotDogStandsAry,
   unsigned& hotDogStandsCountUns) {

   ifstream inFile(hotDogStandsFileNameStr);
   CheckFileStreamOpen(hotDogStandsFileNameStr, inFile);

   string lineInFileBufferStr;


   hotDogStandsCountUns = 0;
   while (getline(inFile, lineInFileBufferStr))
   ++hotDogStandsCountUns;

   hotDogStandsAry = new HotDogStandsClass[hotDogStandsCountUns];

   inFile.clear(); inFile.seekg(0, ios::beg);
   string inputStr;
   constexpr char COMMA_DELIMTER_CHAR = ',';


   for (unsigned lineCount = 0; lineCount < hotDogStandsCountUns; ++lineCount) {
      getline(inFile, lineInFileBufferStr);
      istringstream isStringStream(lineInFileBufferStr);


      getline(isStringStream, inputStr, COMMA_DELIMTER_CHAR);
      hotDogStandsAry[lineCount].setIdStr(inputStr);

      getline(isStringStream, inputStr, COMMA_DELIMTER_CHAR);
      hotDogStandsAry[lineCount].setPriceFl(stof(inputStr));

      getline(isStringStream, inputStr, COMMA_DELIMTER_CHAR);
      hotDogStandsAry[lineCount].setAddressStr(inputStr);

      getline(isStringStream, inputStr, COMMA_DELIMTER_CHAR);
      isStringStream >> inputStr;
      hotDogStandsAry[lineCount].setInventoryAmountUns(static_cast<unsigned>(stoul(inputStr)));

      getline(isStringStream, inputStr, COMMA_DELIMTER_CHAR);
      isStringStream >> inputStr;
      hotDogStandsAry[lineCount].setSoldAmountUns(static_cast<unsigned>(stoul(inputStr)));

   } // for

   inFile.close();
}

void ReadInFromTransactionsFileTo
(string transactionsFileNameStr, TransactionsClass*& transactionsAry, unsigned& transactionsLineCount) {

   ifstream inFile(transactionsFileNameStr);
   CheckFileStreamOpen(transactionsFileNameStr, inFile);

   string lineInFileBufferStr;


   transactionsLineCount = 0;
   while (getline(inFile, lineInFileBufferStr))
      ++transactionsLineCount;

   transactionsAry = new TransactionsClass[transactionsLineCount];

   inFile.clear(); inFile.seekg(0, ios::beg);
   string inputStr;
   constexpr char COMMA_DELIMTER_CHAR = ',';

   for (unsigned lineCount = 0; lineCount < transactionsLineCount; ++lineCount) {
      getline(inFile, lineInFileBufferStr);
      istringstream isStringStream(lineInFileBufferStr);

      getline(isStringStream, inputStr, COMMA_DELIMTER_CHAR);
      transactionsAry[lineCount].setIdentificationStr(inputStr);

      getline(isStringStream, inputStr, COMMA_DELIMTER_CHAR);
      transactionsAry[lineCount].setIdentificationStr(inputStr);

      getline(isStringStream, inputStr, COMMA_DELIMTER_CHAR);
      transactionsAry[lineCount].setTransactionArgumentUns(static_cast<unsigned>(stoul(inputStr)));

   }

   inFile.close();
}

void displayStands(HotDogStandsClass* hotDogStandsAry, unsigned totalNoHotDogStandsUns) {

   for (unsigned displayIndex = 0; displayIndex < totalNoHotDogStandsUns; ++displayIndex)
      cout << hotDogStandsAry[displayIndex] << endl;

   cout << endl << "Global Sold: " << HotDogStandsClass::globalSoldCountUns << endl << endl;
   cout << "Please press enter key once or twice to continue..."; cin.ignore(); cin.get();
   cout << endl << endl;
}

unsigned getMatchingHotDogStandsIndexUns
(HotDogStandsClass* hotDogStandsAry, unsigned totalNoHotDogStandsUns,
   TransactionsClass* transactionsAry, unsigned transactionsCountUns) {
   for (unsigned seekIndex = 0; seekIndex < totalNoHotDogStandsUns; ++seekIndex)
      if (hotDogStandsAry[seekIndex].getIdStr() == transactionsAry[transactionsCountUns].getIdentificationStr())
         return(seekIndex);
}

void processTransaction(TransactionsClass transactionObj, HotDogStandsClass& hotDogStandObj) {
   if (transactionObj.getTransactionIdStr() == "stock inventory") {
      hotDogStandObj.stockInventoryAmountUns(transactionObj.getTransactionArgumentUns());
   }
   else if (transactionObj.getTransactionIdStr() == "buy")
   {
      hotDogStandObj.hotDogBuyUns(transactionObj.getTransactionArgumentUns());
   }

};

void processTransactions
(HotDogStandsClass*& hotDogStandsAry, unsigned totalNoHotDogStandsUns,
   TransactionsClass* transactionsAry, unsigned transactionsCountUns) {

   for (unsigned transactionsUns = 0; transactionsUns < transactionsCountUns; ++transactionsUns) {

      unsigned hotDogStandsIndexUns =
         getMatchingHotDogStandsIndexUns(hotDogStandsAry, totalNoHotDogStandsUns,
            transactionsAry, transactionsUns);
      cout << "---------------------------" << endl << endl;
      cout << "HotDog Stand Current Status: " << endl << endl;
      cout << hotDogStandsAry[hotDogStandsIndexUns] << endl;
      cout << "Transaction: " << endl << endl;
      cout << transactionsAry[transactionsUns] << endl;
      processTransaction(transactionsAry[transactionsUns], hotDogStandsAry[hotDogStandsIndexUns]);
      cout << "HotDog Stand Status After Transaction: " << endl << endl;
      cout << hotDogStandsAry[hotDogStandsIndexUns] << endl << endl;
      cout << "Global Sold: " << HotDogStandsClass::globalSoldCountUns << endl << endl;
      cout << "Please press enter key once or twice to continue..."; cin.ignore(); cin.get();
   }
}

void UpdateHotDogStandsFile
(string hotDogStandsFileNameStr, HotDogStandsClass* hotDogStandsAry,
   unsigned totalNoHotDogStandsUns) {

   ofstream outFile(hotDogStandsFileNameStr);

   for (unsigned writeIndex = 0; writeIndex < totalNoHotDogStandsUns; ++writeIndex)
      outFile <<
      hotDogStandsAry[writeIndex].getIdStr() << "," <<
      hotDogStandsAry[writeIndex].getAddressStr() << "," <<
      hotDogStandsAry[writeIndex].getPriceFl() << "," <<
      hotDogStandsAry[writeIndex].getInventoryAmountUns() << "," <<
      hotDogStandsAry[writeIndex].getSoldAmountUns() << endl;

      outFile.close();
};

void UpdateGlobalSoldFile(string globalSoldFileNameStr) {

   ofstream outFile(globalSoldFileNameStr);
   outFile << HotDogStandsClass::globalSoldCountUns << endl;

   outFile.close();
}

