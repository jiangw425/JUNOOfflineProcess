#include "/junofs/users/jiangw/include/pmtflag.h"
void tfsDiff()
{
    const int num_para= 1;  //don't change it or the code will crush.
    const int num_file= 2;  //don't change it or the code will crush.
    const int num_PMT = 17612; //don't change it or the code will crush.
    bool *hmmt = pmtflag();
    double tmp;
    double tfs[num_file][num_para][num_PMT];
    std::string ver[num_file] = {"old","new"};
    std::string name_para[num_para] = {"timeOffset"};
    std::string filename[num_file] = {
        "/junofs/users/jiangw/J21_calib/TFS/Laser0.1_noSelect/timeOffset_sub_roofit.txt",//old
        "/junofs/users/jiangw/J21_calib/TFS/Laser0.1/timeOffset_sub_roofit.txt" //new, show in red line
    };

    // first read new 
    ifstream ifile;
    for(int f=0;f<num_file;++f){
        ifile.open(filename[f]);
        for(int i=0;i<num_PMT;++i)
            ifile >> tfs[f][0][i];
        ifile.close();
	    cout<<"read "<<ver[f]<<" complete."<<endl;
    }

    int bin[num_para] = {200};
	float minx[num_para]={-50};
	float maxx[num_para]={50};
	int color[num_file] = {4,2};//2(new) is red
    TH1D *h[num_file][num_para];

    for(int i=0; i<num_file; ++i) for(int j=0; j<num_para; ++j){
        TString name = Form("%s_%s",ver[i].c_str(),name_para[j].c_str());
		h[i][j] = new TH1D(name,name,bin[j],minx[j],maxx[j]);
		h[i][j]->SetLineColor(color[i]);
		// if(j==5)	continue;
		for(int k=0;k<num_PMT;++k){
			// if(hmmt[k]) 
			h[i][j]->Fill(tfs[i][j][k]);
		}
    }
    cout<<"Fill complete."<<endl;

    TCanvas *cvs = new TCanvas("cvs","cvs",1920,1080);
	// cvs->Divide(3,2);
	for(int j=0;j<num_para;++j){
		// cvs->cd(j+1);
		h[0][j]->Draw("HIST");
		h[1][j]->Draw("SAME HIST");
	}
}
