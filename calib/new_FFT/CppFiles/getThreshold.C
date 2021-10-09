void getThreshold()
{
	int k0=15, k1=50;
	int kd = k1-k0+1;
	double xjw[kd];
	//for(int j=0;j<kd;j++)
        //{
        //        xjw[j] = (k0+j)/1000.;
        //}
	
	double hrate[kd], nrate[kd];
	double nb,ns,hb,hs;
	ifstream ijw("Amp_threshold_rate.txt");
	//ifstream ijw("12width_threshold_rate.txt");
	for(int j=0;j<kd;j++)
	{
		ijw>>xjw[j]>>nb>>ns>>hb>>hs;
		nrate[j]= ns/sqrt(nb+ns);	hrate[j]= hs/sqrt(hb+hs); //111
		// nrate[j]= nb/(nb+ns);	hrate[j]= hb/(hb+hs); //222
	}
	TGraph *gn = new TGraph(kd,xjw,nrate);
	TGraph *gh = new TGraph(kd,xjw,hrate);
	
	double nmax=0, hmax=0;
	double nmaxbsl=0, hmaxbsl=0;
	for(int j=0;j<kd;++j)
	{
		if(nrate[j]>nmax) {nmax=nrate[j]; nmaxbsl=xjw[j];}
		if(hrate[j]>hmax) {hmax=hrate[j]; hmaxbsl=xjw[j];}
	}

	cout<<"The values below are just suggestions, please refer to Amp_threshold_1d.png."<<endl;
	cout<<"nnvt max rate at: "<< nmaxbsl << "\nhmmt max rate at: "<<hmaxbsl<<endl;

	gh->SetLineColor(kRed);
	
	TCanvas *cjw = new TCanvas("cjw","cjw",1600,900);
	cjw->cd();
	//gn->GetYaxis()->SetRangeUser(50,200);
	gn->Draw();
	gh->Draw("SAME");
	cjw->Print("Amp_threshold_1d.png");

}
