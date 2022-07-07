/* Copyright 2018 The MathWorks, Inc. */
/*
 * Contains all the prototypes for the checksum function implementations
 */

boolean_T verify_crc8_j1850(const uint8_T *, uint16_T, uint16_T);
uint8_T   calculate_crc8_j1850(const uint8_T *, uint16_T, uint16_T);
uint8_T   calculate_crc8_j1850_withInit(const uint8_T *, uint8_T, uint16_T, uint16_T);

uint8_T   calculate_crc8_j1850_bytePacked(const uint8_T *, uint16_T, uint16_T);
uint8_T   calculate_crc8_j1850_withInit_bytePacked(const uint8_T *, uint8_T, uint16_T, uint16_T);
