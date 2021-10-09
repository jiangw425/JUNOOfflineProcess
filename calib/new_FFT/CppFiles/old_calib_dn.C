#include "/junofs/users/jiangw/include/myincludes.h"
double timeinterval = DNLEN.e-9;
void calib_dn()
{
  int npmt = 17612;
  gStyle->SetOptStat(0);  // number of entries only
  const int nn = 4;
  // TString fname[nn] = {"C14", "AmC", "Co60", "Cs137", "Ge68", "photon_22000", "photon_2200", "Averaged"};
  // TString fname2[nn] = {"C14", "AmC", "^{60}Co", "^{137}Cs", "^{68}Ge", "#mu = 0.1", "#mu = 0.01", "Averaged"};
  // TString fname[nn] = {"AmC", "Co60", "Cs137", "Ge68", "Laser0.1", "Laser0.05", "Averaged"};
  // TString fname2[nn] = {"AmC", "^{60}Co", "^{137}Cs", "^{68}Ge", "#mu = 0.1", "#mu = 0.05", "Averaged"};
  TString fname[nn] = {"C14", "Laser0.1", "Laser0.05", "Averaged"};
  TString fname2[nn] = {"^{14}C", "#mu = 0.1", "#mu = 0.05", "Averaged"};
  TFile* f[nn - 1];
  TFile* fc[nn - 1];

  TH1F* darkrateth[nn];
  TH1F* twc[nn - 1];
  double tw[nn - 1];

  TString path0 = "../";
  for (int i = 0; i < nn - 1; i++) {
    f[i] = new TFile(path0 + fname[i]+"/step12/user-root/" + "outf.root", "read");
    darkrateth[i] = (TH1F*)f[i]->Get("darkCharge");
  }
  for (int i = 0; i < nn - 1; i++) {
    fc[i] = new TFile(path0 + fname[i]+"/step12/user-root/" + "user_calibCorr.root", "read");
    twc[i] = (TH1F*)fc[i]->Get("totalWaveCount");
    tw[i] = twc[i]->GetBinContent(1);
  }
  TH1F* spe;
  double spem[17612];
  for (int i = 0; i < 17612; i++) {
    TString spename = Form("ch%d_charge_spec", i);
    spe = (TH1F*)fc[0]->Get(spename);
    spem[i] = spe->GetMean();
  }
  cout << spem[0] << endl;

  vector<double> dn;
  vector<double> dne;
  pair<double, double> dnres;
  darkrateth[nn - 1] = new TH1F("", "darkrate", npmt, 0, npmt);
  darkrateth[nn - 1]->GetXaxis()->SetTitle("PMTID");
  darkrateth[nn - 1]->GetYaxis()->SetTitle("Dark Noise Rate");
  for (int i = 0; i < npmt; i++) {
    // for (int i = 0; i < 176; i++) {
    dn.clear();
    dne.clear();
    for (int j = 0; j < nn - 1; j++) {
      dn.push_back(darkrateth[j]->GetBinContent(i + 1) / spem[i]);
      dne.push_back(sqrt(darkrateth[j]->GetBinContent(i + 1) / spem[i] / tw[j] / timeinterval));
    }
    dnres = weightedMean(dn, dne);
    darkrateth[nn - 1]->SetBinContent(i + 1, dnres.first);
    darkrateth[nn - 1]->SetBinError(i + 1, dnres.second);
    // cout << dn.at(0) << ", " << dn.at(1) << ", " << dn.at(2) << ", " << dn.at(3) << ", " << darkrateth[nn - 1]->GetBinContent(i + 1) << endl;
  }

  TList* dnls = new TList();
  for (int i = 0; i < nn; i++) dnls->Add(darkrateth[i]);
  TCanvas* c2_0 = new TCanvas("darkrateth", "Dark Noise Rate", 800, 600);
  compare(c2_0->cd(), dnls, nn, fname2, 0, "p");
  c2_0->Print("DarkNoiseRate.png");

  ofstream outf("CalibPars_DN.txt");
  for (int i = 0; i < npmt; i++) outf << i << "\t" << darkrateth[nn - 1]->GetBinContent(i + 1) << endl;
}
