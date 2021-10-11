#include "/junofs/users/jiangw/include/myincludes.h"
double Pi = 3.1415926;
const int Npmt = 17612;
double length = 1250.e-9;
double timeinterval = DNLEN.e-9;
vector<int> circle;
vector<int> copyno;
void setPMTMap();

bool script4()
{
  double tmu = 0.1;
  // gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);
  setPMTMap();
  bool* hmmtpmt = pmtflag();
  int* highqepmt = highqeflag();

  // FIXME merged files
  TFile* inputfile = new TFile("../Laser0.1/step12/user-root/user_calibCorr.root", "read");
  // TFile* inputfile2 = new TFile("/junofs/users/zhangxt/20inch/rec/deconvolution/runs/pars20200504/output_deconv.root", "read");
  TFile* inputfile2 = new TFile("../SCSSN/step12/user-root/user_calibCorr.root", "read");

  TH1D* muth = new TH1D("muth", "Total PMT #mu", Npmt, 0, Npmt);
  TH1D* muHth = new TH1D("muHth", "Dynode PMT #mu", Npmt, 0, Npmt);
  TH1D* muNth = new TH1D("muNth", "MCP PMT #mu", Npmt, 0, Npmt);
  TH1D* muHpdfth = new TH1D("muHpdfth", "Dynode PMT #mu Distribution", 100, 0, 2);
  TH1D* muNpdfth = new TH1D("muNpdfth", "MCP PMT #mu Distribution", 100, 0, 2);
  TH1D* darkrateHpdfth = new TH1D("darkrateHpdfth", "Dynode PMT Dark Rate Distribution", 200, 0, 200e3);
  TH1D* darkrateNpdfth = new TH1D("darkrateNpdfth", "MCP PMT Dark Rate Distribution", 200, 0, 200e3);
  vector<pair<bool, double>> mu;
  vector<pair<bool, double>> relaDE;
  vector<pair<bool, double>> darkrate;
  TH1D* mu2pdfth = new TH1D("mu2pdfth", "High #mu Distribution", 100, 0, 2);
  TH1D* mu1pdfth = new TH1D("mu1pdfth", "Low #mu Distribution", 100, 0, 2);

  TH1F* totalCharge = (TH1F*)inputfile->Get("totalCharge");
  TH1F* darkCharge = (TH1F*)inputfile2->Get("darkCharge");
  TH1F* totalWaveCount = (TH1F*)inputfile->Get("totalWaveCount");
  TH1F* totalWaveCount2=(TH1F*)inputfile2->Get("totalWaveCount");
  TH1F* pmtWaveCount = (TH1F*)inputfile->Get("pmtWaveCount");
  TH1F* darkWaveCount = (TH1F*)inputfile2->Get("darkWaveCount");
  TH1I* pmtHitCount = (TH1I*)inputfile->Get("pmtHitCount");
  TH1I* darkHitCount = (TH1I*)inputfile2->Get("darkHitCount");
  float totalWave = totalWaveCount->GetBinContent(1);
  float totalWave2=totalWaveCount2->GetBinContent(1);
  cout << "TotalWave: " << totalWave << endl;
  if (totalWave == 0) {
    cout << "Error: Totalwave histogram is empty!" << endl;
    totalWave = 1;
  }
  TH1F* realDarkrate = (TH1F*)inputfile2->Get("darkCharge");
  realDarkrate->Scale(1. / (totalWave2* timeinterval));
  darkWaveCount->Scale(1. / timeinterval * 1250.e-9);

  TH1F* spespec;
  for (int i = 0; i < Npmt; i++) {
    // for (int i = 0; i < 177; i++) {
    // relaDE
    TString name = Form("ch%d_charge_spec", i);
    spespec = (TH1F*)inputfile2->Get(name);
    double tmpdc = (realDarkrate->GetBinContent(i + 1)) * totalWave * length;
    // mu.push_back(make_pair(hmmtpmt[i], -1. * log((totalWave - (totalCharge->GetBinContent(i + 1) - tmpdc)) / totalWave)));  // Alg0
     mu.push_back(make_pair(hmmtpmt[i], -1. * log((totalWave - (totalCharge->GetBinContent(i + 1) - tmpdc) / spespec->GetMean()) / totalWave)));  // Alg1
    // mu.push_back(make_pair(hmmtpmt[i], -1. * log((totalWave - (pmtWaveCount->GetBinContent(i + 1) - tmpdc)) / totalWave)));
    // mu.push_back(make_pair(hmmtpmt[i], -1. * totalCharge->GetBinContent(i + 1) - tmpdc / totalwave));
    // mu.push_back(make_pair(hmmtpmt[i], -1. * log((totalWave - (pmtWaveCount->GetBinContent(i + 1))) / totalWave) - realDarkrate->GetBinContent(i + 1) * 1250. / 1.e9));
    // mu.push_back(make_pair(hmmtpmt[i], -1. * log((totalWave - (pmtWaveCount->GetBinContent(i + 1))) / totalWave) - darkWaveCount->GetBinContent(i + 1) / totalWave));
    // mu.push_back(make_pair(hmmtpmt[i], -1. * log((totalWave - (pmtWaveCount->GetBinContent(i + 1) - darkWaveCount->GetBinContent(i + 1)) / (1 - darkWaveCount->GetBinContent(i + 1) / totalWave)) / totalWave)));  // Alg2
    // mu.push_back(make_pair(hmmtpmt[i], -1. * log((totalWave - (pmtWaveCount->GetBinContent(i + 1) - darkWaveCount->GetBinContent(i + 1))) / totalWave)));  // Alg3
    //mu.push_back(make_pair(hmmtpmt[i], -1. * log((totalWave - pmtHitCount->GetBinContent(i + 1)) / totalWave) + log((totalWave - darkHitCount->GetBinContent(i + 1)) / totalWave)));  // Alg4
    // cout << totalCharge->GetBinContent(i + 1) - tmpdc << endl;
    // dark rate
    darkrate.push_back(make_pair(hmmtpmt[i], realDarkrate->GetBinContent(i + 1)));
    // print log
    if ((i) % 1000 == 0) cout << i << " finished" << endl;
  }

  for (int i = 0; i < Npmt; i++) {
    if (hmmtpmt[i] == true) {
      relaDE.push_back(make_pair(hmmtpmt[i], mu.at(i).second / tmu));
      muHth->SetBinContent(i + 1, relaDE.at(i).second);
      muHpdfth->Fill(relaDE.at(i).second);
      darkrateHpdfth->Fill(darkrate.at(i).second);
    } else {
      relaDE.push_back(make_pair(hmmtpmt[i], mu.at(i).second / tmu));
      muNth->SetBinContent(i + 1, relaDE.at(i).second);
      muNpdfth->Fill(relaDE.at(i).second);
      darkrateNpdfth->Fill(darkrate.at(i).second);
    }
    if (highqepmt[i] == 2) {
      mu2pdfth->Fill(relaDE.at(i).second);
    } else if (highqepmt[i] == 1) {
      mu1pdfth->Fill(relaDE.at(i).second);
    }
    muth->SetBinContent(i + 1, relaDE.at(i).second);
  }

  // output as a txt
  ofstream ofile("CalibPars_dn.txt");
  for (int i = 0; i < Npmt; i++) {
    // for(int i=0;i<177;i++){
    ofile << i << "\t" << relaDE.at(i).second << "\t" << darkrate.at(i).second << "\t1" << endl;
  }
  ofile.close();

  double zmin_relaDE = 100;
  double zmax_relaDE = -100;
  int xmax = 0;
  int ymax = 0;
  double zmin_darkrate = 1e10;
  double zmax_darkrate = 0;
  for (int i = 0; i < Npmt; i++) {
    if (copyno[i] > xmax) xmax = copyno[i];
    if (circle[i] > ymax) ymax = circle[i];
    if (relaDE.at(i).second > zmax_relaDE) zmax_relaDE = relaDE.at(i).second;
    if (relaDE.at(i).second < zmin_relaDE) zmin_relaDE = relaDE.at(i).second;
    if (darkrate.at(i).second > zmax_darkrate) zmax_darkrate = darkrate.at(i).second;
    if (darkrate.at(i).second < zmin_darkrate) zmin_darkrate = darkrate.at(i).second;
  }
  TH2F* mu2dth = new TH2F("mu2dth", "Total PMT Relative DE 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  TH2F* muH2dth = new TH2F("muH2dth", "Dynode PMT Relative DE 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  TH2F* muN2dth = new TH2F("muN2dth", "MCP PMT Relative DE 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  TH2F* darkrate2dth = new TH2F("darkrate2dth", "Total PMT Dark Rate 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  TH2F* darkrateH2dth = new TH2F("darkrateH2dth", "Dynode PMT Dark Rate 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  TH2F* darkrateN2dth = new TH2F("darkrateN2dth", "MCP PMT Dark Rate 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  for (int i = 0; i < Npmt; i++) {
    mu2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, relaDE.at(i).second);
    darkrate2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, darkrate.at(i).second);
    if (hmmtpmt[i] == true) {
      muH2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, relaDE.at(i).second);
      darkrateH2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, darkrate.at(i).second);
    } else {
      muN2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, relaDE.at(i).second);
      darkrateN2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, darkrate.at(i).second);
    }
  }

  // draw
  TCanvas* c = new TCanvas("c", "c", 800, 600);
  c->cd();
  muth->GetXaxis()->SetTitle("PMTID");
  muth->GetYaxis()->SetTitle("Relative DE");
  muth->Draw();
  c->Print("figs.pdf(");

  TCanvas* c_1 = new TCanvas("c_1", "c_1", 800, 600);
  c_1->cd();
  muHth->GetXaxis()->SetTitle("PMTID");
  muHth->GetYaxis()->SetTitle("Relative DE");
  muHth->Draw();
  c_1->Print("figs.pdf");

  TCanvas* c_10 = new TCanvas("c_10", "c_10", 800, 600);
  c_10->cd();
  muHpdfth->GetXaxis()->SetTitle("Relative DE");
  muHpdfth->GetYaxis()->SetTitle("Counts");
  muHpdfth->Draw();
  c_10->Print("figs.pdf");

  TCanvas* c_2 = new TCanvas("c_2", "c_2", 800, 600);
  c_2->cd();
  muNth->GetXaxis()->SetTitle("PMTID");
  muNth->GetYaxis()->SetTitle("Relative DE");
  muNth->Draw();
  c_2->Print("figs.pdf");

  TCanvas* c_20 = new TCanvas("c_20", "c_20", 800, 600);
  c_20->cd();
  muNpdfth->GetXaxis()->SetTitle("Relative DE");
  muNpdfth->GetYaxis()->SetTitle("Counts");
  muNpdfth->Draw();
  c_20->Print("figs.pdf");

  TCanvas* c3 = new TCanvas("c3", "c3", 800, 600);
  c3->cd();
  mu2dth->GetZaxis()->SetRangeUser(zmin_relaDE, zmax_relaDE);
  mu2dth->GetXaxis()->SetTitle("PMT");
  mu2dth->GetYaxis()->SetTitle("Rings");
  mu2dth->Draw("colz");
  c3->Print("figs.pdf");

  TCanvas* c3_1 = new TCanvas("c3_1", "c3_1", 800, 600);
  c3_1->cd();
  muH2dth->GetZaxis()->SetRangeUser(zmin_relaDE, zmax_relaDE);
  muH2dth->GetXaxis()->SetTitle("PMT");
  muH2dth->GetYaxis()->SetTitle("Rings");
  muH2dth->Draw("colz");
  c3_1->Print("figs.pdf");

  TCanvas* c3_2 = new TCanvas("c3_2", "c3_2", 800, 600);
  c3_2->cd();
  muN2dth->GetZaxis()->SetRangeUser(zmin_relaDE, zmax_relaDE);
  muN2dth->GetXaxis()->SetTitle("PMT");
  muN2dth->GetYaxis()->SetTitle("Rings");
  muN2dth->Draw("colz");
  c3_2->Print("figs.pdf");

  TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
  c2->cd();
  realDarkrate->GetXaxis()->SetTitle("PMTID");
  realDarkrate->GetYaxis()->SetTitle("Dark Rate (Hz)");
  realDarkrate->Draw();
  c2->Print("figs.pdf");

  TCanvas* c6 = new TCanvas("c6", "c6", 800, 600);
  c6->cd();
  darkrate2dth->GetZaxis()->SetRangeUser(zmin_darkrate, zmax_darkrate);
  darkrate2dth->GetXaxis()->SetTitle("PMT");
  darkrate2dth->GetYaxis()->SetTitle("Rings");
  darkrate2dth->Draw("colz");
  c6->Print("figs.pdf");

  TCanvas* c6_1 = new TCanvas("c6_1", "c6_1", 800, 600);
  c6_1->cd();
  darkrateH2dth->GetZaxis()->SetRangeUser(zmin_darkrate, zmax_darkrate);
  darkrateH2dth->GetXaxis()->SetTitle("PMT");
  darkrateH2dth->GetYaxis()->SetTitle("Rings");
  darkrateH2dth->Draw("colz");
  c6_1->Print("figs.pdf");

  TCanvas* c6_2 = new TCanvas("c6_2", "c6_2", 800, 600);
  c6_2->cd();
  darkrateN2dth->GetZaxis()->SetRangeUser(zmin_darkrate, zmax_darkrate);
  darkrateN2dth->GetXaxis()->SetTitle("PMT");
  darkrateN2dth->GetYaxis()->SetTitle("Rings");
  darkrateN2dth->Draw("colz");
  c6_2->Print("figs.pdf)");

  TCanvas* c2_1 = new TCanvas("c2_1", "c2_1", 800, 600);
  c2_1->cd();
  darkrateHpdfth->GetXaxis()->SetTitle("Dark Rate (Hz)");
  darkrateHpdfth->GetYaxis()->SetTitle("Counts");
  darkrateHpdfth->Draw();
  c2_1->Print("figs.pdf");

  TCanvas* c2_2 = new TCanvas("c2_2", "c2_2", 800, 600);
  c2_2->cd();
  darkrateNpdfth->GetXaxis()->SetTitle("Dark Rate (Hz)");
  darkrateNpdfth->GetYaxis()->SetTitle("Counts");
  darkrateNpdfth->Draw();
  c2_2->Print("figs.pdf");

  TCanvas* c4 = new TCanvas("c4", "c4", 800, 600);
  c4->cd();
  // mu2pdfth->GetXaxis()->SetTitle("Dark Rate (Hz)");
  // mu2pdfth->GetYaxis()->SetTitle("Counts");
  mu2pdfth->Draw();
  mu1pdfth->Draw("sames");
  c4->Print("figs.pdf");

  TFile* outf = new TFile("outf.root", "recreate");
  outf->cd();
  muth->Write();
  muHth->Write();
  muHpdfth->Write();
  muNth->Write();
  muNpdfth->Write();
  mu2dth->Write();
  realDarkrate->Write();
  darkrateHpdfth->Write();
  darkrateNpdfth->Write();
  return true;
}

