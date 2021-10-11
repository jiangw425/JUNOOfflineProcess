#include "/junofs/users/jiangw/include/pmtflag.h"
void AllParaDiff()
{
	const int num_para= 5;  //don't change it or the code will crush.
    const int num_file= 2;  //don't change it or the code will crush.
	const int num_PMT = 17612;
	bool *hmmt = pmtflag();
	double tmp;
	double parameter[num_file][num_para][num_PMT]; //old new//relaDE SPEratio timeOffset DNR meanGain//
	std::string ver[num_file] = {"old","new"};
	std::string pa[num_para+1] = {"relaDE","SPEratio","timeOffset","DarkNoiseRate","meanRatio","fixed_relaDE"};
	std::string filename[num_file] = {
		//"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre0/data/Calibration/PMTCalibSvc/data/PmtPrtData_deconv.txt",//old
        //"/junofs/users/jiangw/J21_calib/FFT/Parameters/moved_all_CalibPars.txt" //new, show in red line
		//"/junofs/users/jiangw/give2lxj/J21v1r0-Pre0/PmtPrtData_inte.txt",
		//"/junofs/users/jiangw/give2lxj/J21v1r0-Pre0/PmtPrtData_deconv.txt"
		//"/scratchfs/juno/liujiaxi16/calib/Parameters/moved_all_CalibPars.txt",
		"/scratchfs/juno/jiangw/Pre2_bug/Parameters/moved_all_CalibPars.txt",
		"/scratchfs/juno/jiangw/Pre2/Parameters/moved_all_CalibPars.txt"
    };

	// first read new 
    ifstream ifile;
    for(int f=0;f<num_file;++f){
        ifile.open(filename[f]);
        for(int i=0;i<num_PMT;++i)
            ifile >> tmp >> parameter[f][0][i] >> parameter[f][1][i] >> parameter[f][2][i] >> parameter[f][3][i] >> parameter[f][4][i];
        ifile.close();
	    cout<<"read "<<ver[f]<<" complete."<<endl;
    }
	
	int bin[num_para+1] = {150,150,200,100,150,150};
	float minx[num_para+1]={0.07 , 0.92,-50,0,    0.98,0};
	float maxx[num_para+1]={0.25, 1.,   50,122e3,1.16,2};
	int color[num_file] = {4,2};//2(new) is red
	TH1D *h[num_file][num_para+1];

	for(int i=0; i<num_file;++i) for(int j=0;j<num_para+1;++j){
		TString name = Form("%s_%s",ver[i].c_str(),pa[j].c_str());
		h[i][j] = new TH1D(name,name,bin[j],minx[j],maxx[j]);
		h[i][j]->SetLineColor(color[i]);
		if(j==num_para) continue;
		for(int k=0;k<num_PMT;++k){
	//		if(!hmmt[k]) 
			h[i][j]->Fill(parameter[i][j][k]);
		}
	}
	double sum=0;
	double sum2=0;
	for(int i=0; i<num_PMT; ++i){
		sum += parameter[0][0][i];
		sum2+= parameter[1][0][i];
	}
	double scale = sum/num_PMT;
	double scale2= sum2/num_PMT;
	for(int i=0; i<num_PMT; ++i){
		h[0][num_para]->Fill(parameter[0][0][i]/scale);
		h[1][num_para]->Fill(parameter[1][0][i]/scale2);
	}
	// cout<<"\nscale : "<<h[0][5]->GetMean()<<endl;
	// cout<<"scale2: "<<h[1][5]->GetMean()<<endl;
	cout<<"Fill complete."<<endl;
	
	TCanvas *cvs = new TCanvas("cvs","cvs",1920,1080);
	cvs->Divide(3,2);
	for(int j=0;j<=num_para;++j){
		cvs->cd(j+1);
		h[1][j]->Draw("HIST");
		h[0][j]->Draw("SAME HIST");
	}

	TH1D *d[num_para+1];
	for(int i=0;i<num_para+1;++i){
		TString name = Form("%s_diff",pa[i].c_str());
		d[i] = new TH1D(name,name,num_PMT,0,num_PMT);
		d[i]->SetLineColor(kRed);
		if(i==num_para){
			for(int j=0;j<num_PMT;++j) d[i]->SetBinContent(j+1,(parameter[1][0][j]/scale2)-(parameter[0][0][j]/scale));
			continue;
		}
		for(int j=0;j<num_PMT;++j){
			d[i]->SetBinContent(j+1,parameter[1][i][j]-parameter[0][i][j]);
		}
	}
	TCanvas *cvs2= new TCanvas("cvs2","cvs2",1920,1080);
	cvs2->Divide(3,2);
	for(int j=0;j<num_para+1;++j){
		cvs2->cd(j+1);
		d[j]->Draw("HIST");
	}

	if(false){
		int id[2]={-1,12611};
		double mr_diff[num_PMT]={0};
		double pmtid[num_PMT]={0};
		for(int i=0;i<num_PMT;++i){
			++id[hmmt[i]];
			mr_diff[id[hmmt[i]]] = parameter[1][4][i]-parameter[0][4][i];
			pmtid[i]=i*1.0;
		}
		TCanvas *mr_c = new TCanvas("mrc","mrc",1600,900);
		mr_c->cd();
		TGraph *mr_g = new TGraph(num_PMT,pmtid,mr_diff);
		mr_g->Draw();
	}

}
