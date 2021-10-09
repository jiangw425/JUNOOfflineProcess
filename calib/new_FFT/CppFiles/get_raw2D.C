#include "/junofs/users/jiangw/include/myincludes.h"  
#include "Event/ElecHeader.h"
#include "Event/ElecFeeCrate.h"
#include "Event/ElecFeeChannel.h"
#include "TH2D.h"
const int m_length = 1250;
const int Npmt = 17612;
int Ntotal = 200000;
// int Ntotal = 100;
double integral(JM::ElecFeeChannel& channel);
void setPMTFlag();
void get_raw2D_NUM()
{

  bool debug=false;
//bool debug=true;

  bool* hmmtpmt = pmtflag();
  // TFile* f = new TFile("/junofs/production/data-production/Pre-Releases/J19v1r0-Pre3/Laser/center/photon_22000/elecsim/elecsim-1100.root", "read");
  // TFile* f = new TFile("/junofs/production/data-production/Pre-Releases/J19v1r0-Pre3/ACU/Co60/Co60_0_0_17000/elecsim/elecsim-13400.root", "read");
  // TFile* f = new TFile("/junofs/production/data-production/Pre-Releases/J19v1r0-Pre3/C14/C14/elecsim/elecsim-100.root", "read");
  // TFile* f = new TFile("/junofs/production/data-production/Pre-Releases/J19v1r0-Pre3/ACU/AmC/AmC_0_0_17000/elecsim/elecsim-43400.root", "read");
  //TFile* f = new TFile("/afs/ihep.ac.cn/users/v/valprod0/J20v2r0-Pre0/ACU+CLS/Ge68/Ge68_0_0_0/elecsim/root/elecsim-70001.root", "read");
  // TFile *f = TFile::Open("ELECROOTFILE","read");

    //TFile* f = new TFile("elecsim-70001-2.root", "read");
  // TTree* tr = (TTree*)f->Get("Event/Elec/ElecEvent");
  TChain *tr = new TChain("Event/Elec/ElecEvent");
  tr->Add("ELECROOTFILE-NUM.root");// elecsim - NUM .root
  TFile* sf = new TFile("../user-root/spef-NUM.root", "recreate");
  TH2D* rawh2D=new TH2D("rawh2D", "Raw Distribution of HAM", 600, 0, 600, 5000, 0, 5000);
  TH2D* rawn2D=new TH2D("rawn2D", "Raw Distribution of MCP", 600, 0, 600, 5000, 0, 5000);
  JM::ElecEvent* ee = new JM::ElecEvent();
  tr->SetBranchAddress("ElecEvent", &ee);
  cout << "Total Events: " << tr->GetEntries() << endl;

  TH1D* h_tmp = NULL;
  int pmtID;

  JM::ElecFeeCrate* m_crate;
  int counter_h = 0;
  cout<<"The Entry is "<<tr->GetEntries()<<endl;
  int counter_n = 0;

  const int f_length=1250;
  int f_length2=f_length;
  TVirtualFFT* fft_forward=TVirtualFFT::FFT(1, &f_length2, "R2C EX K");
  double input_raw[f_length];
  double re_full[f_length];
  double im_full[f_length];
  double mo_full[f_length];
  
	// TVirtualFFT* fft_back = TVirtualFFT::FFT(1, &f_length, "C2R EX K");

  
  int entries=tr->GetEntries();
  if(debug==true)entries=1;
  for (int i = 0; i < entries; i++) {
    // for (int i = 0; i < 1; i++) {
    tr->GetEntry(i);
    // if(i%100==0){
      // cout << "Processing evt: "<<i<<", counter_n: " << counter_n << "/" << Ntotal << ", counter_h: " << counter_h << "/" <<Ntotal << endl;
    // }
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
        ++counter_h;
        name = "h";
      } else if (hmmtpmt[pmtID] == false && counter_n < Ntotal) {
        ++counter_n;
        name = "n";
      } else {
        continue;
      }
      // h_tmp = new TH1D(name, title, 1250, 0, 1250);
      double baseline = 0;
      for (int j = 0; j < 50; j++) baseline += channel.adc().at(j);
      baseline /= 50.;

      for (int j = 0; j < 1250; j++) {
        // h_tmp->SetBinContent(j + 1, channel.adc().at(j) - baseline);
        input_raw[j]=channel.adc().at(j) - baseline;
      }

      for (int i = 0; i < f_length; i++) {
      re_full[i] = 0;
      im_full[i] = 0;
      }

      fft_forward->SetPoints(input_raw);
      fft_forward->Transform();
      fft_forward->GetPointsComplex(re_full, im_full);
      if(name=="h")
      {
      for (int i = 0; i < f_length; i++) {
        mo_full[i]=sqrt(re_full[i]*re_full[i]+im_full[i]*im_full[i]);
        rawh2D->Fill(i,mo_full[i]);
      }
      }
      else if(name=="n")
      {
      for (int i = 0; i < f_length; i++) {
        mo_full[i]=sqrt(re_full[i]*re_full[i]+im_full[i]*im_full[i]);
        rawn2D->Fill(i,mo_full[i]);
      }
      }
    }
    if (counter_h >= Ntotal && counter_n >= Ntotal) break;
  }
		TCanvas *c2=new TCanvas("c_rawh2D","c_rawh2D",800,600);
		// c2->SetLogy();
    rawh2D->SetStats(false);
		rawh2D->SetTitle("rawh2D");
		rawh2D->SetXTitle("Frequency ( MHz ) ");
		rawh2D->DrawCopy("colz");
		c2->SaveAs("rawh-NUM.png");

		TCanvas *c1=new TCanvas("c_rawn2D","c_rawn2D",800,600);
		// c2->SetLogy();
    rawn2D->SetStats(false);
		rawn2D->SetTitle("rawn2D");
		rawn2D->SetXTitle("Frequency ( MHz ) ");
		rawn2D->DrawCopy("colz");
		c1->SaveAs("rawn-NUM.png");

      sf->cd();
      rawh2D->Write();
      rawn2D->Write();
    cout << "Successfully" << endl;
  
}

double integral(JM::ElecFeeChannel& channel)
{
  vector<unsigned int>& adc = channel.adc();
  double baseline = 0;
  double rms = 0;
  for (int i = 0; i < 50 && i < m_length; i++) baseline += adc.at(i);
  baseline /= 50.;
  double inte = 0;
  for (int i = 50; i < m_length; i++) {
    if (i > 350 && (adc.at(i) - baseline) < 0) break;
    inte += (adc.at(i) - baseline);
  }
  return inte;
}
