void meanRaitoDiff(){
    const int num_para= 1;  //don't change it or the code will crush.
    const int num_file= 2;  //don't change it or the code will crush.
    const int num_PMT = 17612; //don't change it or the code will crush.
    double para[num_file][num_para][num_PMT];
    std::string ver[num_file] = {"old","new"};
    std::string name_para[num_para] = {"meanRatio"};
    std::string filename[num_file] = {//new are shown in red line
        // "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre0/data/Calibration/PMTCalibSvc/data/output_deconv.root",
		// "/junofs/users/jiangw/give2lxj/J21v1r0-Pre0/output_deconv.root",
		// "/scratchfs/juno/jiangw/J21_calib/FFT/forceTrigger/step12/user-root/user_calibCorr.root"
        // "/junofs/users/jiangw/J21_calib/FFT/C14/step12/user-root/725-975.root"
		"/junofs/users/liujiaxi16/calib/C14/step12/user-root/user_calibCorr.root",
		"/scratchfs/juno/jiangw/Pre2/C14/step12/user-root/user_calibCorr.root"
    };

    for(int f=0;f<num_file;++f){
        TFile* rf = TFile::Open(filename[f].c_str());
        TH1F *spe;
        for(int i=0; i<num_PMT; ++i){
            spe = (TH1F*)rf->Get(Form("ch%d_charge_spec", i));
            para[f][0][i] = spe->GetMean();
        }
        rf->Close();
        cout<<"read "<<ver[f]<<" complete."<<endl;
    }
    
    int bin[num_para] = {150};
	float minx[num_para]={0.98};
	// float maxx[num_para]={1.16};
    float maxx[num_para]={2};
	int color[num_file] = {4,2};//2(new) is red
    TH1D *h[num_file][num_para];
     for(int i=0; i<num_file; ++i) for(int j=0; j<num_para; ++j){
        TString name = Form("%s_%s",ver[i].c_str(),name_para[j].c_str());
		h[i][j] = new TH1D(name,name,bin[j],minx[j],maxx[j]);
		h[i][j]->SetLineColor(color[i]);
		for(int k=0;k<num_PMT;++k){
			// if(hmmt[k]) 
			h[i][j]->Fill(para[i][j][k]);
		}
    }

    TCanvas *cvs = new TCanvas("cvs","cvs",1920,1080);
	for(int j=0;j<num_para;++j){
		h[1][j]->Draw();
		h[0][j]->Draw("SAME");
	}
}