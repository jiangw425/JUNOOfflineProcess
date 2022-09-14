#include "/junofs/users/jiangw/include/pmtflag.h"
void script2()
{
  TFile* f = new TFile("SPE_bck.root", "read");
  // TFile* f = new TFile("temp.root", "update");
  TH1D* m_meanWaveform[20000];
  TH1D* m_Counter;
  int m_totalPMT = 17612;
  int m_length = ADCL;
  TH1* m_tempH;
  TH1D* m_SPERE[m_totalPMT];
  TH1D* m_SPEIM[m_totalPMT];
  TH1D* m_SPEAM[m_totalPMT];
  for (int i = 0; i < m_totalPMT; i++) {
    ostringstream out1;
    out1 << "PMTID_" << i << "_SPERE";
    m_SPERE[i] = new TH1D(out1.str().c_str(), out1.str().c_str(), 400, 0, 400);  // specified for J16v2
    ostringstream out2;
    out2 << "PMTID_" << i << "_SPEIM";
    m_SPEIM[i] = new TH1D(out2.str().c_str(), out2.str().c_str(), 400, 0, 400);  // specified for J16v2
    ostringstream out5;
    out5 << "PMTID_" << i << "_SPEAM";
    m_SPEAM[i] = new TH1D(out5.str().c_str(), out5.str().c_str(), 400, 0, 400);  // specified for J16v2
    ostringstream out3;
    out3 << "PMTID_" << i << "_MeanSpec";
    m_meanWaveform[i] = (TH1D*)f->Get(out3.str().c_str());
  }
  ostringstream out4;
  out4 << "PMT_Counter";
  m_Counter = (TH1D*)f->Get(out4.str().c_str());

  for (int i = 0; i < m_totalPMT; i++) {
    m_meanWaveform[i]->Scale(1. / m_Counter->GetBinContent(i + 1));
    delete TVirtualFFT::GetCurrentTransform();
    TVirtualFFT::SetTransform(0);
    m_tempH = m_meanWaveform[i]->FFT(m_tempH, "MAG");
    std::vector<double> re_full_vec(m_length);
    std::vector<double> im_full_vec(m_length);
    double* re_full = &re_full_vec.front();
    double* im_full = &im_full_vec.front();
    for (int j = 0; j < m_length; j++) {
      re_full[j] = 0;
      im_full[j] = 0;
    }
    TVirtualFFT* fft = TVirtualFFT::GetCurrentTransform();
    fft->GetPointsComplex(re_full, im_full);
    for (int j = 0; j < 400 && j < m_length; j++) {
      m_SPERE[i]->SetBinContent(j + 1, re_full[j]);
      m_SPEIM[i]->SetBinContent(j + 1, im_full[j]);
      m_SPEAM[i]->SetBinContent(j + 1, sqrt(re_full[j]*re_full[j]+im_full[j]*im_full[j]));
    }
  }
  TFile* outfile = new TFile("SPE.root", "recreate");
  outfile->cd();
  for (int i = 0; i < m_totalPMT; i++) {
    m_meanWaveform[i]->Write();
    m_SPERE[i]->Write();
    m_SPEIM[i]->Write();
  }

  //save SPE
  bool *hmmt = pmtflag();
  TH2D *spe2d[4][2]; // time | amp | re | im
  string pmt_name[2] = {"nnvt", "hmmt"};
  string save_name[4] = {"spectra", "amp_F", "re_F", "im_F"};
  int xbin[4] = {200,400,400,400};
  int ybin[4] = {155,1000,800,800};
  int ymin[4] = {-5,0,-800,-800};
  int ymax[4] = {150,1000,800,800};
  for(int s=0; s<4;++s) for(int t=0; t<2; ++t){
      TString name = Form("%s_SPE_%s",pmt_name[t].c_str(),save_name[s].c_str());
      spe2d[s][t] = new TH2D(name,name,xbin[s],0,xbin[s],ybin[s],ymin[s],ymax[s]);
  }
  
  for(int i=0; i<m_totalPMT; ++i){
      if(i%2000==0) cout << "save SPE ... Processing " << i << " pmts." << endl;
      // assert(tmp->GetXaxis()->GetNbins()!=num_PMT);
      for(int j=1; j<=200; ++j){
        spe2d[0][hmmt[i]]->Fill(j,m_meanWaveform[i]->GetBinContent(j));
      }
      for(int j=1; j<=400;++j){
        spe2d[1][hmmt[i]]->Fill(j,m_SPEAM[i]->GetBinContent(j));
        spe2d[2][hmmt[i]]->Fill(j,m_SPERE[i]->GetBinContent(j));
        spe2d[3][hmmt[i]]->Fill(j,m_SPEIM[i]->GetBinContent(j));
      }
  }

  TCanvas *cjw[4];
  for(int s=0; s<4;++s){
    TString name = Form("c_%s",save_name[s].c_str());
    cjw[s] = new TCanvas(name,name,1920,1080);
    cjw[s]->Divide(2,1);
    for(int t=0; t<2; ++t){
      cjw[s]->cd(t+1)->SetLogz();
      spe2d[s][t]->Draw("COLZ");
    }
    cjw[s]->Print(Form("SPE_%s.png",save_name[s].c_str()));
  }
}
