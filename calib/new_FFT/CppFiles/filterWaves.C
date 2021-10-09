#include "/junofs/users/jiangw/include/myincludes.h"
// void filterWaves()
void filterWaves()
{
  TH1D* spere[17612];
  TH1D* speim[17612];
  gStyle->SetOptStat(0);
  bool* hmmtpmt = pmtflag();
  // TFile* spef = new TFile("/junofs/users/zhangxt/20inch/rec/deconvolution/runs/20191009sample/Laser/center/photon_22000/SPE_v19.test.root", "read");
  // TFile* spef = new TFile("/junofs/users/zhangxt/20inch/rec/deconvolution/runs/20191009sample/ACU/Ge68/Ge68_0_0_0000/SPE_v19.test.root", "read");
  //TFile* spef = new TFile("/junofs/users/zhangxt/20inch/rec/deconvolution/runs/20200504sample-J20v1r0-Pre0/wfExt/SPE_v20.root", "read");
  TFile* spef = TFile::Open("SPE.root","read");
  TString spename;
  for (int i = 0; i < 17612; i++) {
    spename = Form("PMTID_%d_SPERE", i);
    spere[i] = (TH1D*)spef->Get(spename);
    spere[i]->SetDirectory(0);
    spename = Form("PMTID_%d_SPEIM", i);
    speim[i] = (TH1D*)spef->Get(spename);
    speim[i]->SetDirectory(0);
    if (i % 5000 == 0) cout << i << " finished!" << endl;
  }
  spef->Close();
  cout << "SPE spectra loaded successfully!" << endl;

  TFile* ffile = new TFile("filter.root", "read");

  // TH1D* h_filter0 = filter(50, 350, 30);
  // h_filter0->SetNameTitle("f0", "f0");
  TH1D* h_filter0 = new TH1D("f0", "f0", 1250, 0, 1250);
  for (int i = 0; i < 1250; i++) h_filter0->SetBinContent(i + 1, 1.);

  // nnvt
  // TH1D* h_filter1 = filter(100, 150, 15);
  // TH1D* h_filter1 = filter(10, 188, 83);
  // TH1D* h_filter1 = (TH1D*)ffile->Get("fn1");
  TH1D* h_filter1 = (TH1D*)ffile->Get("fn0");
  // h_filter1->SetNameTitle("f1", "f1");

  // hmmt
  // TH1D* h_filter2 = filter(80, 130, 15);
  // TH1D* h_filter2 = filter(10, 200, 63);
  // TH1D* h_filter2 = (TH1D*)ffile->Get("fh1");
  TH1D* h_filter2 = (TH1D*)ffile->Get("fh0");
  // h_filter2->SetNameTitle("f2", "f2");

  cout << "Filters Generated Successfully!" << endl;

  TFile* f = new TFile("waves.root", "read");
  TH1D* w_tmp = 0;
  int id = 0;
  TString name;

  TTree* tr = (TTree*)f->Get("waves");
  tr->SetBranchAddress("waves", &w_tmp);
  tr->SetBranchAddress("pmtid", &id);
  cout << "Total Events: " << tr->GetEntries() << endl;

  TFile* sf = new TFile("waves_trans.root", "recreate");
  //TFile* sf = new TFile("waves.root", "recreate");
  sf->cd();
  TH1D* rf = 0;
  TH1D* ff = 0;
  TH1D* ft = 0;
  TH1D* df = 0;
  TH1D* w = 0;
  TH1D* spe2raw = 0;
  double arr[1250];
  TTree* str = new TTree("waves_trans", "waves_trans");
  str->Branch("rawfmag", "TH1D", &rf);
  str->Branch("rawh", "TH1D", &w_tmp);
  str->Branch("filteredfmag", "TH1D", &ff);
  str->Branch("filteredt", "TH1D", &ft);
  str->Branch("dividedfmag", "TH1D", &df);
  str->Branch("dividedt", "TH1D", &w);
  str->Branch("spe2raw", "TH1D", &spe2raw);
  str->Branch("pmtid", &id);

  int start = 0;
  int end = 50000;
  // int end = tr->GetEntries();
  // start *= 4;
  // end *= 4;
  cout << "Start: " << start << ", End: " << end - 1 << ", SaveName: "  << endl;
  int k = 0;
  wavedec* wd = new wavedec();
  // for (int i = 0; i < tr->GetEntries(); i++) {
  for (int i = start; i < end; i++) {
    tr->GetEntry(i);
	//cout<<tr->GetEntries()<<endl;
    if (hmmtpmt[id] == true) {
      // wd->set_input(w_tmp, h_filter0, spere[id], speim[id]);
      wd->set_input(w_tmp, h_filter2, spere[id], speim[id]);
      wd->transform();
      rf = wd->get_rawfmag();
      ff = wd->get_filteredfmag();
      ft = wd->get_filteredt();
      df = wd->get_dividedfmag();
      w = wd->get_dividedt();
      spe2raw = (TH1D*)h_filter2->Clone();
      spe2raw->Divide(w_tmp);
      spe2raw->SetNameTitle("spe2raw", "spe2raw");
      k = 0;
      for (int j = 1150; j < 1250; j++) arr[k++] = w->GetBinContent(j + 1);
      for (int j = 0; j < 1150; j++) {
        arr[k++] = w->GetBinContent(j + 1);
        w->SetBinContent(j + 1, arr[j]);
      }
      for (int j = 1150; j < 1250; j++) w->SetBinContent(j + 1, arr[j]);
    } else {
      // wd->set_input(w_tmp, h_filter0, spere[id], speim[id]);
      wd->set_input(w_tmp, h_filter1, spere[id], speim[id]);
      wd->transform();
      rf = wd->get_rawfmag();
      ff = wd->get_filteredfmag();
      ft = wd->get_filteredt();
      df = wd->get_dividedfmag();
      w = wd->get_dividedt();
      spe2raw = (TH1D*)h_filter1->Clone();
      spe2raw->Divide(w_tmp);
      spe2raw->SetNameTitle("spe2raw", "spe2raw");
      k = 0;
      for (int j = 1150; j < 1250; j++) arr[k++] = w->GetBinContent(j + 1);
      for (int j = 0; j < 1150; j++) {
        arr[k++] = w->GetBinContent(j + 1);
        w->SetBinContent(j + 1, arr[j]);
      }
      for (int j = 1150; j < 1250; j++) w->SetBinContent(j + 1, arr[j]);
    }
    w->SetNameTitle(w_tmp->GetName(), w_tmp->GetTitle());
    str->Fill();
    if (i % 1000 == 0) cout << i << " waveforms finished!" << endl;
  }
  sf->cd();
  str->Write();
}
