#ifndef LDFDATAREADER_H
#define LDFDATAREADER_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <vector>
// ROOT headers
#include "Rtypes.h"
#include "TTree.h"

using namespace std;

// record ID
#define DATA 0x44415441
#define	DIR  0x44495220
#define	HEAD 0x48454144
#define	PAC  0x50414320
#define	SCAL 0x5343414C
#define	DEAD 0x44454144
#define	EF   0x454F4620
#define	PAD  0xFFFFFFFF

#define RECORD_SIZE 32776

class LDFDataReader{
  ifstream *ldf_data_file; // raw data file

  // address navigation
  UInt_t next_record_address;
	UInt_t BytesToNextRecord;
  UInt_t recordID;
  UInt_t event_counter;

  // LDF data frame data
  UInt_t ID[72];
  UInt_t data[72];

public:

  LDFDataReader();
  ~LDFDataReader();

  // open and close raw data file
  Int_t openFile(const char *filename);
  Int_t closeFile();

  void set_BytesToNextRecord(UInt_t record_size) {BytesToNextRecord = record_size;}

  bool get_end_of_file() {return ldf_data_file->eof();}

  UInt_t get_recordID() {return recordID;}
  UInt_t get_BytesToNextRecord() {return BytesToNextRecord;}

  // LDF data
  UInt_t* get_ID() {return ID;}
  UInt_t* get_data() {return data;}

  //Goes to next record
  UInt_t GoToNextRecord();

  // functions for reading raw data
  UInt_t readOneWord();
  UInt_t readRecordID();
  Int_t readDataEvent(); // returns -1 for end of event

  Int_t zeroDataArray();
};
#endif
