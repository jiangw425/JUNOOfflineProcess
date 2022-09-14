#include "INCLUDE"
TF1* tf = new TF1("tf", "gaus(0)", -0.05, 0.05);
TH1D* blamp = new TH1D("blamp", "blamp", 100, -0.05, 0.05);
double m_threshold = 0.025;
double maxv = 0;
double baseline = 0;
double rms = 0;
int getHit(double AC[], int HitBTime[1000], int HitETime[1000]);
int hitAlg(TH1D* h, int HitBTime[1000], int HitETime[1000]);
int intewjw[2] = {NNVTINTEW,HMMTINTEW};
int widthT[2] = {NNVTWIDTH,HMMTWIDTH};
void drawamp2()
{
  gStyle->SetOptStat(0);
  bool* hmmtpmt = pmtflag();
  TString nameflag = "J21v1";
  // TFile* f0 = new TFile("waves_trans_Ge68.root", "read");
  // TFile* f0 = new TFile("newfilter/waves_trans_Ge68_newfilter1.root", "read");
  TFile* f0 = new TFile("waves_trans.root", "read");
  TTree* t0 = (TTree*)f0->Get("waves_trans");
  TH1D* dividedt = 0;
  int m_length = 1250;
  int id = 0;
  int k0 = 15;
  int k1 = 50;
  const int kd = k1 - k0 + 1;
  t0->SetBranchAddress("dividedt", &dividedt);
  t0->SetBranchAddress("pmtid", &id);
  cout << "Total Events: " << t0->GetEntries() << endl;

  int HitBTime[1000];
  int HitETime[1000];
  int count = 0;
  int width = 0;

  //double rate[2][kd];
  int jw[2][2][kd];//nnvt hmmt, noise, signal, kd
  TH2D* whvsq[kd];
  TH2D* wnvsq[kd];
  TString wqhname[kd];
  TString wqnname[kd];
  TString wqhtname[kd];
  TString wqntname[kd];
memset(jw, 0, 2 * 2 * kd * sizeof(int));

  for (int k = k0; k <= k1; k++) {
    wqhname[k - k0] = Form("whvsq%d", k - k0);
    wqnname[k - k0] = Form("wnvsq%d", k - k0);
    wqhtname[k - k0] = Form("Width vs. Charge Distribution, Threshold = %f, Dynode", k / 1000.);
    wqntname[k - k0] = Form("Width vs. Charge Distribution, Threshold = %f, MCP", k / 1000.);
    whvsq[k - k0] = new TH2D(wqhname[k - k0], wqhtname[k - k0], 100, 0, 100, 400, -1, 3);
    wnvsq[k - k0] = new TH2D(wqnname[k - k0], wqntname[k - k0], 100, 0, 100, 400, -1, 3);
    whvsq[k - k0]->GetXaxis()->SetTitle("Width");
    whvsq[k - k0]->GetYaxis()->SetTitle("Charge");
    wnvsq[k - k0]->GetXaxis()->SetTitle("Width");
    wnvsq[k - k0]->GetYaxis()->SetTitle("Charge");
  }

  for (int ent = 0; ent < t0->GetEntries(); ent++) {
//    for (int ent = 0; ent < 1000; ent++) {
    t0->GetEntry(ent);
    for (int k = k0; k <= k1; k++) {

      m_threshold = k / 1000.;
      count = hitAlg(dividedt, HitBTime, HitETime);

      for (int i = 0; i < count; i++) {
	      double intew = intewjw[hmmtpmt[id]];
        width = HitETime[i] - HitBTime[i] + 1;

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
	        if(width>widthT[1]) jw[1][0][k-k0]++; else jw[1][1][k-k0]++;
        } else {
          wnvsq[k - k0]->Fill(width, inte);
        	if(width>widthT[0]) jw[0][0][k-k0]++; else jw[0][1][k-k0]++;
        }
      }
    }
    if (ent % 500 == 0) cout << ent << " waveforms finished!" << endl;
  }
	double xjw[kd];
	for(int j=0;j<kd;j++)
        {
                xjw[j] = (k0+j)/1000.;
        }	
	/*std::string type[2] = {"nnvt","hmmt"};
	double xjw[kd];
	double yjwh[kd], yjwn[kd];
	memset(xjw, 0, sizeof(xjw));
	memset(yjwh, 0,sizeof(xjw));
	memset(yjwn, 0,sizeof(xjw));
	for(int j=0;j<kd;j++)
        {
                xjw[j] = (k0+j)/1000.;
        }
	//TGraph *hmmtrate;
	//TGraph *nnvtrate;
	for(int i=0;i<2;++i)
	for(int j=0;j<kd;j++)
	{
		//rate[i][j] = jw[i][1][j]/jw[i][0][j]; 
		if(jw[1][0][j]==0){
			jw[1][0][j]=1;
		}
		if(i==0) yjwn[j]= jw[i][1][j]/jw[i][0][j];
		else yjwh[j] = jw[i][1][j]/jw[i][0][j];
	}*/
	ofstream ojw("Amp_threshold_rate.txt");
	for(int j=0;j<kd;j++)
  {
    //ojw << xjw[j] <<" "<< yjwh[j]<<" "<<yjwn[j]<<endl;
    ojw<<xjw[j]<<" "<<jw[0][0][j]<<" "<<jw[0][1][j]<<" "<<jw[1][0][j]<<" "<<jw[1][1][j]<<endl;
  }
	ojw.close();
	//TGraph *hmmtrate = new TGraph(kd,xjw,yjwh);
	//hmmtrate->SetName("hmmt");
        //TGraph *nnvtrate = new TGraph(kd,xjw,yjwn);
	//nnvtrate->SetName("nnvt");

  // TCanvas* wvsqc = new TCanvas("wvsqc", "wvsqc", 800, 1200);
  // wvsqc->Divide(1, 2);
  // wvsqc->Print("figwvsq" + nameflag + ".pdf[");
  // for (int k = k0; k <= k1; k++) {
  // wvsqc->cd(1);
  // wvsqc->cd(1)->SetLogz();
  // whvsq[k - k0]->Draw("colz");
  // wvsqc->cd(2);
  // wvsqc->cd(2)->SetLogz();
  // wnvsq[k - k0]->Draw("colz");

  // wvsqc->Print("figwvsq" + nameflag + ".pdf");
  // }
  // wvsqc->Print("figwvsq" + nameflag + ".pdf]");
  

  TCanvas* wvsqc = new TCanvas("wvsqc", "wvsqc", 800, 800);
  for (int k = k0; k <= k1; k++) {
    wvsqc->cd()->SetLogz();
    wnvsq[k - k0]->Draw("colz");
    if (k < k1)
      wvsqc->Print("figwvsq" + nameflag + ".gif+20");
    else
      wvsqc->Print("figwvsq" + nameflag + ".gif++20++");
  }

  TFile* whvsqsf = new TFile("whvsqsf" + nameflag + ".root", "recreate");
  whvsqsf->cd();
  for (int k = k0; k <= k1; k++) {
    whvsq[k - k0]->Write();
    wnvsq[k - k0]->Write();
//	hmmtrate->Write();
//	nnvtrate->Write();
  }
}

int getHit(double AC[], int HitBTime[1000], int HitETime[1000])
{
  int m_length = 1250;
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
  const int m_length = 1250;
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
