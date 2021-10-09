#include "/junofs/users/jiangw/include/pmtflag.h"
void diff2()
{
	bool *hmmt = pmtflag();
	const int n = 17612;
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
     ifile.open("../DN50_100/Calib/all_CalibPars.txt");
//	ifile.open("../TimeCalib/TimeOffset/timeOffset_roofit.txt");
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
//ifile.open("../TimeCalib_old/TimeOffset/NOangle/J20v2/tOffset.txt");
        for (int i = 0; i < 17612; i++) {
        ifile >> tmp >>relaN[i]>>gainN[i]>>tfsN[i]>>drN[i]>>spemN[i];//<< "\t" << mu[i] * 0.1 << "\t" << gain[i] << "\t" << tfs[i] << "\t" << dr[i] << "\t" << spem[i] << endl;
  }     ifile.close();

	double cp[n]={0};
	TH1D *cp1d[2];
	cp1d[0]=new TH1D("Nnvt","nnvt diff",n,0,n);
	cp1d[1]=new TH1D("Hmmt","hmmt diff",n,0,n);
	int id[2]={-1,12611};
	for(int i=0;i<n;++i)
	{
		id[hmmt[i]]++;
		//cp[i] = (tfs[i]-tfsN[i])/(tfsN[i]+365);
		//cp[i] = (rela[i]-relaN[i])/relaN[i];
		cp[id[hmmt[i]]] = (dr[i]-drN[i])/drN[i];
		//cp[i] = (spem[i]-spemN[i])/spemN[i];
		//cp[i] = (gain[i]-gainN[i])/gainN[i];
		//cp1d[hmmt[i]]->SetBinContent(i+1,cp[i]);
	}
	double pmtid[n]={0};
	for (int i=0;i<n;++i)
	{
		pmtid[i]=i*1.0;
	}
	//cp1d[0]->SetLineColor(kBlue);
	//cp1d[1]->SetLineColor(kRed);
	//cp1d[0]->Draw();
	//cp1d[1]->Draw("SAME");
	TGraph *g = new TGraph(n,pmtid,cp);
	//g->SetTitle("SPE ratio diff");
	g->Draw();

	//TH1D* h = new TH1D("h", "tfs",200,-50,50);
        //TH1D* h2= new TH1D("h2","tfs",200,-50,50);
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

/*	double sum=0;
	double sumN=0;
	for(int i=0; i<17612; ++i) if(hmmt[i]) {sum+=tfs[i]; sumN+=tfsN[i];}
	for(int i=0; i<17612; ++i) if(hmmt[i]) {tfs[i]-=sum/5e3; tfsN[i]-=sumN/5e3;}
*/	
	//for(int i=0; i<17612; ++i) if(!hmmt[i]) h->Fill(spem[i]);
	//for(int i=0; i<17612; ++i) if(!hmmt[i]) h2->Fill(spemN[i]);

//	double scale = hh->GetMean();
//	double scale2=hh2->GetMean();
//	for(int i=0; i<17612; ++i) h->Fill(rela[i]/scale);
  //      for(int i=0; i<17612; ++i) h2->Fill(relaN[i]/scale2);

	//h2->SetLineColor(kBlue);
	//h->SetLineColor(kRed);
    //    h2->Draw();
	//h->Draw("SAME");
}
