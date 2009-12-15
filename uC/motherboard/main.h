/*! \file main.h \brief Main file of the motherboard
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-08
 */
//    Copyright (C) 2008  Mikael Larsmark, SM2WMV
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*! Activate a driver output */
#define BUS_CMD_DRIVER_ACTIVATE_OUTPUT		0x20
/*! Deactivate a driver output */
#define BUS_CMD_DRIVER_DEACTIVATE_OUTPUT	0x21

typedef struct {
	//To see if we have started to read a keyboard command
	unsigned char started;
	unsigned char bit_count;
	unsigned char data;
	unsigned char data_ready;
	unsigned char transmit;
	unsigned char parity;
	unsigned char tx_data;
	unsigned char prev_cmd;
} PS2_STRUCT;

#endif