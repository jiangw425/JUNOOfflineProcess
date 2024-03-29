#include "/junofs/users/jiangw/include/myincludes.h"
void ckAngle_3()
{
	TH1::AddDirectory(kFALSE);
    const int num_pmt = 17612;
    const int num_file= 3;
    bool* hmmtpmt = pmtflag();
    // TFile *ojw = TFile::Open("tmp.root","recreate");
    // const int n = 20000; 
    // int pmtID[n];
    // float local_z[n], local_x[n], local_y[n];
    std::vector<int>   *pmtID   = new std::vector<int>;
    std::vector<float> *local_x = new std::vector<float>;
    std::vector<float> *local_y = new std::vector<float>;
    std::vector<float> *local_z = new std::vector<float>;

    // TH1F *hit_pos[2][num_pmt];
    // for(int i=0;i<num_pmt;++i){
    //     TString tmpz = Form("%d_hit_z",i);
    //     hit_pos[0][i] = new TH1F(tmpz,tmpz,200,0,200);
    //     TString tmpa = Form("%d_hit_theta",i);
    //     hit_pos[1][i] = new TH1F(tmpa,tmpa,100,0,100);
    // }
    // int evtID;
    // float x, y, z;
    // std::vector<float> *local_z = new std::vector<float>;
    // TFile *f = TFile::Open("/junofs/production/data-production/Pre-Releases/J20v2r0-Pre0/e+/0-10MeV/0MeV/detsim/user-root/user-detsim-123.root");
    // TTree *c = (TTree*)f->Get("evt");
    
    // std::string path = "root://junoeos01.ihep.ac.cn//eos/juno/user/jiangw/ckAngle/10101.8_0_5832.29/user-root/user-detsim-";
    //std::string path = "/junofs/production/data-production/Pre-Releases/J20v2r0-Pre0/ACU+CLS/Laser/photon_11522/Laser_0_0_0/detsim/user-root/user-detsim-";
    //std::string path = "/junofs/production/data-production/Pre-Releases/J20v2r0-Pre0/e+/0-10MeV/10MeV/detsim/user-root/user-detsim-";
    std::string paths[num_file] = {
        // "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/00/e+_angle/e+_Uniform/0MeV/detsim/user-root/user-detsim-",
        // "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/11/e+_angle/e+_Uniform/0MeV/detsim/user-root/user-detsim-"
        "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/00/ACU-CLS/Ge68/Ge68_0_0_0/detsim/user-root/user-detsim-",
        "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/11/ACU-CLS/Ge68/Ge68_0_0_0/detsim/user-root/user-detsim-",
        //"root://junoeos01.ihep.ac.cn//eos/juno/user/jiangw/J21v2r0-Pre0/Ge68_0_0_0_default/user-root/user-detsim-",
        "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/ACU-CLS/Ge68/Ge68_0_0_0/detsim/user-root/user-detsim-",
    };
    // std::string names[4][2]={{"_","uniform"},{"off","on"},{"nnvt","hmmt"},{"z","theta"}};
    std::string names[2][2]={{"nnvt","hmmt"},{"z","theta"}};
    std::string c_names[num_file]={"00","11","J21v2"};
    std::string compare_names[2]={"_","uniform"};
    int color[num_file] = {1,4,2};//black, blue, red
    TH1F *H1D[2][num_file][2][2];// hist, copy | off, on |  nnvt, hmmt | z,theta
    TH1F *diffH1D[2][2];
    TString hname[2][num_file][2][2];
    int range[2] = {200,100};
    for(int i=0;i<2;++i) for(int j=0;j<num_file;++j) for(int k=0;k<2;++k) for(int l=0;l<2;++l){
        hname[i][j][k][l] = Form("%s_%s_%s_%s",compare_names[i].c_str(),c_names[j].c_str(),names[0][k].c_str(),names[1][l].c_str());
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
    // TH2F *XY[2][2];
    // TH3F *XYZ[2][2];
    int begin = 0;
    int num = 10;

    //file-begin to file-begin+num
    for(int fn=0;fn<num_file;++fn){
        std::string path = paths[fn];
        TChain *c = new TChain("evt");
        // for(int i=begin; i<begin+num; ++i)  c->Add(Form("%s%d.root",path.c_str(),i));
        c->Add(Form("%s*.root",path.c_str()));
        c->SetBranchStatus("*", 0);
        c->SetBranchStatus("pmtID", 1);
        c->SetBranchStatus("LocalPosX", 1);
        c->SetBranchStatus("LocalPosY", 1);
        c->SetBranchStatus("LocalPosZ", 1);
        c->SetBranchAddress("pmtID",&pmtID);
        c->SetBranchAddress("LocalPosX",&local_x);
        c->SetBranchAddress("LocalPosY",&local_y);
        c->SetBranchAddress("LocalPosZ",&local_z);

        // for(int i=0;i<2;++i){
            // TString name = Form("%s_%s_local_z",ntype[fn].c_str(),type[i].c_str());
            // H1D[0][fn][i] = new TH1D(name,name,200,0,200);
            // H1D[0][fn][i]->SetStats(false);
            // H1D[0][fn][i]->SetLineColor(color[fn]);
            // name = Form("%s_%s_local_theta",ntype[fn].c_str(),type[i].c_str());
            // H1D[1][fn][i] = new TH1D(name,name,100,0,100);
            // H1D[1][fn][i]->SetStats(false);
            // H1D[1][fn][i]->SetLineColor(color[fn]);
            // TString name = Form("%s_%s_local_XY",names[1][fn].c_str(),names[2][i].c_str());
            // XY[fn][i] = new TH2F(name,name,104,-260,260,108,-270,270);
            // XY[fn][i]->SetStats(false);
            // name = Form("%s_%s_local_XYZ",names[1][fn].c_str(),names[2][i].c_str());
            // XYZ[fn][i]=new TH3F(name,name,52,-260,260,54,-270,270,100,0,200);
        // }
        // TTree *t = new TTree("local","pmt local hit pos");
        // t->Branch("evtID",&evtID,"evtID/I");
        // t->Branch("local_xx",&x,"local_x/F");
        // t->Branch("local_yy",&y,"local_y/F");
        // t->Branch("local_zz",&z,"local_z/F");

        cout<<"Total entries: "<<c->GetEntries()<<endl;
        for(int i=0; i<c->GetEntries();++i)
        // for(int i=0; i<5000;++i)
        {
            local_x->clear();
            local_y->clear();
            local_z->clear();
            if(i%100==0) cout<<"Processing "<<i<<endl;
        //     memset(pmtID,0,sizeof(pmtID));
        //     memset(local_x,0,sizeof(local_x));
        //     memset(local_y,0,sizeof(local_y));
        //     memset(local_z,0,sizeof(local_z));
        //     //pmtID->clear();
        //     //local_z->clear();
            c->GetEntry(i);
            // cout << local_x->size() << "\t" << local_y->size() << "\t" << local_z->size() << endl; 
        //     evtID=i;
            for(int j=0; j<local_x->size();++j)
            {
                if(pmtID->at(j)>=17612) {
                    // cout << "WP_PMT ?: " << pmtID->at(j) << endl;
                    continue;
                }
        //         if(local_z[j]==0) continue;
                // if(!hmmtpmt[pmtID->at(j)])
                // {
                H1D[0][fn][hmmtpmt[pmtID->at(j)]][0]->Fill(local_z->at(j));
                TVector3 tmp(local_x->at(j),local_y->at(j),local_z->at(j));
                float tmptheta = tmp.Theta()/TMath::Pi()*180;
                H1D[0][fn][hmmtpmt[pmtID->at(j)]][1]->Fill(tmptheta);
                // XY[fn][hmmtpmt[pmtID->at(j)]]->Fill(local_x->at(j),local_y->at(j));
                // XYZ[fn][hmmtpmt[pmtID->at(j)]]->Fill(local_x->at(j),local_y->at(j),local_z->at(j));

                // hit_pos[0][pmtID->at(j)]->Fill(local_z->at(j));
                // hit_pos[1][pmtID->at(j)]->Fill(tmptheta);
        //             x=local_x[j];//->push_back(local_x[j]);
        //             y=local_y[j];//->push_back(local_y[j]);
        //             z=local_z[j];//->push_back(local_z[j]);
        //             t->Fill();
                // }
            }
        // //t->Fill();
        }
        c->~TChain();
    }
    delete pmtID;
    delete local_x;
    delete local_y;
    delete local_z;

    for(int j=0;j<num_file;++j) for(int k=0;k<2;++k) for(int l=0;l<2;++l){
        H1D[1][j][k][l] = (TH1F*)H1D[0][j][k][l]->Clone(hname[1][j][k][l]);
        H1D[1][j][k][l]->Scale(1./H1D[1][j][k][l]->Integral());
    }

    TString cmpname[2] = {"w/o reflection", "with reflection"};

    TCanvas *c[2];
    // TCanvas *c3 = new TCanvas("c3","c3",1920,1080);
    // TCanvas *c4 = new TCanvas("c4","c4",1920,1080);
    TList* tl[2][2][2];
    // i: hist, copy | j: off, on | k: nnvt, hmmt | l: z,theta 
    // c3->Print("0MeV_angle.pdf[");
    for(int l=0;l<2;++l){
        TString cname = Form("c%d",l);
        c[l] = new TCanvas(cname,cname,1920,1080);
        c[l]->Divide(2,2);
        // cout << "l: " << l << endl;
        for(int i=0;i<2;++i) for(int k=0;k<2;++k){
            // cout << "i: " << i << "\tk:"<<k <<endl;
            tl[i][k][l] = new TList();
            for(int j=0;j<num_file;++j) tl[i][k][l]->Add(H1D[i][j][k][l]);
            compare(c[l]->cd(2*i+k+1),tl[i][k][l],3,cmpname,0);
        }
        // c[l]->Print(Form("0MeV_%s.png",names[1][l].c_str()));
        // c[l]->Print("0MeV_angle.pdf");
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
	// c5->Print("0MeV_diff.png");

    // TFile *ojwz = TFile::Open("hit_z.root","recreate");
    // ojwz->cd();
    // for(int i=0;i<num_pmt;++i) hit_pos[0][i]->Write();
    // ojwz->Close();

    // TFile *ojwa = TFile::Open("hit_theta.root","recreate");
    // ojwa->cd();
    // for(int i=0;i<num_pmt;++i) hit_pos[1][i]->Write();
    // ojwa->Close();
	TFile *ojw = TFile::Open("hit_zt.root","recreate");
	for(int j=0;j<3;++j) for(int k=0;k<2;k++) for(int l=0;l<2;l++) H1D[1][j][k][l]->Write();
	ojw->Close();
}
