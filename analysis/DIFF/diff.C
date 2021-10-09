#include "/junofs/users/jiangw/include/pmtflag.h"
void diff()
{
	bool *hmmt = pmtflag();
	double rela[17612];
	double relaN[17612];
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
	//ifile.open("/junofs/users/jiangw/testSub/CalibPars/PmtPrtData_deconv.txt");
	ifile.open("/junofs/users/liujiaxi16/pmt/fft/calib/all_CalibPars.txt");
     //ifile.open("../deconvFinal/Calib/all_CalibPars.txt");
     //ifile.open("v1_PmtPrtData_deconv.txt");
     //ifile.open("testTTSwoAngle_inte.txt");
	for (int i = 0; i < 17612; i++) {
        ifile >> tmp >>rela[i]>>gain[i]>>tfs[i]>>dr[i]>>spem[i];//<< "\t" << mu[i] * 0.1 << "\t" << gain[i] << "\t" << tfs[i] << "\t" << dr[i] << "\t" << spem[i] << endl;
  }     ifile.close();

ifile.open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre2/data/Calibration/PMTCalibSvc/data/PmtPrtData_deconv.txt");	
//ifile.open("../EasyIntegral2/Calib/all_CalibPars.txt");
//ifile.open("PmtPrtData_deconv.txt");
//ifile.open("testTTSwoAngle_de.txt");
//ifile.open("/afs/ihep.ac.cn/users/l/luoxj/Deconvolution_Sim/J20v2r0-Pre0_2nd/CalibPars_m.txt");
//ifile.open("/junofs/users/jiangw/testSub/CalibPars/PmtPrtData_deconv.txt");
        for (int i = 0; i < 17612; i++) {
        ifile >> tmp >>relaN[i]>>gainN[i]>>tfsN[i]>>drN[i]>>spemN[i];//<< "\t" << mu[i] * 0.1 << "\t" << gain[i] << "\t" << tfs[i] << "\t" << dr[i] << "\t" << spem[i] << endl;
  }     ifile.close();

	//TH1D* h = new TH1D("h", "tfs",200,-50,50);
      //  TH1D* h2= new TH1D("h2","tfs",200,-50,50);
	//TH1D* h = new TH1D("h", "gain",150,0.92,1);
	//TH1D* h2= new TH1D("h2","gain",150,0.92,1);
	//TH1D* h = new TH1D("h", "DR",100,0,122000);
        //TH1D* h2= new TH1D("h2","DR",100,0,122000);
	//TH1D* h = new TH1D("h","meanGain",150,0.98,1.16);
	//TH1D* h2= new TH1D("h2","meanGain",150,0.98,1.16);
	TH1D *hh = new TH1D("hh", "Detect Efficiency",150,0.07,0.145);
	TH1D *hh2= new TH1D("hh2","Detect Efficiency",150,0.07,0.145);
	TH1D *h = new TH1D("h", "Detect Efficiency",150,0.,2);
    TH1D *h2= new TH1D("h2","Detect Efficiency",150,0.,2);

/*	double sum=0;
	double sumN=0;
	for(int i=0; i<17612; ++i) if(hmmt[i]) {sum+=tfs[i]; sumN+=tfsN[i];}
	for(int i=0; i<17612; ++i) if(hmmt[i]) {tfs[i]-=sum/5e3; tfsN[i]-=sumN/5e3;}
*/	
	for(int i=0; i<17612; ++i)  hh->Fill(tfs[i]);
	for(int i=0; i<17612; ++i)  hh2->Fill(tfsN[i]);

	double scale = hh->GetMean();
	double scale2=hh2->GetMean();
	
	for(int i=0; i<17612; ++i) h->Fill(rela[i]/scale);
    for(int i=0; i<17612; ++i) h2->Fill(relaN[i]/scale2);

	h2->SetLineColor(kBlue);
	h->SetLineColor(kRed);
        h->Draw();
	h2->Draw("SAME");
}
