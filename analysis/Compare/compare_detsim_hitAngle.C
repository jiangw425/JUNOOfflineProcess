#include "/junofs/users/jiangw/include/myincludes.h"
void compare_detsim_hitAngle()
{
	TH1::AddDirectory(kFALSE);
    const int num_pmt = 17612;
    bool* hmmtpmt = pmtflag();
    std::vector<int>   *pmtID   = new std::vector<int>;
    std::vector<float> *local_x = new std::vector<float>;
    std::vector<float> *local_y = new std::vector<float>;
    std::vector<float> *local_z = new std::vector<float>;
    std::string path[2] = {
        "OLD",
        "NEW",
    };
    std::string names[4][2]={{"_","uniform"},{"old","new"},{"nnvt","hmmt"},{"z","theta"}};
    int color[2] = {1,2};//blue, red
    TH1F *H1D[2][2][2][2];// hist, copy | off, on |  nnvt, hmmt | z,theta
    TH1F *diffH1D[2][2];
    TString hname[2][2][2][2];
    int range[2] = {200,100};
    for(int i=0;i<2;++i) for(int j=0;j<2;++j) for(int k=0;k<2;++k) for(int l=0;l<2;++l){
        hname[i][j][k][l] = Form("%s_%s_%s_%s",names[0][i].c_str(),names[1][j].c_str(),names[2][k].c_str(),names[3][l].c_str());
        if(i==0){
            H1D[i][j][k][l] = new TH1F(hname[i][j][k][l],hname[i][j][k][l],range[l],0,range[l]);
            H1D[i][j][k][l]->SetLineColor(color[j]);
            H1D[i][j][k][l]->SetStats(false);
        }
		if(i==0&&j==0){
			TString tname = hname[i][j][k][l] + Form("_diff");
			diffH1D[k][l] = new TH1F(tname,tname,range[l],0,range[l]);
		}
    }

    for(int fn=0;fn<2;++fn){
        TChain *c = new TChain("evt");
        c->Add(Form("%s/*.root",path[fn].c_str()));
        c->SetBranchStatus("*", 0);
        c->SetBranchStatus("pmtID", 1);
        c->SetBranchStatus("LocalPosX", 1);
        c->SetBranchStatus("LocalPosY", 1);
        c->SetBranchStatus("LocalPosZ", 1);
        c->SetBranchAddress("pmtID",&pmtID);
        c->SetBranchAddress("LocalPosX",&local_x);
        c->SetBranchAddress("LocalPosY",&local_y);
        c->SetBranchAddress("LocalPosZ",&local_z);
        int totalEntries = t->GetEntries();
        cout << path[fn] << " total entries: " << totalEntries << endl;
        
        int tmpentries = 5000 < totalEntries ? 5000 : totalEntries;
        for(int i=0; i<tmpentries;++i)
        {
            pmtID   -> clear();
            local_x -> clear();
            local_y -> clear();
            local_z -> clear();
            if(i%100==0) cout << "Processing " << i << "/" << tmpentries << endl;
            c->GetEntry(i);
            for(int j=0; j<local_x->size();++j)
            {
                if(pmtID->at(j)>=17612) continue;
                H1D[0][fn][hmmtpmt[pmtID->at(j)]][0]->Fill(local_z->at(j));
                TVector3 tmp(local_x->at(j),local_y->at(j),local_z->at(j));
                float tmptheta = tmp.Theta()/TMath::Pi()*180;
                H1D[0][fn][hmmtpmt[pmtID->at(j)]][1]->Fill(tmptheta);
            }
        }
        c->~TChain();
    }
    delete pmtID;
    delete local_x;
    delete local_y;
    delete local_z;

    for(int j=0;j<2;++j) for(int k=0;k<2;++k) for(int l=0;l<2;++l){
        H1D[1][j][k][l] = (TH1F*)H1D[0][j][k][l]->Clone(hname[1][j][k][l]);
        H1D[1][j][k][l]->Scale(1./H1D[1][j][k][l]->Integral());
    }

    TString cmpname[2] = {"old","new"};

    TCanvas *c[2];
    TList* tl[2][2][2];
    // i: hist, copy | j: off, on | k: nnvt, hmmt | l: z,theta 
    for(int l=0;l<2;++l){
        TString cname = Form("c%d",l);
        c[l] = new TCanvas(cname,cname,1920,1080);
        c[l]->Divide(2,2);
        for(int i=0;i<2;++i) for(int k=0;k<2;++k){
            tl[i][k][l] = new TList();
            for(int j=0;j<2;++j) tl[i][k][l]->Add(H1D[i][j][k][l]);
            compare(c[l]->cd(2*i+k+1),tl[i][k][l],2,cmpname,0);
        }
        c[l]->Print(Form("%s.png",names[3][l].c_str()));
    }

    // c3->Divide(2,2);
    // for(int i=0;i<4;++i){
    //     c3->cd(i+1);
    //     XY[i/2][i%2]->Draw("SURF2");
    // }
    // c3->Print("0MeV_xy.png");

    // c4->Divide(2,2);
    // for(int i=0;i<4;++i){
    //     c4->cd(i+1);
    //     XYZ[i/2][i%2]->Draw("COLZ");
    // }
    // c3->Print("0MeV_angle.pdf");
    // c4->Print("0MeV_angle.pdf");
    // c4->Print("0MeV_xyz.png");
    // c4->Print("0MeV_angle.pdf]");

    // //TFile *ojw = TFile::Open("1251.root","recreate");
    // ojw->cd();
    // t->Write();
    // ojw->Close();
	TCanvas *c5 = new TCanvas("c5","c5",1920,1080);
	c5->Divide(2,2);
	for(int k=0;k<2;++k) for(int l=0;l<2;++l){
		c5->cd(k+2*l+1);
		diffH1D[k][l]->Add(H1D[0][0][k][l],-1);
		diffH1D[k][l]->Add(H1D[0][1][k][l],1);
		diffH1D[k][l]->Draw("HIST");
	}
	c5->Print("diff.png");

    // TFile *ojwz = TFile::Open("hit_z.root","recreate");
    // ojwz->cd();
    // for(int i=0;i<num_pmt;++i) hit_pos[0][i]->Write();
    // ojwz->Close();

    // TFile *ojwa = TFile::Open("hit_theta.root","recreate");
    // ojwa->cd();
    // for(int i=0;i<num_pmt;++i) hit_pos[1][i]->Write();
    // ojwa->Close();
}
