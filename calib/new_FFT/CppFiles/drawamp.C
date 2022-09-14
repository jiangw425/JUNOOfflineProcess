#include "/junofs/users/jiangw/include/myincludes.h"
TF1* tf = new TF1("tf", "gaus(0)", -0.05, 0.05);
TH1D* blamp = new TH1D("blamp", "blamp", 100, -0.05, 0.05);
double m_threshold = 0.025;
//double m_threshold = 0.03;
double maxv = 0;
double baseline = 0;
double rms = 0;
int getHit(double AC[], int HitBTime[1000], int HitETime[1000]);
int hitAlg(TH1D* h, int HitBTime[1000], int HitETime[1000]);
int intew = 9;
const int adcl = ADCL;
void drawamp()
{
  gStyle->SetOptStat(0);
  bool* hmmtpmt = pmtflag();
  TString nameflag = "_tmp";
  // TFile* f0 = new TFile("waves_trans_Ge68.root", "read");
  // TFile* f0 = new TFile("newfilter/waves_trans_Ge68_newfilter1.root", "read");
  TFile* f0 = new TFile("waves_trans.root", "read");
  // TFile* f0 = new TFile("newfilter/waves_trans_Ge68_newfilter4.root", "read");
  TTree* t0 = (TTree*)f0->Get("waves_trans");
  TH1D* dividedt = 0;
  int m_length = adcl;
  int id = 0;
  int k0 = 0;
  int k1 = 50;
  const int kd = k1 - k0 + 1;
  t0->SetBranchAddress("dividedt", &dividedt);
  t0->SetBranchAddress("pmtid", &id);
  cout << "Total Events: " << t0->GetEntries() << endl;

  TH1D* amph = new TH1D("amph", "Amp Distribution", 500, 0, 0.5);
  TH1D* ampn = new TH1D("ampn", "Amp Distribution", 500, 0, 0.5);
  TH1D* rmsh = new TH1D("rmsh", "Rms Distribution", 500, 0, 0.1);
  TH1D* rmsn = new TH1D("rmsn", "Rms Distribution", 500, 0, 0.1);
  TH1D* wh = new TH1D("wh", "Width Distribution", 50, 0, 50);
  TH1D* wn = new TH1D("wn", "Width Distribution", 50, 0, 50);
  int HitBTime[1000];
  int HitETime[1000];
  int count = 0;
  int width = 0;

  TH2D* whvsq[kd];
  TH2D* wnvsq[kd];
  TString wqhname[kd];
  TString wqnname[kd];
  TString wqtname[kd];

  TH2D* itvsqh = new TH2D("itvsqh", "Extended Integral Width vs. Charge", kd, k0 - 0.5, k1 + 0.5, 400, -1, 3);
  TH2D* itvsqn = new TH2D("itvsqn", "Extended Integral Width vs. Charge", kd, k0 - 0.5, k1 + 0.5, 400, -1, 3);
  for (int k = k0; k <= k1; k++) {
    wqhname[k - k0] = Form("whvsq%d", k - k0);
    wqnname[k - k0] = Form("wnvsq%d", k - k0);
    wqtname[k - k0] = Form("Width vs. Charge Distribution, %d ns", k);
    whvsq[k - k0] = new TH2D(wqhname[k - k0], wqtname[k - k0], 100, 0, 100, 400, -1, 3);
    wnvsq[k - k0] = new TH2D(wqnname[k - k0], wqtname[k - k0], 100, 0, 100, 400, -1, 3);
    whvsq[k - k0]->GetXaxis()->SetTitle("Width");
    whvsq[k - k0]->GetYaxis()->SetTitle("Charge");
    wnvsq[k - k0]->GetXaxis()->SetTitle("Width");
    wnvsq[k - k0]->GetYaxis()->SetTitle("Charge");
  }

  TH1D* myh = new TH1D("myh", "Y Mean vs. Integral Width", k1 - k0 + 1, k0 - 0.5, k1 + 0.5);
  TH1D* myn = new TH1D("myn", "Y Mean vs. Integral Width", k1 - k0 + 1, k0 - 0.5, k1 + 0.5);

  for (int ent = 0; ent < t0->GetEntries(); ent++) {
    // for (int ent = 0; ent < 1000; ent++) {
    t0->GetEntry(ent);
    count = hitAlg(dividedt, HitBTime, HitETime);

    for (int i = 0; i < count; i++) {
      width = HitETime[i] - HitBTime[i] + 1;
      for (int k = k0; k <= k1; k++) {

        intew = k;
        int start = HitBTime[i] - intew >= 0 ? HitBTime[i] - intew : 0;
        if (i > 0) start = HitETime[i - 1] + intew <= start ? start : HitBTime[i] - (HitBTime[i] - HitETime[i - 1] - 1) / 2;
        int end = HitETime[i] + intew >= m_length ? m_length - 1 : HitETime[i] + intew;
        if (i < count - 1) end = HitBTime[i + 1] - intew <= end ? HitETime[i] + (HitBTime[i + 1] - HitETime[i] - 1) / 2 : end;

        double inte = 0;
        for (int j = start; j <= end; j++) {
          inte += dividedt->GetBinContent(j) - baseline;
        }

        if (hmmtpmt[id] == true) {
          whvsq[k - k0]->Fill(width, inte);
          itvsqh->Fill(k - k0, inte);
        } else {
          wnvsq[k - k0]->Fill(width, inte);
          itvsqn->Fill(k - k0, inte);
        }
      }
      if (hmmtpmt[id] == true) {
        wh->Fill(width);
      } else {
        wn->Fill(width);
      }
    }
    if (hmmtpmt[id] == true) {
      amph->Fill(maxv);
      rmsh->Fill(rms);
    } else {
      ampn->Fill(maxv);
      rmsn->Fill(rms);
    }
    if (ent % 500 == 0) cout << ent << " waveforms finished!" << endl;
  }
  for (int k = k0; k <= k1; k++) {
    myh->SetBinContent(k - k0 + 1, whvsq[k - k0]->GetMean(2));
    myn->SetBinContent(k - k0 + 1, wnvsq[k - k0]->GetMean(2));
  }

  //*************************************************************
	TF1 * f1h = new TF1("f1h","gaus");
	TF1 * f1n = new TF1("f1n","gaus");
	TF1 * f2h = new TF1("f2h","gaus");
	TF1 * f2n = new TF1("f2n","gaus");
  f1h->SetParameters(amph->GetBinContent(amph->GetMaximumBin()),amph->GetMean(),amph->GetRMS());
	f1n->SetParameters(ampn->GetBinContent(ampn->GetMaximumBin()),ampn->GetMean(),ampn->GetRMS());
	f2h->SetParameters(rmsh->GetBinContent(rmsh->GetMaximumBin()),rmsh->GetMean(),rmsh->GetRMS());
	f2n->SetParameters(rmsn->GetBinContent(rmsn->GetMaximumBin()),rmsn->GetMean(),rmsn->GetRMS());
  amph->Fit("f1h");
	ampn->Fit("f1n");
	rmsh->Fit("f2h");
	rmsn->Fit("f2n");

	double rara[12];
	f1h->GetParameters(&rara[0]);
	f1n->GetParameters(&rara[3]);
	f2h->GetParameters(&rara[6]);
	f2n->GetParameters(&rara[9]);
	
	cout << "xinzaobi::::::::::hmmt: "<< rara[1]/rara[7] <<"\tnnvt:" << rara[4]/rara[10] <<endl;
	cout << "hmmt:asdasdasda "<< rara[1]<<"\t"<<rara[4] <<"\t" << rara[7]<<"\t"<<rara[10] <<endl;
 
 //************************************************************

  TCanvas* amprmsc = new TCanvas("amprmsc", "amprmsc", 1920, 1080);
  amprmsc->Divide(2, 2);

  TList* ampls = new TList();
  ampls->Add(amph);
  ampls->Add(ampn);
  TString ampname[2] = {"hmmt", "nnvt"};
  compare(amprmsc->cd(1), ampls, 2, ampname, 0);

  TList* rmsls = new TList();
  rmsls->Add(rmsh);
  rmsls->Add(rmsn);
  TString rmsname[2] = {"hmmt", "nnvt"};
  compare(amprmsc->cd(2), rmsls, 2, rmsname, 1);

  TList* wls = new TList();
  wls->Add(wh);
  wls->Add(wn);
  TString wname[2] = {"hmmt", "nnvt"};
  compare(amprmsc->cd(3), wls, 2, wname, 1);

  TList* mywls = new TList();
  mywls->Add(myh);
  mywls->Add(myn);
  TString mywname[2] = {"hmmt", "nnvt"};
  compare(amprmsc->cd(4), mywls, 2, mywname, 0);

  amprmsc->Print("amprmshn" + nameflag + ".png");

  // TCanvas* wvsqc = new TCanvas("wvsqc", "wvsqc", 1600, 600);
  // wvsqc->Divide(2, 1);
  // wvsqc->Print("figwvsq" + nameflag + ".pdf[");
  // for (int k = k0; k <= k1; k++) {
  //   wvsqc->cd(1);
  //   wvsqc->cd(1)->SetLogz();
  //   whvsq[k - k0]->Draw("colz");
  //   wvsqc->cd(2);
  //   wvsqc->cd(2)->SetLogz();
  //   wnvsq[k - k0]->Draw("colz");

    // wvsqc->Print("figwvsq" + nameflag + ".pdf");
  // }
  // wvsqc->Print("figwvsq" + nameflag + ".pdf]");

  TCanvas* itvsqc = new TCanvas("itvsqc", "itvsqc", 800, 600);
  itvsqc->cd();

  itvsqh->GetYaxis()->SetRangeUser(0.4, 1.6);
  itvsqh->GetXaxis()->SetTitle("Extended Width");
  itvsqh->GetYaxis()->SetTitle("Charge");
  itvsqh->Draw("colz");
  myh->SetLineColor(1);
  myh->Draw("same");
  itvsqc->Print("itvsqh" + nameflag + ".png");

  itvsqn->GetYaxis()->SetRangeUser(0.4, 1.6);
  itvsqn->GetXaxis()->SetTitle("Extended Width");
  itvsqn->GetYaxis()->SetTitle("Charge");
  itvsqn->Draw("colz");
  myn->SetLineColor(1);
  myn->Draw("same");
  itvsqc->Print("itvsqn" + nameflag + ".png");

  TFile* savef = new TFile("res" + nameflag + ".root", "recreate");
  savef->cd();
  amph->Write();
  ampn->Write();
  rmsh->Write();
  rmsn->Write();
  wh->Write();
  wn->Write();
  // for (int k = k0; k <= k1; k++) {
  // whvsq[k - k0]->Write();
  // wnvsq[k - k0]->Write();
  //}
  myh->Write();
  myn->Write();
  itvsqh->Write();
  itvsqn->Write();
}

