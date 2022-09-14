#include "INCLUDE"
const int adcl = ADCL;
const int fRange = int(adcl/2);
void getFilter()
{
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);
  bool* hmmtpmt = pmtflag();
  TH2D* rawh2D=new TH2D("rawh2D", "Raw Distribution of HAM", fRange, 0, fRange, 5000, 0, 5000);
  TH2D* rawn2D=new TH2D("rawn2D", "Raw Distribution of MCP", fRange, 0, fRange, 5000, 0, 5000);
  const int fnum = FIUM;
  for(int i=0;i<fnum;++i){
    TFile* spef = new TFile(Form("spef-%d.root",i), "read");
    cout << i << "\t" << rawn2D->GetEntries() << " and " << rawh2D->GetEntries() << endl;
    rawn2D->Add((TH2D*)spef->Get("rawn2D"));
    rawh2D->Add((TH2D*)spef->Get("rawh2D"));
    spef->Close();
  }
  cout << "read done." << rawn2D->GetEntries() << " and " << rawh2D->GetEntries() << endl;
  // TFile* spef = new TFile("spef.root", "read");
  // TH2D* rawh2D = (TH2D*)spef->Get("rawh2D");
  // TH2D* rawn2D = (TH2D*)spef->Get("rawn2D");
  TH1D* projh[fRange];
  TH1D* projn[fRange];
  TH1D* meanh = new TH1D("meanh", "meanh", fRange, 0, fRange);
  TH1D* meann = new TH1D("meann", "meann", fRange, 0, fRange);
  TString name;
  for (int i = 0; i < fRange; i++) {
    name = "projyh_";
    name += i;
    projh[i] = rawh2D->ProjectionY(name, i + 1, i + 1, "");
    name = "projyn_";
    name += i;
    projn[i] = rawn2D->ProjectionY(name, i + 1, i + 1, "");
    projh[i]->Smooth(3);
    projn[i]->Smooth(3);
    meanh->SetBinContent(i + 1, pow(projh[i]->GetBinCenter(projh[i]->GetMaximumBin()), 2));
    meann->SetBinContent(i + 1, pow(projn[i]->GetBinCenter(projn[i]->GetMaximumBin()), 2));
  }
  double noise_baseline_h=0;
  double noise_baseline_n=0;
  const int noise_start = 300;
  for(int i=noise_start;i<fRange;i++ )
  {
    noise_baseline_h+=meanh->GetBinContent(i+1);
    noise_baseline_n+=meann->GetBinContent(i+1);
  }
  noise_baseline_h/=double(fRange-noise_start);
  noise_baseline_n/=double(fRange-noise_start);

  TH1D* backh = (TH1D*)meanh->Clone();
  TH1D* backn = (TH1D*)meann->Clone();

  for (int i = 0; i < fRange; i++) {
    //meanh->AddBinContent(i + 1, -200 * 200);
    //meann->AddBinContent(i + 1, -200 * 200);
    meanh->AddBinContent(i + 1, -noise_baseline_h );
    meann->AddBinContent(i + 1, -noise_baseline_n );
  }
  cout<< "noise_baseline_h: "<<noise_baseline_h<<endl;
  cout<< "noise_baseline_n: "<<noise_baseline_n<<endl;

  meanh->Divide(backh);
  meann->Divide(backn);
  if(meanh->GetBinContent(1)<0)meanh->SetBinContent(1,1);
  if(meann->GetBinContent(1)<0)meann->SetBinContent(1,1);
  // meanh->Draw();
  // meann->Draw("same");

  for( int i=150;i<meanh->GetNbinsX();i++)
  {
	meann->SetBinContent(i+1, 0);
  }

  // smooth
  TGraph* tfh = new TGraph();
  TGraph* tfn = new TGraph();
  for (int i = 1; i < fRange; i++) {
    tfh->SetPoint(i - 1, meanh->GetBinCenter(i + 1), meanh->GetBinContent(i + 1));
    tfn->SetPoint(i - 1, meann->GetBinCenter(i + 1), meann->GetBinContent(i + 1));
  }
  TGraphSmooth* gsh = new TGraphSmooth("normal");
  TGraph* grouth = gsh->SmoothSuper(tfh, "", 3);
  // TGraph* grouth = gsh->SmoothLowess(tfh, "", 0.2);
  TGraphSmooth* gsn = new TGraphSmooth("normal");
  TGraph* groutn = gsn->SmoothSuper(tfn, "", 3);
  // TGraph* groutn = gsn->SmoothLowess(tfn, "", 0.2);
  // grouth->Draw();
  // groutn->Draw("same");

  TH1D* fh0 = new TH1D("fh0", "fh0", adcl, 0, adcl);
  TH1D* fn0 = new TH1D("fn0", "fn0", adcl, 0, adcl);
  double xx, yy;
  fh0->SetBinContent(1, 0);
  fn0->SetBinContent(1, 0);
  int ih = 0;
  int in = 0;
  for (int i = 1; i < adcl; i++) {
    grouth->GetPoint(i - 1, xx, yy);
    if (yy < 0) {
      ih = i;
      break;
    }
    fh0->SetBinContent(i + 1, yy);
  }
  for (int i = 1; i < adcl; i++) {
    groutn->GetPoint(i - 1, xx, yy);
    if (yy < 0) {
      in = i;
      break;
    }
    fn0->SetBinContent(i + 1, yy);
  }
  for (int i = 0; i < adcl; i++) {
    fh0->SetBinContent(i + 1, (fh0->GetBinContent(i + 1)));
    fn0->SetBinContent(i + 1, (fn0->GetBinContent(i + 1)));
  }

  TCanvas* ch = new TCanvas("hc", "hc", 800, 600);
  TCanvas* cn = new TCanvas("nc", "nc", 800, 600);
  TList* lsh = new TList();
  TList* lsn = new TList();
  TString cname[2] = {"Av_f", "Filter"};
  lsh->Add(meanh);
  lsh->Add(fh0);
  lsn->Add(meann);
  lsn->Add(fn0);
  compare(ch->cd(), lsh, 2, cname, 0);
  compare(cn->cd(), lsn, 2, cname, 0);

  TFile* savef = new TFile("filter.root", "recreate");
  savef->cd();
  rawh2D->Write();
  rawn2D->Write();
  meanh->Write();
  meann->Write();
  fh0->Write();
  fn0->Write();
  //for(int i=450; i<600; ++i)
  //{
	//projh[i]->Write();
	//projn[i]->Write();
  //}

  // save filter
  fn0->GetXaxis()->SetRange(0,250);
  fh0->GetXaxis()->SetRange(0,250);
  fn0->SetTitle("nnvt_filter");
  fh0->SetTitle("hmmt_filter");
  TCanvas *cjw = new TCanvas("cjw","cjw",1920,1080);
  cjw->Divide(2,1);
  cjw->cd(1);
  fn0->Draw();
  cjw->cd(2);
  fh0->Draw();
  cjw->Print("filter_all.png");
}
