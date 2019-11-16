#include "LDFDataReader.h"

// constructor
LDFDataReader::LDFDataReader() {
	ldf_data_file=NULL;
  event_counter = 0;
	BytesToNextRecord=RECORD_SIZE;
}

// destructor
LDFDataReader::~LDFDataReader(){
};

// open raw data file
Int_t LDFDataReader::openFile(const char *filename){

  ldf_data_file = new ifstream(filename, ios::binary);

	// check for opened file
  if(ldf_data_file->is_open()){
    cout << "Opened file " << filename << endl;
    return 0;
  }
  else{
    cout << "File " << filename << " could not be opened." << endl;
    return -1;
  }
}

// close raw data file
Int_t LDFDataReader::closeFile(){
  ldf_data_file->close();

  return 0;
}

UInt_t LDFDataReader::readOneWord(){
	//Reads one 32-bit word
  UInt_t word;
	UInt_t temp_word;

	ldf_data_file->read((char *) &temp_word, 4); // each read stores starting with least significant byte
  BytesToNextRecord = BytesToNextRecord - ldf_data_file->gcount();

	// shift bytes read to memory order
  word = ((temp_word & 0x000000FF) << 24) |
    ((temp_word & 0x0000FF00) << 8) |
    ((temp_word & 0x00FF0000) >> 8) |
    ((temp_word & 0xFF000000) >> 24);

	return word;
}

UInt_t LDFDataReader::readRecordID(){
	UInt_t temp_word;

	ldf_data_file->read((char *) &temp_word, 4); // each read stores starting with least significant byte
  BytesToNextRecord = BytesToNextRecord - ldf_data_file->gcount();

	// shift bytes read to memory order
  recordID = ((temp_word & 0x000000FF) << 24) |
    ((temp_word & 0x0000FF00) << 8) |
    ((temp_word & 0x00FF0000) >> 8) |
    ((temp_word & 0xFF000000) >> 24);

	return recordID;
}

Int_t LDFDataReader::readDataEvent(){
	UInt_t ParameterID;
	UInt_t ParameterData;

  // channel loop for event-----------------------------------------
	UInt_t temp_word = 0;
  UInt_t channels_read = 0;
  do{
    temp_word = readOneWord();

    if(temp_word != 0xFFFFFFFF){
      // parse values
      ParameterID = (temp_word & 0xFF000000) >> 24;
      ParameterData = ((temp_word & 0x000000FF) << 8) | ((temp_word & 0x0000FF00) >> 8);

      data[ParameterID] = ParameterData;
      //cout << "Parameter values" << "\t" << ParameterID << "\t" << ParameterData << endl;
      channels_read++;
    }
  }
  while(temp_word != 0xFFFFFFFF);

  // end of event loop ---------------------------------------------

  if(channels_read == 0){
    return -1;
  }
  else{
		// count event
		event_counter++;
		if(event_counter%1000 == 0){
			cout << "Sorting event: " << event_counter << endl;
		}

	  return channels_read;
  }
}

UInt_t LDFDataReader::GoToNextRecord(){
	UInt_t current_address = ldf_data_file->tellg();
	next_record_address = current_address + BytesToNextRecord;
	ldf_data_file->seekg(next_record_address); // go to next record current_address
	BytesToNextRecord = RECORD_SIZE; // reset bytes to next record

	return next_record_address;
}

Int_t LDFDataReader::zeroDataArray(){
	for(Int_t i=0; i<72; i++){
		ID[i] = 0;
		data[i] = 0;
	}
	return 0;
}