void setPMTMap()
{
  // draw as a 2D pattern
  //TString filename = "/junofs/users/zhangxt/20inch/rec/deconvolution/runs/20191009sample/PMTPos_Acrylic_with_chimney.csv";
  TString filename = "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre0/offline/Simulation/DetSimV2/DetSimOptions/data/PMTPos_Acrylic_with_chimney.csv";
  ifstream sfile;
  sfile.open(filename, ios::in);
  int line = 0;
  vector<double> theta, phi;
  TVector3 pmt_p;
  vector<TVector3> m_pmt_pos;
  m_pmt_pos.clear();
  double mag = 19500;
  int tmp_lineno = -1, tmp_copyno = 0;
  double tmp_theta, tmp_phi;
  double tmp, tmp_flag = 0;
  while (sfile >> tmp >> tmp >> tmp >> tmp >> tmp_theta >> tmp_phi) {
    theta.push_back(tmp_theta);
    phi.push_back(tmp_phi);
    pmt_p.SetMagThetaPhi(mag, tmp_theta * Pi / 180, tmp_phi * Pi / 180);
    m_pmt_pos.push_back(pmt_p);
    if (tmp_flag != tmp_theta) {
      tmp_flag = tmp_theta;
      tmp_copyno = 0;
      tmp_lineno++;
    }
    copyno.push_back(tmp_copyno++);
    circle.push_back(tmp_lineno);
  }
  sfile.close();
}
