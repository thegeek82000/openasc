/***************************************************************************
 *   Copyright (C) 2009 by Mikael Larsmark, SM2WMV   *
 *   mike@sm3wmv.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __COMMCLASS_H__
#define __COMMCLASS_H__

#include <QMainWindow>
#include <QThread>
#include <QLinkedList>

#include "qextserialport.h"

//! The serial acknowledge of the computer communication protocol
#define COMPUTER_COMM_ACK				0xFA
//! The serial NOT acknowledge of the computer communication protocol
#define COMPUTER_COMM_NACK			0xFB

//! Command to force the openASC box into bootloader mode
#define COMPUTER_COMM_ENTER_BOOTLOADER		0x01
#define CTRL_REBOOT												0x02
#define CTRL_GET_FIRMWARE_REV							0x03

#define COMPUTER_COMM_REDIRECT_DATA			0x10

#define COMPUTER_COMM_BUTTON_PRESSED		0x20
#define COMPUTER_COMM_RX_ANT_TEXT				0x21

/* START OF BUTTON PRESSED DEFINES */
#define BUTTON_TX1			1
#define BUTTON_TX2			2
#define BUTTON_TX3			3
#define BUTTON_TX4			4

#define BUTTON_RX1			5
#define BUTTON_RX2			6
#define BUTTON_RX3			7
#define BUTTON_RX4			8

#define BUTTON_RXANT1		9
#define BUTTON_RXANT2		10
#define BUTTON_RXANT3		11
#define BUTTON_RXANT4		12
#define BUTTON_RXANT5		13
#define BUTTON_RXANT6		14
#define BUTTON_RXANT7		15
#define BUTTON_RXANT8		16
#define BUTTON_RXANT9		17
#define BUTTON_RXANT10	18

#define BUTTON_TXRX_MODE		19
#define BUTTON_RXANT				20
/* END OF BUTTON PRESSED DEFINES */

class CommClass : public QThread
{
	public:
		CommClass();
		int openPort(QString deviceName);
		int closePort();
		void receiveMsg();
		void sendMessage(char *data, int length);
		void sendMessage(QByteArray& data);
		void addTXMessage(unsigned char cmd, unsigned char length, unsigned char *data);
		void addTXMessage(unsigned char cmd, unsigned char data);
		void checkTXQueue();
		void stopProcess();
		void parseRXQueue();
		bool isOpen();
	private:
	
	protected:
		bool threadActive;
		QextSerialPort *serialPort;
		QByteArray receivedMessage;
		QLinkedList<QByteArray> txQueue;
		QLinkedList<QByteArray> rxQueue;
		bool lastMessageAcked;
		int sent_count;
		void run();
};

#endif
