#include "INCLUDE"
double timeinterval = DNLEN.e-9;
void calib_dn()
{
  int npmt = 17612;
  gStyle->SetOptStat(0);  // number of entries only
  // TString fname[nn] = {"C14", "AmC", "Co60", "Cs137", "Ge68", "photon_22000", "photon_2200", "Averaged"};
  // TString fname2[nn] = {"C14", "AmC", "^{60}Co", "^{137}Cs", "^{68}Ge", "#mu = 0.1", "#mu = 0.01", "Averaged"};
  // TString fname[nn] = {"AmC", "Co60", "Cs137", "Ge68", "Laser0.1", "Laser0.05", "Averaged"};
  // TString fname2[nn] = {"AmC", "^{60}Co", "^{137}Cs", "^{68}Ge", "#mu = 0.1", "#mu = 0.05", "Averaged"};
  TString fname = "SCSSN";
  TFile* f;
  TFile* fc;

  TH1F* darkrateth;
  TH1F* twc;
  double tw;

  TString path0 = "../";
  f = new TFile(path0 + fname +"/step12/user-root/" + "outf.root", "read");
  darkrateth = (TH1F*)f->Get("darkCharge");
  fc = new TFile(path0 + fname +"/step12/user-root/" + "user_calibCorr.root", "read");
  twc = (TH1F*)fc->Get("totalWaveCount");
  tw = twc->GetBinContent(1);
  TH1F* spe;
  double spem[17612];
  for (int i = 0; i < 17612; i++) {
    TString spename = Form("ch%d_charge_spec", i);
    spe = (TH1F*)fc->Get(spename);
    spem[i] = spe->GetMean();
  }
  cout << spem[0] << endl;

  // vector<double> dn;
  // vector<double> dne;
  // pair<double, double> dnres;
  TH1F *darkrateth2 = new TH1F("", "darkrate", npmt, 0, npmt);
  darkrateth2->GetXaxis()->SetTitle("PMTID");
  darkrateth2->GetYaxis()->SetTitle("Dark Noise Rate");
  for (int i = 0; i < npmt; i++) {
    darkrateth2->SetBinContent(i + 1, darkrateth->GetBinContent(i + 1) / spem[i]);
    darkrateth2->SetBinError(i + 1, sqrt(darkrateth->GetBinContent(i + 1) / spem[i] / tw / timeinterval));
    // cout << dn.at(0) << ", " << dn.at(1) << ", " << dn.at(2) << ", " << dn.at(3) << ", " << darkrateth[nn - 1]->GetBinContent(i + 1) << endl;
  }

  // TList* dnls = new TList();
  // for (int i = 0; i < nn; i++) dnls->Add(darkrateth[i]);
  // TCanvas* c2_0 = new TCanvas("darkrateth", "Dark Noise Rate", 800, 600);
  // compare(c2_0->cd(), dnls, nn, fname2, 0, "p");
  // c2_0->Print("DarkNoiseRate.png");

  ofstream outf("CalibPars_DN.txt");
  for (int i = 0; i < npmt; i++) outf << i << "\t" << darkrateth2->GetBinContent(i + 1) << endl;
}
