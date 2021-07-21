/*

Arduino Library for Sensirion SHT3X-DIS Digital Humidity & Temperature Sensors
Written by AA
---

The MIT License (MIT)

Copyright (c) 2015-2017 ClosedCube Limited

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#include <Wire.h>

#include "ClosedCube_SHT31D.h"

ClosedCube_SHT31D::ClosedCube_SHT31D()
{
}

SHT31D_ErrorCode ClosedCube_SHT31D::begin(uint8_t address) {
	SHT31D_ErrorCode error = SHT3XD_NO_ERROR;
	_address = address;
	return error;
}

SHT31D_ErrorCode ClosedCube_SHT31D::reset()
{
	return  softReset();
}

SHT31D ClosedCube_SHT31D::periodicFetchData()
{
	SHT31D_ErrorCode error = writeCommand(SHT3XD_CMD_FETCH_DATA);
	if (error == SHT3XD_NO_ERROR)
		return readTemperatureAndHumidity();
	else
		return returnError(error);
}

SHT31D_ErrorCode ClosedCube_SHT31D::periodicStop() {
	return writeCommand(SHT3XD_CMD_STOP_PERIODIC);
}

SHT31D_ErrorCode ClosedCube_SHT31D::periodicStart(SHT31D_Repeatability repeatability, SHT31D_Frequency frequency)
{
	SHT31D_ErrorCode error;

	switch (repeatability)
	{
	case SHT3XD_REPEATABILITY_LOW:
		switch (frequency)
		{
		case SHT3XD_FREQUENCY_HZ5:
			error = writeCommand(SHT3XD_CMD_PERIODIC_HALF_L);
			break;
		case SHT3XD_FREQUENCY_1HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_1_L);
			break;
		case SHT3XD_FREQUENCY_2HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_2_L);
			break;
		case SHT3XD_FREQUENCY_4HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_4_L);
			break;
		case SHT3XD_FREQUENCY_10HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_10_L);
			break;
		default:
			error = SHT3XD_PARAM_WRONG_FREQUENCY;
			break;
		}
		break;
	case SHT3XD_REPEATABILITY_MEDIUM:
		switch (frequency)
		{
		case SHT3XD_FREQUENCY_HZ5:
			error = writeCommand(SHT3XD_CMD_PERIODIC_HALF_M);
			break;
		case SHT3XD_FREQUENCY_1HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_1_M);
			break;
		case SHT3XD_FREQUENCY_2HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_2_M);
			break;
		case SHT3XD_FREQUENCY_4HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_4_M);
			break;
		case SHT3XD_FREQUENCY_10HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_10_M);
			break;
		default:
			error = SHT3XD_PARAM_WRONG_FREQUENCY;
			break;
		}
		break;

	case SHT3XD_REPEATABILITY_HIGH:
		switch (frequency)
		{
		case SHT3XD_FREQUENCY_HZ5:
			error = writeCommand(SHT3XD_CMD_PERIODIC_HALF_H);
			break;
		case SHT3XD_FREQUENCY_1HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_1_H);
			break;
		case SHT3XD_FREQUENCY_2HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_2_H);
			break;
		case SHT3XD_FREQUENCY_4HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_4_H);
			break;
		case SHT3XD_FREQUENCY_10HZ:
			error = writeCommand(SHT3XD_CMD_PERIODIC_10_H);
			break;
		default:
			error = SHT3XD_PARAM_WRONG_FREQUENCY;
			break;
		}
		break;
	default:
		error = SHT3XD_PARAM_WRONG_REPEATABILITY;
		break;
	}

	delay(100);

	return error;
}

SHT31D ClosedCube_SHT31D::readTempAndHumidity(SHT31D_Repeatability repeatability, SHT31D_Mode mode, uint8_t timeout)
{
	SHT31D result;

	switch (mode) {
	case SHT3XD_MODE_CLOCK_STRETCH:
		result = readTempAndHumidityClockStretch(repeatability);
		break;
	case SHT3XD_MODE_POLLING:
		result = readTempAndHumidityPolling(repeatability, timeout);
		break;
	default:
		result = returnError(SHT3XD_PARAM_WRONG_MODE);
		break;
	}

	return result;
}


SHT31D ClosedCube_SHT31D::readTempAndHumidityClockStretch(SHT31D_Repeatability repeatability)
{
	SHT31D_ErrorCode error = SHT3XD_NO_ERROR;
	SHT31D_Commands command;

	switch (repeatability)
	{
	case SHT3XD_REPEATABILITY_LOW:
		error = writeCommand(SHT3XD_CMD_CLOCK_STRETCH_L);
		break;
	case SHT3XD_REPEATABILITY_MEDIUM:
		error = writeCommand(SHT3XD_CMD_CLOCK_STRETCH_M);
		break;
	case SHT3XD_REPEATABILITY_HIGH:
		error = writeCommand(SHT3XD_CMD_CLOCK_STRETCH_H);
		break;
	default:
		error = SHT3XD_PARAM_WRONG_REPEATABILITY;
		break;
	}

	delay(50);

	if (error == SHT3XD_NO_ERROR) {
		return readTemperatureAndHumidity();
	} else {
		return returnError(error);
	}

}


SHT31D ClosedCube_SHT31D::readTempAndHumidityPolling(SHT31D_Repeatability repeatability, uint8_t timeout)
{
	SHT31D_ErrorCode error = SHT3XD_NO_ERROR;
	SHT31D_Commands command;

	switch (repeatability)
	{
	case SHT3XD_REPEATABILITY_LOW:
		error = writeCommand(SHT3XD_CMD_POLLING_L);
		break;
	case SHT3XD_REPEATABILITY_MEDIUM:
		error = writeCommand(SHT3XD_CMD_POLLING_M);
		break;
	case SHT3XD_REPEATABILITY_HIGH:
		error = writeCommand(SHT3XD_CMD_POLLING_H);
		break;
	default:
		error = SHT3XD_PARAM_WRONG_REPEATABILITY;
		break;
	}

	delay(50);

	if (error == SHT3XD_NO_ERROR) {
		return readTemperatureAndHumidity();
	} else {
		return returnError(error);
	}

}

SHT31D ClosedCube_SHT31D::readAlertHighSet() {
	return readAlertData(SHT3XD_CMD_READ_ALR_LIMIT_HS);
}

SHT31D ClosedCube_SHT31D::readAlertHighClear() {
	return readAlertData(SHT3XD_CMD_READ_ALR_LIMIT_HC);
}

SHT31D ClosedCube_SHT31D::readAlertLowSet() {
	return readAlertData(SHT3XD_CMD_READ_ALR_LIMIT_LS);
}

SHT31D ClosedCube_SHT31D::readAlertLowClear() {
	return readAlertData(SHT3XD_CMD_READ_ALR_LIMIT_LC);
}


SHT31D_ErrorCode ClosedCube_SHT31D::writeAlertHigh(float temperatureSet, float temperatureClear, float humiditySet, float humidityClear) {
	SHT31D_ErrorCode error = writeAlertData(SHT3XD_CMD_WRITE_ALR_LIMIT_HS, temperatureSet, humiditySet);
	if (error == SHT3XD_NO_ERROR)
		error = writeAlertData(SHT3XD_CMD_WRITE_ALR_LIMIT_HC, temperatureClear, humidityClear);

	return error;
}

SHT31D_ErrorCode ClosedCube_SHT31D::writeAlertLow(float temperatureClear, float temperatureSet, float humidityClear, float humiditySet) {
	SHT31D_ErrorCode error = writeAlertData(SHT3XD_CMD_WRITE_ALR_LIMIT_LS, temperatureSet, humiditySet);
	if (error == SHT3XD_NO_ERROR)
		writeAlertData(SHT3XD_CMD_WRITE_ALR_LIMIT_LC, temperatureClear, humidityClear);

	return error;
}

SHT31D_ErrorCode ClosedCube_SHT31D::writeAlertData(SHT31D_Commands command, float temperature, float humidity)
{
	SHT31D_ErrorCode  error;

	if ((humidity < 0.0) || (humidity > 100.0) || (temperature < -40.0) || (temperature > 125.0))
	{
		error = SHT3XD_PARAM_WRONG_ALERT;
	} else
	{
		uint16_t rawTemperature = calculateRawTemperature(temperature);
		uint16_t rawHumidity = calculateRawHumidity(humidity);
		uint16_t data = (rawHumidity & 0xFE00) | ((rawTemperature >> 7) & 0x001FF);

		uint8_t	buf[2];
		buf[0] = data >> 8;
		buf[1] = data & 0xFF;

		uint8_t checksum = calculateCrc(buf);

		Wire.beginTransmission(_address);
		Wire.write(command >> 8);
		Wire.write(command & 0xFF);
		Wire.write(buf[0]);
		Wire.write(buf[1]);
		Wire.write(checksum);
		return (SHT31D_ErrorCode)(-10 * Wire.endTransmission());
	}

	return error;
}


SHT31D_ErrorCode ClosedCube_SHT31D::writeCommand(SHT31D_Commands command)
{
	Wire.beginTransmission(_address);
	Wire.write(command >> 8);
	Wire.write(command & 0xFF);
	return (SHT31D_ErrorCode)(-10 * Wire.endTransmission());
}

SHT31D_ErrorCode ClosedCube_SHT31D::softReset() {
	return writeCommand(SHT3XD_CMD_SOFT_RESET);
}

SHT31D_ErrorCode ClosedCube_SHT31D::generalCallReset() {
	Wire.beginTransmission(0x0);
	Wire.write(0x06);
	return (SHT31D_ErrorCode)(-10 * Wire.endTransmission());
}

SHT31D_ErrorCode ClosedCube_SHT31D::heaterEnable() {
	return writeCommand(SHT3XD_CMD_HEATER_ENABLE);
}

SHT31D_ErrorCode ClosedCube_SHT31D::heaterDisable() {
	return writeCommand(SHT3XD_CMD_HEATER_DISABLE);
}

SHT31D_ErrorCode ClosedCube_SHT31D::artEnable() {
	return writeCommand(SHT3XD_CMD_ART);
}


uint32_t ClosedCube_SHT31D::readSerialNumber()
{
	uint32_t result = SHT3XD_NO_ERROR;
	uint16_t buf[2];

	if (writeCommand(SHT3XD_CMD_READ_SERIAL_NUMBER) == SHT3XD_NO_ERROR) {
		if (read(buf, 2) == SHT3XD_NO_ERROR) {
			result = (buf[0] << 16) | buf[1];
		}
	}

	return result;
}

SHT31D_RegisterStatus ClosedCube_SHT31D::readStatusRegister()
{
	SHT31D_RegisterStatus result;

	SHT31D_ErrorCode error = writeCommand(SHT3XD_CMD_READ_STATUS);
	if (error == SHT3XD_NO_ERROR)
		error = read(&result.rawData, 1);

	return result;
}

SHT31D_ErrorCode ClosedCube_SHT31D::clearAll() {
	return writeCommand(SHT3XD_CMD_CLEAR_STATUS);
}


SHT31D ClosedCube_SHT31D::readTemperatureAndHumidity()
{
	SHT31D result;

	result.t = 0;
	result.rh = 0;

	SHT31D_ErrorCode error;
	uint16_t buf[2];

	if (error == SHT3XD_NO_ERROR)
		error = read(buf, 2);

	if (error == SHT3XD_NO_ERROR) {
		result.t = calculateTemperature(buf[0]);
		result.rh = calculateHumidity(buf[1]);
	}
	result.error = error;

	return result;
}

SHT31D ClosedCube_SHT31D::readAlertData(SHT31D_Commands command)
{
	SHT31D result;

	result.t = 0;
	result.rh = 0;

	SHT31D_ErrorCode error;
	
	uint16_t buf;

	error = writeCommand(command);

	if (error == SHT3XD_NO_ERROR)
		error = read(&buf, 1);

	if (error == SHT3XD_NO_ERROR) {
		result.rh = calculateHumidity(buf & 0xFE00);
		result.t = calculateTemperature(buf << 7);
	}

	result.error = error;

	return result;
}

SHT31D_ErrorCode ClosedCube_SHT31D::read(uint16_t* data, uint8_t numOfPair)
{
	uint8_t	buf[2];
	uint8_t checksum;

	const uint8_t numOfBytes = numOfPair * 3;
	Wire.requestFrom(_address, numOfBytes);

	int counter = 0;

	for (counter = 0; counter < numOfPair; counter++) {
		Wire.readBytes(buf, (uint8_t)2);
		checksum = Wire.read();

		if (checkCrc(buf, checksum) != 0)
			return SHT3XD_CRC_ERROR;

		data[counter] = (buf[0] << 8) | buf[1];
	}

	return SHT3XD_NO_ERROR;
}


uint8_t ClosedCube_SHT31D::checkCrc(uint8_t data[], uint8_t checksum)
{
	return calculateCrc(data) != checksum;
}

float ClosedCube_SHT31D::calculateTemperature(uint16_t rawValue)
{
	return 175.0f * (float)rawValue / 65535.0f - 45.0f;
}


float ClosedCube_SHT31D::calculateHumidity(uint16_t rawValue)
{
	return 100.0f * rawValue / 65535.0f;
}

uint16_t ClosedCube_SHT31D::calculateRawTemperature(float value)
{
	return (value + 45.0f) / 175.0f * 65535.0f;
}

uint16_t ClosedCube_SHT31D::calculateRawHumidity(float value)
{
	return value / 100.0f * 65535.0f;
}

uint8_t ClosedCube_SHT31D::calculateCrc(uint8_t data[])
{
	uint8_t bit;
	uint8_t crc = 0xFF;
	uint8_t dataCounter = 0;

	for (; dataCounter < 2; dataCounter++)
	{
		crc ^= (data[dataCounter]);
		for (bit = 8; bit > 0; --bit)
		{
			if (crc & 0x80)
				crc = (crc << 1) ^ 0x131;
			else
				crc = (crc << 1);
		}
	}

	return crc;
}

SHT31D ClosedCube_SHT31D::returnError(SHT31D_ErrorCode error) {
	SHT31D result;
	result.t = 0;
	result.rh = 0;
	result.error = error;
	return result;
}
