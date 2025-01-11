/*
 * LTC6804.h
 *
 *  Created on: Jan 11, 2025
 *      Author: verma
 */

#ifndef INC_LTC6804_H_
#define INC_LTC6804_H_

typedef struct{
	uint16_t packVoltage;
	uint8_t SOC;
}BMSData;

typedef struct{
	uint16_t slaveID;
	uint16_t peakTemp;
	uint16_t lowestTemp;
	uint16_t highestCellVoltage;
	uint16_t lowestCellVoltage;
}BMSBoard;

// Function to check how many cells are present
uint8_t populateBP(BMSBoard *slaveBoard);

// Get voltage and temperature data from bms slave
uint16_t getSlaveData(BMSBoard *slaveBoard);

// Get highest cell voltage
uint16_t getPeakCellVoltage(BMSBoard *slaveBoard);

// Get lowest cell voltage
uint16_t getLowCellVoltage(BMSBoard *slaveBoard);

// Get highest cell temp
uint16_t getPeakCellTemp(BMSBoard *slaveBoard);

// Get lowest cell temp
uint16_t getLowCellTemp(BMSBoard *slaveBoard);

#endif /* INC_LTC6804_H_ */
