#include "/junofs/users/jiangw/include/pmtflag.h"
void dnrDiff()
{
    const int num_para= 1;  //don't change it or the code will crush.
    const int num_file= 2;  //don't change it or the code will crush.
    const int num_PMT = 17612; //don't change it or the code will crush.
    bool *hmmt = pmtflag();
    double tmp;
    double dnr[num_file][num_para][num_PMT];
    std::string ver[num_file] = {"old","new"};
    std::string name_para[num_para] = {"darkNoiseRate"};
    std::string filename[num_file] = {
		"/junofs/users/jiangw/J21_calib/FFT/Parameters/old_CalibPars_DN.txt",
        //"/junofs/users/jiangw/J21_calib/FFT/DNR_alg1.txt",//old
        //"/junofs/users/jiangw/J21_calib/FFT/DNR.txt" //new, show in red line
		"/scratchfs/juno/jiangw/J21_calib/FFT/Parameters/CalibPars_DN.txt"
    };

    // first read new 
	int qwe;
    ifstream ifile;
    for(int f=0;f<num_file;++f){
        ifile.open(filename[f]);
        for(int i=0;i<num_PMT;++i)
			//if(f==1) 
				ifile >> qwe >> dnr[f][0][i];
//			else ifile >> dnr[f][0][i];
        ifile.close();
	    cout<<"read "<<ver[f]<<" complete."<<endl;
    }

    int bin[num_para] = {200};
	float minx[num_para]={0};
	float maxx[num_para]={100e3};
	int color[num_file] = {4,2};//2(new) is red
    TH1D *h[num_file][num_para];

    for(int i=0; i<num_file; ++i) for(int j=0; j<num_para; ++j){
        TString name = Form("%s_%s",ver[i].c_str(),name_para[j].c_str());
		h[i][j] = new TH1D(name,name,bin[j],minx[j],maxx[j]);
		h[i][j]->SetLineColor(color[i]);
		// if(j==5)	continue;
		for(int k=0;k<num_PMT;++k){
			// if(hmmt[k]) 
			h[i][j]->Fill(dnr[i][j][k]);
		}
    }
    //read truth
    double DCR;
    TH1D *truth_DCR = new TH1D("truth_DCR","truth_DCR",200,0,100e3);
    TFile *tfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre0/data/Simulation/ElecSim/PmtData_Lpmt.root");
    TTree *ttree = (TTree*)tfile->Get("PmtData_Lpmt");
    ttree->SetBranchStatus("*",0);
    ttree->SetBranchStatus("DCR",1);
    ttree->SetBranchAddress("DCR",&DCR);
    assert(num_PMT == ttree->GetEntries());
    for(int i=0; i<num_PMT; ++i){
        ttree->GetEntry(i);
        truth_DCR->Fill(DCR*1e3);
    }
    cout<<"Read truth DCR complete."<<endl;

    TCanvas *cvs = new TCanvas("cvs","cvs",1920,1080);
	// cvs->Divide(3,2);
	for(int j=0;j<num_para;++j){
		// cvs->cd(j+1);
		h[0][j]->Draw();
		h[1][j]->Draw("SAME");
        truth_DCR->Draw("SAME");
	}

    TCanvas *cv = new TCanvas("cv","cv",1920,1080);
    //double pmtid[num_PMT]; for(int i=0;i<num_PMT;++i)   pmtid[i]=i*1.0;
    //int id[2]={-1,12611};
    //double cp[num_PMT]={0};
	//for(int i=0;i<num_PMT;++i){++id[hmmt[i]];    cp[id[hmmt[i]]]=(dnr[1][0][i]-dnr[0][0][i])/dnr[0][0][i];}
    //TGraph *g = new TGraph(num_PMT,pmtid,cp);
	TH1D *g = new TH1D("g","DNR diff",num_PMT,0,num_PMT);
	for(int i=0;i<num_PMT;++i){g->SetBinContent(i+1,(dnr[1][0][i]-dnr[0][0][i])/dnr[0][0][i]);}
    cv->cd();
    g->Draw("HIST");
}
