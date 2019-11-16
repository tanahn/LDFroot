{
  TFile *f = new TFile("output.root");
  TTree *t;
  f->GetObject("tree", t);

  int NEvents = t->GetEntries();
  cout << NEvents << endl;

  int data[72];

  t->SetBranchAddress("data", &data);

  TH1I *h = new TH1I("","",4096,0,4095);

  for(int i = 0; i<NEvents; i++){
    t->GetEntry(i);
    h->Fill(data[60]);
  }

  h->Draw();
}
