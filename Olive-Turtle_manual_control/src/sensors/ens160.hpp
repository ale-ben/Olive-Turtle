/**
 * @file ens160.hpp
 * @author Benetton Alessandro (aleben98@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ENS160_HPP
#define ENS160_HPP
#include <Arduino.h>
#include <SparkFun_ENS160.h>

// #define DISABLE_ENS160

/**
 * @brief  Initial configuration for ENS160 sensor
 *
 * @param ambientTemp Ambient temperature (necessary to calibrate sensor)
 * @param ambientHum  Ambient humidity (necessary to calibrate sensor)
 * @return true If the setup succedes
 * @return false If the setup fails
 */
bool ens160Setup(float ambientTemp, float ambientHum);

/**
 * @brief Prints the last read data to the serial port
 *
 */
void ens160PrintData();
#endif