int getHit(double AC[], int HitBTime[1000], int HitETime[1000])
{
  int m_length = adcl;
  // find over-threshold wave. threshold = 0.6, 0.65
  int Pass = 0, HitCount = 0;
  for (int i = 0; i < m_length; i++) {
    // find beginning point
    if (Pass == 0 && AC[i] > m_threshold) {
      for (int tt = i; tt > 0; tt--) {
        if (AC[tt] < 0) {
          HitBTime[HitCount] = tt;
          break;
        }
      }
      Pass = 1;
      continue;
    }
    if (Pass == 1) {
      if (AC[i] <= 0) {
        Pass = 0;
        HitETime[HitCount] = i;
        if (HitETime[HitCount] - HitBTime[HitCount] > 6)
          HitCount++;
        else {
          HitBTime[HitCount] = 0;
          HitETime[HitCount] = 0;
        }
      }
    }
    if (HitCount > 990) break;
  }
  return HitCount;
}

int hitAlg(TH1D* h, int HitBTime[1000], int HitETime[1000])
{
  const int m_length = adcl;
  double AC[m_length];

  blamp->Reset();
  for (int i = 0; i < m_length; i++) {
    AC[i] = h->GetBinContent(i + 1);
    blamp->Fill(AC[i]);
  }
  blamp->Fit(tf, "RLQ");
  baseline = tf->GetParameter(1);
  rms = tf->GetParameter(2);

  maxv = -10000.;
  for (int i = 0; i < m_length; i++) {
    AC[i] -= baseline;
    if (AC[i] > maxv) maxv = AC[i];
  }

  for (int i = 0; i < 1000; i++) {
    HitBTime[i] = 0;
    HitETime[i] = 0;
  }

  int HitCount = getHit(AC, HitBTime, HitETime);

  return HitCount;
}
