#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdint.h>

using namespace std;

#define GroundStationOutputChannel cout
const string TELEMETRY_RECIEVER_INPUT_CHANNEL = "inputDataAntenna.txt";

enum class MESSAGE_ID {SATELLITE_INFORMATION_MESSAGE = 1, EXPERIMENT_INFORMATION_MESSAGE = 2, SEND_SUMMARY_MESSAGE = 3};
MESSAGE_ID messageID;

enum class InformationType { SATELLITE_INFORMATION, EXPIRIMENT_INFORMATION, EMPTY};
const uint8_t ARRAY_DATA_STORAGE_SIZE = 5;

struct SatelliteInformation {
   float_t temperature,
      voltage;
};

struct ExperimentInformation {
   uint16_t radiationCount,
      latchupEventsCount;
};

union MasterUnion {
   SatelliteInformation satelliteInformation;
   ExperimentInformation experimentInformation;
};

struct MasterInformation {
   InformationType infoType;
   MasterUnion masterUnion;
};

void ProcessSatelliteInformationMessage
(InformationType& infoType, ifstream& telemetryInputAntennaReceiverChannel,
   MasterInformation& masterInformation, MasterInformation* infoStoredArray, uint8_t& entryPosition);

void ProcessSatelliteExprimentMessage
(InformationType& infoType, ifstream& telemetryInputAntennaReceiverChannel,
   MasterInformation& masterInformation, MasterInformation* infoStoredArray, uint8_t& entryPosition);
 
   int main() {
   MasterInformation infoStoredArray[ARRAY_DATA_STORAGE_SIZE];
   uint8_t entryPositionIndex = 0;
   MasterInformation masterInformation;
   ifstream telemetryInputAntennaReceiverChannel;

   telemetryInputAntennaReceiverChannel.open(TELEMETRY_RECIEVER_INPUT_CHANNEL, ios::in);
   if (telemetryInputAntennaReceiverChannel.fail()) {
      cout << "File " << TELEMETRY_RECIEVER_INPUT_CHANNEL << " could not be opened." << endl;
      cout << "Press eneter key once or twice to end..."; cin.ignore(); cin.get();
      exit(EXIT_FAILURE);
   }

   GroundStationOutputChannel << setprecision(1) << fixed << showpoint;

   for (uint8_t index = 0; index < ARRAY_DATA_STORAGE_SIZE; index++)
      infoStoredArray[index].infoType = InformationType::EMPTY;

   uint16_t messageId;
   while (telemetryInputAntennaReceiverChannel >> messageId) {
      messageID = MESSAGE_ID(messageId);

      switch (messageID) {
      case MESSAGE_ID::SATELLITE_INFORMATION_MESSAGE:
         ProcessSatelliteInformationMessage(masterInformation.infoType,
            telemetryInputAntennaReceiverChannel,
            masterInformation,
            infoStoredArray,
            entryPositionIndex);
         break;

      case MESSAGE_ID::EXPERIMENT_INFORMATION_MESSAGE:
         ProcessSatelliteExprimentMessage(masterInformation.infoType,
            telemetryInputAntennaReceiverChannel,
            masterInformation,
            infoStoredArray,
            entryPositionIndex);
         break;

      case MESSAGE_ID::SEND_SUMMARY_MESSAGE:

         float_t minTemperature = -50,
            maxTemperature = 212;
         float_t maxVoltage = 100.0,
            minVoltage = 0.0;

         uint16_t totalRadiationCount = 0,
            totalLatchupEventsCount = 0,
            infoSICount = 0,
            infoEICount = 0;


         for (uint8_t index = 0; index < ARRAY_DATA_STORAGE_SIZE; index++) {
            switch (infoStoredArray[index].infoType) {

            case InformationType::SATELLITE_INFORMATION:
               GroundStationOutputChannel <<
                  "Temperature: " <<
                  infoStoredArray[index].masterUnion.satelliteInformation.temperature << endl <<
                  "Voltage: " <<
                  infoStoredArray[index].masterUnion.satelliteInformation.voltage << endl << endl;
               infoSICount++;

               if (maxTemperature < infoStoredArray[index].masterUnion.satelliteInformation.temperature)
                  maxTemperature = infoStoredArray[index].masterUnion.satelliteInformation.temperature;

               if (minTemperature > infoStoredArray[index].masterUnion.satelliteInformation.temperature)
                  minTemperature = infoStoredArray[index].masterUnion.satelliteInformation.temperature;

               if (maxVoltage < infoStoredArray[index].masterUnion.satelliteInformation.voltage)
                  maxVoltage = infoStoredArray[index].masterUnion.satelliteInformation.voltage;

               if (minVoltage > infoStoredArray[index].masterUnion.satelliteInformation.voltage)
                  minVoltage = infoStoredArray[index].masterUnion.satelliteInformation.voltage;
               break;

            case InformationType::EXPIRIMENT_INFORMATION:
               GroundStationOutputChannel <<
                  "Radiation Count: " <<
                  infoStoredArray[index].masterUnion.experimentInformation.radiationCount << endl <<
                  "Latchup Event Count: " <<
                  infoStoredArray[index].masterUnion.experimentInformation.latchupEventsCount << endl << endl;
               infoEICount++;

               totalRadiationCount += infoStoredArray[index].masterUnion.experimentInformation.radiationCount;
               totalLatchupEventsCount += infoStoredArray[index].masterUnion.experimentInformation.latchupEventsCount;
               break;

            case InformationType::EMPTY:

               break;

            } // end switch
         } // end for
         
         GroundStationOutputChannel <<
            "Summary Information" << endl <<
            "-------------------" << endl <<
            "Number of Satellite Information Records: " << infoSICount << endl <<
            "Number of Experiment Information Records: " << infoEICount << endl <<
            "Total Radiation Count: " << totalRadiationCount << endl <<
            "Total Latchup Event Count: " << totalLatchupEventsCount << endl <<
            "Maximum Temperature: " << maxTemperature << endl <<
            "Minimum Temperature: " << minTemperature << endl <<
            "Maximum Voltage: " << maxVoltage << endl <<
            "Minimum Voltage: " << minVoltage << endl << endl;


         cout << "Press the enter key once or twice to continue..."; cin.ignore(); cin.get();

      } // end switch
   } // end while

   return(EXIT_SUCCESS);
} // end main

            void ProcessSatelliteInformationMessage(InformationType & infoType,
               ifstream& telemetryInputAntennaReceiverChannel,
               MasterInformation& masterInformation,
               MasterInformation* infoStoredArray,
               uint8_t& entryPositionIndex) {

               infoType = InformationType::SATELLITE_INFORMATION;
               telemetryInputAntennaReceiverChannel >>
                  masterInformation.masterUnion.satelliteInformation.temperature >>
                  masterInformation.masterUnion.satelliteInformation.voltage;
               infoStoredArray[entryPositionIndex] = masterInformation;
               entryPositionIndex = (entryPositionIndex == ARRAY_DATA_STORAGE_SIZE - 1) ? 0 : ++entryPositionIndex;
            }

            void ProcessSatelliteExprimentMessage(InformationType& infoType,
               ifstream& telemetryInputAntennaReceiverChannel,
               MasterInformation& masterInformation,
               MasterInformation* infoStoredArray,
               uint8_t& entryPositionIndex) {

               infoType = InformationType::EXPIRIMENT_INFORMATION;
               telemetryInputAntennaReceiverChannel >>
                  masterInformation.masterUnion.experimentInformation.radiationCount >>
                  masterInformation.masterUnion.experimentInformation.latchupEventsCount;
               infoStoredArray[entryPositionIndex] = masterInformation;
               entryPositionIndex = (entryPositionIndex == ARRAY_DATA_STORAGE_SIZE - 1) ? 0 : ++entryPositionIndex;
            }
