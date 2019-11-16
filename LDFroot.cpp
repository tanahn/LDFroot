#include "LDFroot.h"

int main(int argc, char *argv[]){
  // check for correct std inputs
  if(argc != 3){
    cout << "Usage: LDFroot [input file] [output file]" << endl;
    return -1;
  }

  // open input file
  ifstream input_file(argv[1]);
  if(!input_file.is_open()){
    cout << "Cannot open file " << argv[1] << endl;
    return -2;
  }

  //-------Read input file parameters-------

  // filename for LDF data file
  string get_directory;
  input_file >> get_directory; // input

  // output file
  string output_filename;
  input_file >> output_filename; // 4th input

  // maximum number of events
  UInt_t EndEvent = 0;
  input_file >> EndEvent; // 5th input

  //------------------------------------------

  // LDFDataReader object
  LDFDataReader *ldf  = new LDFDataReader();

  // open raw data file
  ldf->openFile(get_directory.c_str());

 //------------Data-------------------

  // Root file open
  TFile *output_rootfile  = new TFile(output_filename.c_str(), "RECREATE");

  TTree *tree = new TTree("tree", "tree");

  //---------Make branches------------

  tree->Branch("data", ldf->get_data(), "data[72]/i");

  // read record loop ------------------------------------
  do{
  //for(int i=0; i<4; i++){
    ldf->readRecordID(); // read record ID, first word

    // check if reading past end-of-file
    if(ldf->get_end_of_file()){
      break;
    }


    if(ldf->get_recordID() == DATA){
      //cout << "DATA record!" << endl;

      ldf->readOneWord(); // read next word: 8192

      // event loop
      while(ldf->get_BytesToNextRecord() > 0){
        Int_t channels_read;
        channels_read = ldf->readDataEvent();
        if(channels_read > 0){
          tree->Fill();
          ldf->zeroDataArray();
        }
      }
      // end of event loop -------------------------------
      ldf->set_BytesToNextRecord(RECORD_SIZE); // reset byte counter to next record
    }
    else{
      //cout << "Not DATA record!" << endl;
      ldf->GoToNextRecord();
    }
  }
  while(!ldf->get_end_of_file());
  // end of record loop ----------------------------------

  // save ROOT file
  tree->Write();
  output_rootfile->Write();
  output_rootfile->Close();
  ldf->closeFile();
  return 0;
}
