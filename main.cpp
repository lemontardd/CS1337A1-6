/*----------------------------------------------------------------------------------------------------------------------------
Program Name: Bank Accounts
Date: 2021-07-21
Author: Julia Manuel
Purpose: Uses object oriented dsign to design accounts with transaction processing at a bank.
Inputs: "InfoBankAccounts.txt", "InfoBankAccountsTransactions.txt", "InfoBankConfig.txt" files
Outputs: Checkings, interests, and savings account data.
----------------------------------------------------------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

#include "BankAccountsClass.h"
#include "CheckingAccountsClass.h"
#include "InterestCheckingAccountsClass.h"
#include "SavingsAccountsClass.h"
#include "TransactionsClass.h"

using namespace std;

const string BANK_CONFIGURATION_FILE_NAME_STR = "InfoBankConfig.txt";
const string BANK_ACCOUNTS_FILE_NAME_STR = "InfoBankAccounts.txt";
const string BANK_ACCOUNTS_TRANSACTIONS_FILE_NAME_STR = "InfoBankAccountsTransactions.txt";

void CheckFileStreamOpen(string fileNameStr, ifstream& inFile);
void CheckFileStreamOpen(string fileNameStr, ofstream& outFile);
void ReadConfigurationFile(string bankConfigurationFileNameStr);
void ReadInFromBankAccountsFile
(string bankAccountsFileNameStr,
   BankAccountsClass**& bankAccountsAry, unsigned& accountsCount);
void ReadInFromTransactionsFile
(string transactionsFileNameStr,
   TransactionsClass*& transactionsAry, unsigned& transactionsCountUns);
void ProcessTransactions
(TransactionsClass* transactionsAry, unsigned& transactionsCountUns,
   BankAccountsClass**& bankAccountsAry, unsigned& accountsCountUns);
bool getMatchingBankAccountIndexUns(
   BankAccountsClass**& bankAccountsAry, unsigned  accountsCountUns,
   string accountIdStr, unsigned& bankAccountIndex);
void UpdateBankAccountsFile
(string   bankAccountsFileNameStr,
   BankAccountsClass**& bankAccountsAry, unsigned totalNoBankAccountsUns);

int main() {
   BankAccountsClass** bankAccountsAry = nullptr;
   TransactionsClass* transactionsAry = nullptr;
   unsigned            accountsCountUns = 0;
   unsigned            transactionsCountUns = 0;

   cout << setprecision(2) << fixed << showpoint;           //  set up output for dollar amounts

   ReadConfigurationFile(BANK_CONFIGURATION_FILE_NAME_STR);
   ReadInFromBankAccountsFile(BANK_ACCOUNTS_FILE_NAME_STR,
      bankAccountsAry, accountsCountUns);
   ReadInFromTransactionsFile(BANK_ACCOUNTS_TRANSACTIONS_FILE_NAME_STR,
      transactionsAry, transactionsCountUns);
   ProcessTransactions(transactionsAry, transactionsCountUns,
      bankAccountsAry, accountsCountUns);
   UpdateBankAccountsFile(BANK_ACCOUNTS_FILE_NAME_STR,
      bankAccountsAry, accountsCountUns);

} // int main()

void CheckFileStreamOpen(string fileNameStr, ifstream& inFile) {
   if (inFile.fail()) {
      cout << "File " << fileNameStr << " could not be opened!" << endl;
      cout << endl << "Press the enter key once or twice to continue..." << endl; cin.ignore(); cin.get();
      exit(EXIT_FAILURE);
   }
}

void CheckFileStreamOpen(string fileNameStr, ofstream& outFile) {
   if (outFile.fail()) {
      cout << "File " << fileNameStr << " could not be opened!" << endl;
      cout << endl << "Press the enter key once or twice to continue..." << endl; cin.ignore(); cin.get();
      exit(1);
   }
}

void ReadConfigurationFile(string bankConfigurationFileNameStr) {

   ifstream inFile(bankConfigurationFileNameStr);
   CheckFileStreamOpen(bankConfigurationFileNameStr, inFile);

   cout << "Configuration Information: " << endl <<
      "==========================" << endl << endl;

   float valueFloat;
   inFile >> valueFloat; SavingsAccountsClass::setSavingsInterestRate(valueFloat);
   cout << "Savings Interest Rate: " << SavingsAccountsClass::getSavingsInterestRate() << endl;

   inFile >> valueFloat; CheckingAccountsClass::setCheckingMinimumFl(valueFloat);
   cout << "Checking Minimum Balance: " << CheckingAccountsClass::getCheckingMinimumFl() << endl;

   inFile >> valueFloat; CheckingAccountsClass::setChargePerCheckFl(valueFloat);
   cout << "Charge Per Check: " << CheckingAccountsClass::getChargePerCheckFl() << endl;

   inFile >> valueFloat; InterestCheckingAccountsClass::setInterestCheckingRate(valueFloat);
   cout << "Interest Checking Rate: " << InterestCheckingAccountsClass::getInterestCheckingRate() << endl;

   inFile >> valueFloat; InterestCheckingAccountsClass::setMinimmunBlanceRequired(valueFloat);
   cout << "Interest Checking Minimum Balance: " << InterestCheckingAccountsClass::getMinimmunBlanceRequired() << endl;

   inFile >> valueFloat; InterestCheckingAccountsClass::setMonthlyFeeChargeNoMinimumBalance(valueFloat);
   cout << "Interest Checking Monthly Fee Charge: " << InterestCheckingAccountsClass::getMonthlyFeeChargeNoMinimumBalance() << endl;

   cout << endl << endl;

   inFile.close();

}

void ReadInFromBankAccountsFile
(string bankAccountsFileNameStr,
   BankAccountsClass**& bankAccountsAry, unsigned& accountsCountUns) {

   ifstream file(bankAccountsFileNameStr);
   CheckFileStreamOpen(bankAccountsFileNameStr, file);

   string lineInFileBufferStr;


   accountsCountUns = 0;
   while (getline(file, lineInFileBufferStr)) ++accountsCountUns;

   bankAccountsAry = new BankAccountsClass * [accountsCountUns];

   file.clear(); file.seekg(0, ios::beg);
   constexpr char COMMA_DELIMTER_CHAR = ',';

   string accountIdStr;
   string accountOwnerStr;
   string accountBalanceStr;
   float accountBalanceFl;

   cout << "Accounts: " << endl <<
      "=========" << endl << endl;

   for (unsigned lineCount = 0; lineCount < accountsCountUns; ++lineCount) {
      getline(file, lineInFileBufferStr);
      istringstream isStringStream(lineInFileBufferStr);


      getline(isStringStream, accountIdStr, COMMA_DELIMTER_CHAR);
      getline(isStringStream, accountOwnerStr, COMMA_DELIMTER_CHAR);
      getline(isStringStream, accountBalanceStr, COMMA_DELIMTER_CHAR);
      accountBalanceFl = stof(accountBalanceStr);

      switch (accountIdStr.at(0)) {
      case 'S':
      case's':
         bankAccountsAry[lineCount] = new SavingsAccountsClass(accountIdStr, accountOwnerStr, accountBalanceFl);
         bankAccountsAry[lineCount]->display();
         break;

      case 'C':
      case 'c':
         bankAccountsAry[lineCount] = new CheckingAccountsClass(accountIdStr, accountOwnerStr, accountBalanceFl);
         bankAccountsAry[lineCount]->display();
         break;

      case 'I':
      case 'i':
         bankAccountsAry[lineCount] = new InterestCheckingAccountsClass(accountIdStr, accountOwnerStr, accountBalanceFl);
         bankAccountsAry[lineCount]->display();
         break;

      default:
         cout << "*** Illegal Account Identification ***" << endl;
      } //switch
   } // for

   file.close();
}//ReadInFromBankAccountsFile()

void ReadInFromTransactionsFile
(string transactionsFileNameStr,
   TransactionsClass*& transactionsAry, unsigned& transactionsCountUns) {

   ifstream file(transactionsFileNameStr);
   CheckFileStreamOpen(transactionsFileNameStr, file);

   string lineInFileBufferStr;


   transactionsCountUns = 0;
   while (getline(file, lineInFileBufferStr))
      ++transactionsCountUns;

   transactionsAry = new TransactionsClass[transactionsCountUns];

   file.clear(); file.seekg(0, ios::beg);
   constexpr char COMMA_DELIMTER_CHAR = ',';

   string accountIdStr;
   string accountTransactionIdStr;
   string accountTransactionAmountStr;
   float accountTransactionAmountFl;

   cout << "Transactions: " << endl <<
      "-------------" << endl << endl;


   for (unsigned lineCount = 0; lineCount < transactionsCountUns; ++lineCount) {

      getline(file, lineInFileBufferStr);
      istringstream isStringStream(lineInFileBufferStr);


      getline(isStringStream, accountIdStr, COMMA_DELIMTER_CHAR);
      transactionsAry[lineCount].setIdentifcationStr(accountIdStr);

      getline(isStringStream, accountTransactionIdStr, COMMA_DELIMTER_CHAR);
      transactionsAry[lineCount].setTransactionIdStr(accountTransactionIdStr);

      if (accountTransactionIdStr != "interest") {
         getline(isStringStream, accountTransactionAmountStr, COMMA_DELIMTER_CHAR);
         accountTransactionAmountFl = stof(accountTransactionAmountStr);
         transactionsAry[lineCount].setTransactionArgumentFl(accountTransactionAmountFl);
      }
      transactionsAry[lineCount].displayTransaction();

   } // for

   cout << endl;
   file.close();

}//ReadInFromTransactionsFile

void ProcessTransactions
(TransactionsClass* transactionsAry, unsigned& transactionsCountUns,
   BankAccountsClass**& bankAccountsAry, unsigned& accountsCountUns) {

   string accountIdStr;
   string accountTransactionIdStr;
   string accountTransactionAmountStr;
   float accountTransactionAmountFl;

   unsigned bankAccountIndex;

   cout << "Process Transactions:" << endl <<
      "---------------------" << endl << endl;

   for (unsigned transactionCount = 0; transactionCount < transactionsCountUns; ++transactionCount) {

      accountIdStr = transactionsAry[transactionCount].getIdentifcationStr();
      if (!getMatchingBankAccountIndexUns(bankAccountsAry, accountsCountUns,
         accountIdStr, bankAccountIndex)) {
         cout << "*** Account Identification Not Found ***" << endl <<
            "Account Id: " << accountIdStr << endl <<
            endl;
         return;
      }

      accountTransactionIdStr = transactionsAry[transactionCount].getTransactionIdStr();

      switch (accountIdStr.at(0)) {

      case 'S':
      case 's':
         cout << "Account Before Transaction" << endl <<
            "--------------------------" << endl;
         static_cast<SavingsAccountsClass*>(bankAccountsAry[bankAccountIndex])->display();

         transactionsAry[transactionCount].displayTransaction();

         if (accountTransactionIdStr == "deposit") {
            accountTransactionAmountFl = transactionsAry[transactionCount].getTransactionArgumentFl();
            bankAccountsAry[bankAccountIndex]->deposit(accountTransactionAmountFl);
         }

         else if (accountTransactionIdStr == "withdraw") {
            accountTransactionAmountFl = transactionsAry[transactionCount].getTransactionArgumentFl();
            static_cast<SavingsAccountsClass*>(bankAccountsAry[bankAccountIndex])->
               savingsWithdraw(accountTransactionAmountFl);
         }

         else if (accountTransactionIdStr == "interest") {
            static_cast<SavingsAccountsClass*>(bankAccountsAry[bankAccountIndex])->
               calculateInterest();
         }
         else {
            cout << "Invalid Transaction Id: " << accountTransactionIdStr << endl << endl;

         }

         cout << "Account After Transaction: " << endl <<
            "--------------------------" << endl;
         static_cast<SavingsAccountsClass*>(bankAccountsAry[bankAccountIndex])->display();

         break;

      case 'C':
      case 'c':
         cout << "Account Before Transaction" << endl <<
            "--------------------------" << endl;
         static_cast<CheckingAccountsClass*>(bankAccountsAry[bankAccountIndex])->display();

         transactionsAry[transactionCount].displayTransaction();

         if (accountTransactionIdStr == "deposit") {
            accountTransactionAmountFl = transactionsAry[transactionCount].getTransactionArgumentFl();
            accountTransactionAmountFl = bankAccountsAry[bankAccountIndex]->getAccountBalanceFl() + accountTransactionAmountFl;
            bankAccountsAry[bankAccountIndex]->setAccountBalanceFl(accountTransactionAmountFl);
         }
         else if (accountTransactionIdStr == "check") {
            accountTransactionAmountFl = transactionsAry[transactionCount].getTransactionArgumentFl();
            static_cast<CheckingAccountsClass*>(bankAccountsAry[bankAccountIndex])->
               cashCheck(accountTransactionAmountFl);
         }

         cout << "Account After Transaction:" << endl <<
            "--------------------------" << endl;
         static_cast<CheckingAccountsClass*>(bankAccountsAry[bankAccountIndex])->display();
         break;

      case'I':
      case 'i':
         cout << "Account Before Transaction:" << endl <<
            "---------------------------" << endl;

         static_cast<InterestCheckingAccountsClass*>(bankAccountsAry[bankAccountIndex])->display();

         transactionsAry[transactionCount].displayTransaction();

         if (accountTransactionIdStr == "deposit") {
            accountTransactionAmountFl = transactionsAry[transactionCount].getTransactionArgumentFl();
            accountTransactionAmountFl = bankAccountsAry[bankAccountIndex]->getAccountBalanceFl() + accountTransactionAmountFl;
            bankAccountsAry[bankAccountIndex]->setAccountBalanceFl(accountTransactionAmountFl);
         }

         else if (accountTransactionIdStr == "check") {
            accountTransactionAmountFl = transactionsAry[transactionCount].getTransactionArgumentFl();
            static_cast<InterestCheckingAccountsClass*>(bankAccountsAry[bankAccountIndex])->
               cashCheck(accountTransactionAmountFl);
         }

         else if (accountTransactionIdStr == "interest") {
            static_cast<InterestCheckingAccountsClass*>(bankAccountsAry[bankAccountIndex])->
               calculateInterest();
         }
         cout << "Account After Transaction:" << endl <<
            "--------------------------" << endl;
         static_cast<InterestCheckingAccountsClass*>(bankAccountsAry[bankAccountIndex])->display();
         break;

      default:
         cout << "*** Illegal Account Indentification In Transaction ***" << endl <<
            "Account Id: " << accountIdStr << endl <<
            endl;
      } // switch

   } // for

   cout << endl;
         
}//ProcessTransactions

bool getMatchingBankAccountIndexUns(
   BankAccountsClass**& bankAccountsAry, unsigned  accountsCountUns,
   string              accountIdStr, unsigned& bankAccountIndex) {
   bankAccountIndex = 0;
   for (; bankAccountIndex < accountsCountUns; ++bankAccountIndex)
      if (bankAccountsAry[bankAccountIndex]->getAcountIdStr() == accountIdStr) {
         return true;
      }
   return false;
}

void UpdateBankAccountsFile
(string bankAccountsFileNameStr,
   BankAccountsClass**& bankAccountsAry, unsigned accountsCountUns) {

   ofstream outFile(bankAccountsFileNameStr);
   CheckFileStreamOpen(bankAccountsFileNameStr, outFile);

   cout << "Updating " << bankAccountsFileNameStr << endl <<
      "-----------------------------" << endl;

   for (unsigned writeIndex = 0; writeIndex < accountsCountUns; ++writeIndex)
      outFile <<
      bankAccountsAry[writeIndex]->getAcountIdStr() << "," <<
      bankAccountsAry[writeIndex]->getAccountOwnerStr() << "," <<
      bankAccountsAry[writeIndex]->getAccountBalanceFl() << endl;

   outFile.close();
};