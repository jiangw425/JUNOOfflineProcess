#include "INCLUDE"
#include "Event/CdWaveformHeader.h"
#include "Event/ElecWaveform.h"
const int m_length = ADCL;
const int Npmt = 17612;
int Ntotal = 100000;
// int Ntotal = 100;
void extractWaves(int is_forceTrigger=0)
{
  bool* hmmtpmt = pmtflag();
  // TFile* f = new TFile("/junofs/production/data-production/Pre-Releases/J19v1r0-Pre3/Laser/center/photon_22000/elecsim/elecsim-1100.root", "read");
  // TFile* f = new TFile("/junofs/production/data-production/Pre-Releases/J19v1r0-Pre3/ACU/Co60/Co60_0_0_17000/elecsim/elecsim-13400.root", "read");
  // TFile* f = new TFile("/junofs/production/data-production/Pre-Releases/J19v1r0-Pre3/C14/C14/elecsim/elecsim-100.root", "read");
  // TFile* f = new TFile("/junofs/production/data-production/Pre-Releases/J19v1r0-Pre3/ACU/AmC/AmC_0_0_17000/elecsim/elecsim-43400.root", "read");
  // TFile* f = TFile::Open("ELECROOTFILE", "read");
  // TTree* tr = (TTree*)f->Get("Event/Elec/ElecEvent");
  TChain *tr = new TChain("Event/CdWaveform/CdWaveformEvt");
  tr->Add("ELECROOTFILE");
  TFile* sf = new TFile("waves.root", "recreate");
  JM::CdWaveformEvt* ee = new JM::CdWaveformEvt();
  tr->SetBranchAddress("CdWaveformEvt", &ee);
  cout << "Total Events: " << tr->GetEntries() << endl;

  TTree* str = new TTree("waves", "waves");
  TH1D* h_tmp = NULL;
  int pmtID;
  str->Branch("waves", "TH1D", &h_tmp);
  str->Branch("pmtid", &pmtID);

  JM::ElecFeeCrate* m_crate;
  int counter_h = 0;
  int counter_n = 0;
  for (int i = 0; i < tr->GetEntries(); i++) {
    // for (int i = 0; i < 1; i++) {
    tr->GetEntry(i);
    const JM::ElecFeeCrate& efc = ee->elecFeeCrate();
    m_crate = const_cast<JM::ElecFeeCrate*>(&efc);
    std::map<int, JM::ElecFeeChannel> feeChannels = m_crate->channelData();
    std::map<int, JM::ElecFeeChannel>::iterator it;
    
    for (it = feeChannels.begin(); it != feeChannels.end(); ++it) {
      JM::ElecFeeChannel& channel = (it->second);
      if (channel.adc().size() == 0) {
        continue;
      }
      pmtID = it->first;
      TString name;
      TString title;
      if (hmmtpmt[pmtID] == true && counter_h < Ntotal) {
        name = Form("waveh_%d", counter_h++);
        title = Form("%d", pmtID);
      } else if (hmmtpmt[pmtID] == false && counter_n < Ntotal) {
        name = Form("waven_%d", counter_n++);
        title = Form("%d", pmtID);
      } else {
        continue;
      }
      h_tmp = new TH1D(name, title, adcl, 0, adcl);
      double baseline = 0;
      for (int j = 0; j < 50; j++) baseline += channel.adc().at(j);
      baseline /= 50.;
      for (int j = 0; j < adcl; j++) {
        h_tmp->SetBinContent(j + 1, channel.adc().at(j) - baseline);
      }
      sf->cd();
      str->Fill();
    }
    if (counter_h >= Ntotal && counter_n >= Ntotal) break;
  }
  str->Write();
}
