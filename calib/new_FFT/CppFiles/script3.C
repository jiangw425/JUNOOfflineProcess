#include "/junofs/users/jiangw/include/myincludes.h"

double Pi = 3.1415926;
const int Npmt = 17612;
double length = DNLEN.e-9;
vector<int> circle;
vector<int> copyno;
void setPMTMap();
double pcenter = 1;

void script3(int pmax = 3, int pnum = 2000, double pwidth = 0.35, double pmu = 0.05, int ploop = 0)
{
  bool* hmmtpmt = pmtflag();
  int loopN = Npmt;
  if (ploop != 0) loopN = ploop;

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);
  setPMTMap();
  nmax = pmax;

  // FIXME merged files
  TFile* inputfile = new TFile("user_calibCorr.root", "read");
  TH1F* th[Npmt];
  TH1D* muth = new TH1D("muth", "Total PMT #mu", Npmt, 0, Npmt);
  TH1D* muHth = new TH1D("muHth", "Dynode PMT #mu", Npmt, 0, Npmt);
  TH1D* muNth = new TH1D("muNth", "MCP PMT #mu", Npmt, 0, Npmt);
  TH1D* muHpdfth = new TH1D("muHpdfth", "Dynode PMT #mu Distribution", 300, 0, 0.3);
  TH1D* muNpdfth = new TH1D("muNpdfth", "MCP PMT #mu Distribution", 300, 0, 0.3);
  TH1D* darkrateHpdfth = new TH1D("darkrateHpdfth", "Dynode PMT Dark Rate Distribution", 200, 0, 200e3);
  TH1D* darkrateNpdfth = new TH1D("darkrateNpdfth", "MCP PMT Dark Rate Distribution", 200, 0, 200e3);
  TH1D* gainth = new TH1D("gainth", "Gain", Npmt, 0, Npmt);
  TH1D* chi2ndfth = new TH1D("chi2ndfth", "Chi2/NDF", Npmt, 0, Npmt);
  TH1D* chi2ndfpdfth = new TH1D("chi2ndfpdfth", "#chi^2/NDF Distribution", 500, 0, 5);
  TH1D* p0h = new TH1D("p0h", "p0h", 1000, 0, 10000);
  TH1D* p3h = new TH1D("p3h", "p3h", 100, 1, 2);
  TH1D* p4h = new TH1D("p4h", "p4h", 100, 0, 1);
  TH1D* p7h = new TH1D("p7h", "p7h", 100, 0, 0.05);
  vector<pair<bool, double>> mu;
  vector<pair<bool, double>> relaDE;
  vector<pair<bool, double>> darkrate;
  double gain[Npmt];
  double chi2ndf[Npmt];
  // double mumeanH = 0;
  // double mumeanN = 0;
  double mumean = 0;
  // TF1* tf1 = new TF1("tf1", "gaus(0)", 0, 2);
  // double par[3] = {1.e3, 1, 0.35};
  TF1* tf1 = new TF1("tf1", NIMmodel, 0.1, 8, 8);
  //TF1* tf2 = new TF1("tf2", "gaus(0)", 0.2, 1.4);
  TF1* tf2 = new TF1("tf2", "gaus(0)", GAINMIN, GAINMAX);
  TF1* tf;
  double par[8] = {20000, 0, 0, 1., 0.35, 0.05, 5, 0.1};
  double par2[3] = {20000, 1., 0.35};
  par[0] = pnum;
  par[3] = pcenter;
  par[4] = pwidth;
  par[7] = pmu;
  par2[0] = pnum;
  par2[1] = pcenter;
  par2[2] = pwidth;

  tf1->SetParameters(par);
  tf1->SetParLimits(0, 0, 1e5);
  tf1->FixParameter(1, 0);
  tf1->FixParameter(2, 0);
  tf1->SetParLimits(4, 0, 0.4);
  tf1->FixParameter(5, 0);
  tf1->FixParameter(6, 0);
  tf1->SetParLimits(7, 0, 1);

  tf2->SetParLimits(0, 0, 1e5);
  tf2->SetParLimits(1, 0, 1.5);
  TH1F* darkCount = (TH1F*)inputfile->Get("darkCharge");
  TH1F* totalWaveCount = (TH1F*)inputfile->Get("totalWaveCount");
  float totalWave = totalWaveCount->GetBinContent(1);
  cout << "TotalWave: " << totalWave << endl;
  if (totalWave == 0) {
    cout << "Error: Totalwave histogram is empty!" << endl;
    totalWave = 1;
  }
  // darkCount->Divide(totalWaveCount);
  darkCount->Scale(1. / (totalWave * length));

  TCanvas* cfit = new TCanvas("cfit", "", 800, 600);
  cfit->Print("figfit.pdf[");
  for (int i = 0; i < loopN; i++) {
    TString name = Form("ch%d_charge_spec", i);
    th[i] = (TH1F*)inputfile->Get(name);
    // th[i]->Rebin(5);
    cfit->cd();
    th[i]->GetXaxis()->SetRangeUser(-2, 8);
    th[i]->Draw();
    tf1->SetParameters(par);
    tf1->SetParLimits(0, 0, 1e5);
    tf1->FixParameter(1, 0);
    tf1->FixParameter(2, 0);
    tf1->SetParLimits(4, 0, 1.5);
    tf1->FixParameter(5, 0);
    tf1->FixParameter(6, 0);
    tf1->SetParLimits(7, 0, 1);
    tf2->SetParLimits(0, 0, 1e5);
    tf2->SetParLimits(1, 0, 1.5);
    if (hmmtpmt[i] == true)
      tf = tf1;
    else
      tf = tf2;
    th[i]->Fit(tf, "LRQ");
    double tmpchi2ndf = tf->GetChisquare() / tf->GetNDF();
    cfit->SetLogy();
    if (tmpchi2ndf > 3) {
      cout << "Refitting" << endl;
      tf1->SetParameters(par);
      tf1->FixParameter(1, 0);
      tf1->FixParameter(2, 0);
      tf1->SetParLimits(4, 0, 1.5);
      tf1->FixParameter(5, 0);
      tf1->FixParameter(6, 0);
      tf1->SetParLimits(0, 0, 1e5);
      tf1->SetParLimits(7, 0, 1);
      tf2->SetParLimits(0, 0, 1e5);
      tf2->SetParLimits(1, 0, 1.5);
      th[i]->Fit(tf, "LRQ+");
    }
    // if (tf->GetParameter(1) > 1.2) cfit->Print("figfit.pdf");
    // if (i < 50) cfit->Print("figfit.pdf");
    if (tf->GetChisquare() / tf->GetNDF() > 4 || i < 10) {
      cout << i << "th fitting failed! Chi2/NDF = " << tf->GetChisquare() / tf->GetNDF() << endl;
      cfit->Print("figfit.pdf");
    }
    mu.push_back(make_pair(hmmtpmt[i], -1. * log(1. - th[i]->Integral() / totalWave)));
    // gain
    int npar;
    int gpar;
    if (hmmtpmt[i] == true) {
      gpar = 3;
    } else {
      gpar = 1;
    }
    gain[i] = tf->GetParameter(gpar);
    gainth->SetBinContent(i + 1, gain[i]);
    gainth->SetBinError(i + 1, tf->GetParError(gpar));
    chi2ndf[i] = tf->GetChisquare() / tf->GetNDF();
    chi2ndfth->SetBinContent(i + 1, chi2ndf[i]);
    chi2ndfpdfth->Fill(tf->GetChisquare() / tf->GetNDF());
    p0h->Fill(tf->GetParameter(0));
    p3h->Fill(tf->GetParameter(gpar));
    if (hmmtpmt[i] == true) {
      p4h->Fill(tf1->GetParameter(4));
      p7h->Fill(tf1->GetParameter(7));
    }
    // dark rate
    darkrate.push_back(make_pair(hmmtpmt[i], darkCount->GetBinContent(i + 1) / gain[i]));
    // print log
    if ((i) % 1000 == 0) cout << i << " finished" << endl;
  }
  cfit->Print("figfit.pdf]");
  // mumeanH /= 5000.;
  // mumeanN /= (Npmt - 5000.);

  for (int i = 0; i < loopN; i++) {
    if (hmmtpmt[i] == true) {
      // relaDE.push_back(make_pair(hmmtpmt[i], mu.at(i).second / mumeanH));
      relaDE.push_back(make_pair(hmmtpmt[i], mu.at(i).second));
      muHth->SetBinContent(i + 1, relaDE.at(i).second);
      muHpdfth->Fill(relaDE.at(i).second);
      darkrateHpdfth->Fill(darkrate.at(i).second);
    } else {
      // relaDE.push_back(make_pair(hmmtpmt[i], mu.at(i).second / mumeanN));
      relaDE.push_back(make_pair(hmmtpmt[i], mu.at(i).second));
      muNth->SetBinContent(i + 1, relaDE.at(i).second);
      muNpdfth->Fill(relaDE.at(i).second);
      darkrateNpdfth->Fill(darkrate.at(i).second);
    }
    muth->SetBinContent(i + 1, relaDE.at(i).second);
  }

  // output as a txt
  ofstream ofile("CalibPars.txt");
  for (int i = 0; i < loopN; i++) {
    // for(int i=0;i<177;i++){
    ofile << i << "\t" << relaDE.at(i).second << "\t" << gain[i] << "\t" << darkrate.at(i).second << endl;
  }
  ofile.close();

  double zmin_relaDE = 100;
  double zmax_relaDE = -100;
  double zmin_gain = 100;
  double zmax_gain = 0;
  double zmin_darkrate = 1e10;
  double zmax_darkrate = 0;
  int xmax = 0;
  int ymax = 0;
  for (int i = 0; i < loopN; i++) {
    if (copyno[i] > xmax) xmax = copyno[i];
    if (circle[i] > ymax) ymax = circle[i];
    if (relaDE.at(i).second > zmax_relaDE) zmax_relaDE = relaDE.at(i).second;
    if (relaDE.at(i).second < zmin_relaDE) zmin_relaDE = relaDE.at(i).second;
    if (gain[i] > zmax_gain) zmax_gain = gain[i];
    if (gain[i] < zmin_gain) zmin_gain = gain[i];
    if (darkrate.at(i).second > zmax_darkrate) zmax_darkrate = darkrate.at(i).second;
    if (darkrate.at(i).second < zmin_darkrate) zmin_darkrate = darkrate.at(i).second;
  }
  TH2F* mu2dth = new TH2F("mu2dth", "Total PMT Relative DE 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  TH2F* muH2dth = new TH2F("muH2dth", "Dynode PMT Relative DE 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  TH2F* muN2dth = new TH2F("muN2dth", "MCP PMT Relative DE 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  TH2F* gain2dth = new TH2F("gain2dth", "Gain 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  TH2F* darkrate2dth = new TH2F("darkrate2dth", "Total PMT Dark Rate 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  TH2F* darkrateH2dth = new TH2F("darkrateH2dth", "Dynode PMT Dark Rate 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  TH2F* darkrateN2dth = new TH2F("darkrateN2dth", "MCP PMT Dark Rate 2D", xmax + 1, 0, xmax + 1, ymax + 1, 0, ymax + 1);
  for (int i = 0; i < loopN; i++) {
    mu2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, relaDE.at(i).second);
    darkrate2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, darkrate.at(i).second);
    if (hmmtpmt[i] == true) {
      muH2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, relaDE.at(i).second);
      darkrateH2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, darkrate.at(i).second);
    } else {
      muN2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, relaDE.at(i).second);
      darkrateN2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, darkrate.at(i).second);
    }
    gain2dth->SetBinContent(copyno[i] + 1, ymax - (circle[i]) + 1, gain[i]);
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

  TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
  c1->cd();
  gainth->GetXaxis()->SetTitle("PMTID");
  gainth->GetYaxis()->SetTitle("Relative Gain");
  gainth->Draw();
  c1->Print("figs.pdf");

  TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
  c2->cd();
  darkCount->GetXaxis()->SetTitle("PMTID");
  darkCount->GetYaxis()->SetTitle("Dark Rate (Hz)");
  darkCount->Draw();
  c2->Print("figs.pdf");

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

  TCanvas* c4 = new TCanvas("c4", "c4", 800, 600);
  c4->cd();
  gain2dth->GetZaxis()->SetRangeUser(zmin_gain, zmax_gain);
  gain2dth->GetXaxis()->SetTitle("PMT");
  gain2dth->GetYaxis()->SetTitle("Rings");
  gain2dth->Draw("colz");
  c4->Print("figs.pdf");

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

  TFile* outf = new TFile("outf.root", "recreate");
  outf->cd();
  muth->Write();
  muHth->Write();
  muHpdfth->Write();
  muNth->Write();
  muNpdfth->Write();
  gainth->Write();
  darkCount->Write();
  darkrateHpdfth->Write();
  darkrateNpdfth->Write();
  mu2dth->Write();
  gain2dth->Write();
  darkrate2dth->Write();
  darkrateH2dth->Write();
  darkrateN2dth->Write();
  chi2ndfth->Write();
  chi2ndfpdfth->Write();
  p0h->Write();
  p3h->Write();
  p4h->Write();
  p7h->Write();
}

void setPMTMap()
{
  // draw as a 2D pattern
  TString filename = "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre0/offline/Simulation/DetSimV2/DetSimOptions/data/PMTPos_Acrylic_with_chimney.csv";
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
