#include "/junofs/users/jiangw/include/pmtflag.h"
void speCompare(){
	bool *hmmt = pmtflag();
	double spe[17612];
	double speN[17612];
  	double gain[17612];
	double gainN[17612];
  	double dr[17612];
	double drN[17612];
  	double tmp;
  	double tfs[17612];
	double tfsN[17612];
	double spem[17612];
	double spemN[17612];

	ifstream ifile;
     //ifile.open("../FFTex_v2/all_CalibPars.txt");
     ifile.open("/junofs/users/jiangw/JUNOofflineProcess/SimpleIntegral/Ge68/run_step1/root/Integral.txt");
	for (int i = 0; i < 17612; i++) {
        ifile >> tmp >>spe[i]>>gain[i];//<< "\t" << mu[i] * 0.1 << "\t" << gain[i] << "\t" << tfs[i] << "\t" << dr[i] << "\t" << spem[i] << endl;
  }     ifile.close();
	
ifile.open("/junofs/users/jiangw/JUNOofflineProcess/FFT_v2/laser0.1_FFT/Laser0.1/run_step2/Integral.txt");
//ifile.open("PmtPrtData_inte.txt");
        for (int i = 0; i < 17612; i++) {
        ifile >> tmp >>speN[i]>>gainN[i];//<< "\t" << mu[i] * 0.1 << "\t" << gain[i] << "\t" << tfs[i] << "\t" << dr[i] << "\t" << spem[i] << endl;
  }     ifile.close();

	TH1D* h = new TH1D("h", "SPEsigma",150,250,550);
	TH1D* h2= new TH1D("h2","SPEsigma",150,250,550);
	//TH1D* h = new TH1D("h","DN",100,0,0.2);
	//TH1D* h2= new TH1D("h2","DN",100,0,0.2);
	for(int i=0; i<17612; ++i) if(!hmmt[i]) h->Fill(gain[i]);
	for(int i=0; i<17612; ++i) if(!hmmt[i]) h2->Fill(gainN[i]);
	
	h->SetLineColor(kBlue);
	h2->SetLineColor(kRed);
        h->Draw();
	h2->Draw("SAME");
}
