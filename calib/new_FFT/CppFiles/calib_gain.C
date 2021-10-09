#include "/junofs/users/jiangw/include/myincludes.h"
void calib_gain()
{
  int npmt = 17612;
  gStyle->SetOptStat(0);  // number of entries only
  const int nn = 7;
  TString path0="../";
  TString fname[nn] = {"AmC", "Co60", "Cs137", "Ge68", "Laser0.1", "Laser0.05", "Averaged"};
  TString fname2[nn] = {"AmC", "^{60}Co", "^{137}Cs", "^{68}Ge", "#mu = 0.1", "#mu = 0.05", "Averaged"};
  /*
  const int nn = 4;
  TString path0="../";
  TString fname[nn] = {"C14","Laser0.1", "Laser0.05", "Averaged"};
  TString fname2[nn] = {"C14", "#mu = 0.1", "#mu = 0.05", "Averaged"};
  */
  TFile* f[nn - 1];
  TFile* fc[nn - 1];

  TH1D* gainth[nn];
  TH1D* gainpdfth[nn];
  TH1D* gainerrth[nn];

  for (int i = 0; i < nn - 1; i++) {
    f[i] = new TFile(path0 + fname[i] + "/step2/user-root/" + "outf.root", "read");
    gainth[i] = (TH1D*)f[i]->Get("gainth");
    gainpdfth[i] = new TH1D("", "Gain Distribution", 400, 0.8, 1.2);
    gainpdfth[i]->GetXaxis()->SetTitle("Gain");
    gainerrth[i] = new TH1D("", "Gain Stat. Error", 100, 0, 0.04);
    gainerrth[i]->GetXaxis()->SetTitle("Gain Error");
    for (int j = 0; j < npmt; j++) {
      gainpdfth[i]->Fill(gainth[i]->GetBinContent(j + 1));
      gainerrth[i]->Fill(gainth[i]->GetBinError(j + 1));
    }
  }

  vector<double> g;
  vector<double> ge;
  pair<double, double> gres;
  gainth[nn - 1] = new TH1D("", "Gain", npmt, 0, npmt);
  gainth[nn - 1]->GetXaxis()->SetTitle("PMTID");
  gainth[nn - 1]->GetYaxis()->SetTitle("Gain");
  gainpdfth[nn - 1] = new TH1D("", "Gain Distribution", 400, 0.8, 1.2);
  gainpdfth[nn - 1]->GetXaxis()->SetTitle("Gain");
  gainerrth[nn - 1] = new TH1D("", "Gain Stat. Error", 100, 0, 0.04);
  gainerrth[nn - 1]->GetXaxis()->SetTitle("Gain Error");

  pair<double, double> dnres;
  for (int i = 0; i < npmt; i++) {
    // for (int i = 0; i < 176; i++) {
    g.clear();
    ge.clear();
    for (int j = 0; j < nn - 1; j++) {
      g.push_back(gainth[j]->GetBinContent(i + 1));
      ge.push_back(gainth[j]->GetBinError(i + 1));
    }
    gres = weightedMean(g, ge);
    gainth[nn - 1]->SetBinContent(i + 1, gres.first);
    gainth[nn - 1]->SetBinError(i + 1, gres.second);
    gainpdfth[nn - 1]->Fill(gres.first);
    gainerrth[nn - 1]->Fill(gainth[nn - 1]->GetBinError(i + 1));
  }
  TList* gainpdfls = new TList();
  for (int i = 0; i < nn; i++) gainpdfls->Add(gainpdfth[i]);
  TCanvas* c = new TCanvas("c", "c", 800, 600);
  compare(c->cd(), gainpdfls, nn, fname2, 1);
  c->Print("GainPDF.png");

  TList* gainls = new TList();
  for (int i = 0; i < nn; i++) gainls->Add(gainth[i]);
  TCanvas* c1_0 = new TCanvas("gainth", "Gain", 800, 600);
  compare(c1_0->cd(), gainls, nn, fname2, 0, "p");
  c1_0->Print("Gain.png");

  TList* gainerrls = new TList();
  for (int i = 0; i < nn; i++) gainerrls->Add(gainerrth[i]);
  TCanvas* c1_2 = new TCanvas("gainerrth", "Gain Error", 800, 600);
  compare(c1_2->cd(), gainerrls, nn, fname2, 1);
  c1_2->Print("GainError.png");

  float timeoffset[20000]={0};
  ifstream tfs_f("timeOffset_sub_roofit.txt");
  for (int i = 0; i < npmt; i++)    tfs_f >> timeoffset[i];
  tfs_f.close();

  ofstream outf("CalibPars.txt");
  for (int i = 0; i < npmt; i++) outf << i << "\t0\t" << gainth[nn - 1]->GetBinContent(i + 1) << "\t"<< timeoffset[i] <<"\t0\t0" << endl;
}
