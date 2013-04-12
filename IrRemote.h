/*
 * IrRemote
 * @version 20130409
 * $Id:$
 * Copyright 2013 Ken Chou
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.htm http://arcfn.com
 * Edited by Mitra to add new controller SANYO
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 */

#ifndef IrRemote_h
#define IrRemote_h

namespace IrRemote
{

// The following are compile-time library options.
// If you change them, recompile the library.
// If DEBUG is defined, a lot of debugging output will be printed during decoding.
// TEST must be defined for the IRtest unittests to work.  It will make some
// methods virtual, which will be slightly slower, which is why it is optional.
// #define DEBUG
// #define TEST

// Results returned from the decoder
class IrData
{
	public:
		int encodeType; // NEC, SONY, RC5, UNKNOWN
		unsigned int panasonicAddress; // This is only used for decoding Panasonic data
		unsigned long value; // Decoded value
		int bits; // Number of bits in decoded value
		volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
		int rawlen; // Number of records in rawbuf.
};

// Values for decode_type
#define NEC 1
#define SONY 2
#define RC5 3
#define RC6 4
#define DISH 5
#define SHARP 6
#define PANASONIC 7
#define JVC 8
#define SANYO 9
#define MITSUBISHI 10
#define UNKNOWN -1

// Decoded value for NEC when a repeat code is received
#define REPEAT 0xffffffff

// main class for receiving IR
class Receiver
{
	public:
		Receiver(int recvPin);
		void blink13(int blinkFlag);
		int decode(IrData *results);
		void enableIn();
		void resume();
	private:
		// These are called by decode
		int getRClevel(IrData *results, int *offset, int *used, int t1);
		long decodeNec(IrData *results);
		long decodeSony(IrData *results);
		long decodeSanyo(IrData *results);
		long decodeMitsubishi(IrData *results);
		long decodeRc5(IrData *results);
		long decodeRc6(IrData *results);
		long decodePanasonic(IrData *results);
		long decodeJvc(IrData *results);
		long decodeHash(IrData *results);
		int compare(unsigned int oldval, unsigned int newval);
} 
;

// Only used for testing; can remove virtual for shorter code
#ifdef TEST
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif

class Transmitter
{
	public:
		Transmitter() {};
		void send(IrData *data);
		void sendRaw(unsigned int buf[], int len, int hz);
		void sendNec(unsigned long data, int nbits);
		void sendSony(unsigned long data, int nbits);
		// Neither Sanyo nor Mitsubishi send is implemented yet
		//  void sendSanyo(unsigned long data, int nbits);
		//  void sendMitsubishi(unsigned long data, int nbits);
		void sendRc5(unsigned long data, int nbits);
		void sendRc6(unsigned long data, int nbits);
		void sendDish(unsigned long data, int nbits);
		void sendSharp(unsigned long data, int nbits);
		void sendPanasonic(unsigned int address, unsigned long data);
		void sendJvc(unsigned long data, int nbits, int repeat); // *Note instead of sending the REPEAT constant if you want the JVC repeat signal sent, send the original code value and change the repeat argument from 0 to 1. JVC protocol repeats by skipping the header NOT by sending a separate code value like NEC does.
		// private:
		void enableOut(int khz);
		VIRTUAL void mark(int usec);
		VIRTUAL void space(int usec);
}
;

// Some useful constants

#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100 // Length of raw duration buffer

// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
#define MARK_EXCESS 100

}
#endif
