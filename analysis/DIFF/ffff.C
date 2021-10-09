#include "/junofs/users/jiangw/include/pmtflag.h"
void ffff()
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
     //ifile.open("../FFT_v2/Calib/all_CalibPars.txt");
     ifile.open("PmtPrtData_inte.txt");
	for (int i = 0; i < 17612; i++) {
        ifile >> tmp >>rela[i]>>gain[i]>>tfs[i]>>dr[i]>>spem[i];//<< "\t" << mu[i] * 0.1 << "\t" << gain[i] << "\t" << tfs[i] << "\t" << dr[i] << "\t" << spem[i] << endl;
  }     ifile.close();

//ifile.open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre0/data/Calibration/PMTCalibSvc/data/PmtPrtData_deconv.txt");	
//ifile.open("../EasyIntegral2/Calib/all_CalibPars.txt");
//ifile.open("PmtPrtData_deconv.txt");
ifile.open("timeOffset_subasd.txt");
//ifile.open("/afs/ihep.ac.cn/users/l/luoxj/Deconvolution_Sim/J20v2r0-Pre0_2nd/CalibPars_m.txt");
        for (int i = 0; i < 17612; i++) {
        ifile >>tfsN[i];//<< "\t" << mu[i] * 0.1 << "\t" << gain[i] << "\t" << tfs[i] << "\t" << dr[i] << "\t" << spem[i] << endl;
  }     ifile.close();

  ofstream ofile;
  ofile.open("testTTSwoAngle_inte.txt");
  for (int i = 0; i < 17612; i++) {
    ofile << i << "\t" << 0 << "\t" << 0 << "\t" << tfsN[i] << "\t" << 0 << "\t" << 0 << endl;
	//ofile << i << "\t" << rela[i] << "\t" << gain[i] << "\t" << tfsN[i] << "\t" << dr[i] << "\t" << spem[i] << endl;
  }
  ofile.close();

	//TH1D* h = new TH1D("h", "gain",150,0.92,1);
	//TH1D* h2= new TH1D("h2","gain",150,0.92,1);
	//TH1D* h = new TH1D("h", "DR",100,0,122000);
        //TH1D* h2= new TH1D("h2","DR",100,0,122000);
	//TH1D* h = new TH1D("h","meanGain",150,0.98,1.16);
	//TH1D* h2= new TH1D("h2","meanGain",150,0.98,1.16);
	//TH1D *h = new TH1D("hh", "Detect Efficiency",150,0.07,0.145);
	//TH1D *h2= new TH1D("hh2","Detect Efficiency",150,0.07,0.145);
//	TH1D *h = new TH1D("h", "Detect Efficiency",150,0.,2);
  //      TH1D *h2= new TH1D("h2","Detect Efficiency",150,0.,2);
	//for(int i=0; i<17612; ++i) if(1) h->Fill(rela[i]);
	//for(int i=0; i<17612; ++i) if(1) h2->Fill(relaN[i]);

//	double scale = hh->GetMean();
//	double scale2=hh2->GetMean();
//	for(int i=0; i<17612; ++i) h->Fill(rela[i]/scale);
  //      for(int i=0; i<17612; ++i) h2->Fill(relaN[i]/scale2);

	//h2->SetLineColor(kBlue);
	//h->SetLineColor(kRed);
        //h2->Draw();
	//h->Draw("SAME");
}
