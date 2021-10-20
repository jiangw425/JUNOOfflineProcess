// #include "/junofs/users/jiangw/include/myincludes.h"
void ck_nPE_Ge68_r()
{
	TH1::AddDirectory(kFALSE);
    const int num_pmt = 17612;
    // bool* hmmtpmt = pmtflag();
    int CalibPosNum = 293;
    std::string Ge68_path[2] = {
        "/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/00/ACU-CLS/Ge68/Ge68",
        "/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/11/ACU-CLS/Ge68/Ge68"
    };

    double positionX[CalibPosNum];
    double positionY[CalibPosNum];
    double positionZ[CalibPosNum];
    double positionR[CalibPosNum];
    double positionR3[CalibPosNum];
    std::vector<TString>* StrPosX = new std::vector<TString>;
    std::vector<TString>* StrPosY = new std::vector<TString>;
    std::vector<TString>* StrPosZ = new std::vector<TString>;

    int posid = 0;
    string posdir = "/junofs/users/huanggh/Data/ACU_CLS_3D_Pos";
    std::ifstream cfile;
    cfile.open((posdir+"/ACU-CLS-GT_XYZPos.txt").c_str(), std::ios::in);
    for(int i=0;i<CalibPosNum;i++) {
        cfile>>posid;
        cfile>>positionX[i];
        cfile>>positionY[i];
        cfile>>positionZ[i];
        // std::cout<<positionX[i]<<'\t'<<positionZ[i]<<std::endl;
        positionR[i] = sqrt(positionX[i]*positionX[i]+positionY[i]*positionY[i]+positionZ[i]*positionZ[i])/1e3;
        positionR3[i]=positionR[i]*positionR[i]*positionR[i];
    }
    cfile.close();

    cfile.open( (posdir+"/ACU-CLS-GT_XYZPos.txt").c_str(), std::ios::in);
    for(int i=0;i<CalibPosNum;i++) {
        cfile>>posid;
        TString temp;
        cfile>>temp;
        StrPosX->push_back(temp);
        cfile>>temp;
        StrPosY->push_back(temp);
        cfile>>temp;
        StrPosZ->push_back(temp);
        // std::cout<<StrPosX->at(i)<<'\t'<<StrPosZ->at(i)<<std::endl;
    }

    // TH1D *nPE_r = new TH1D("nPE_r", "nPE_r", 20,0,20);
    // TH1D *nPE_r3= new TH1D("nPE_r3","nPE_r3",20,0,20);
    int tmp_totPE;
    double totPE[2][CalibPosNum];

    for(int t=0;t<2;++t) for(int i=0;i<CalibPosNum;i++) {
        TString simFilePath = Form("%s_"+ StrPosX->at(i) + "_" + StrPosY->at(i) + "_" + StrPosZ->at(i) +  "/detsim/user-root/user-detsim-0.root", Ge68_path[t].c_str());
        TChain *tc = new TChain("evt");
        tc->Add(simFilePath);
        tc->SetBranchStatus("*",0);
        tc->SetBranchStatus("totalPE",1);
        int sumPE = 0;
        tc->SetBranchAddress("totalPE",&tmp_totPE);
        for(int en=0;en<tc->GetEntries();++en){
            tc->GetEntry(en);
            sumPE+=tmp_totPE;
        }
        totPE[t][i] = sumPE/tc->GetEntries();
        cout << i <<"\t"<< totPE[t][i] << "\t" << tc->GetEntries() << endl;
        tc->~TChain();
    }

    TCanvas *c0 = new TCanvas("c0","c0",1600,900);
    TH1D *nPE_r0= new TH1D("nPE_r0","nPE_r0",20,0,20);
    TH1D *nPE_r1= new TH1D("nPE_r1","nPE_r1",20,0,20);
    for(int i=0;i<CalibPosNum;++i){
        int bin = int(positionR[i]);
        nPE_r0->SetBinContent(bin+1,totPE[0][i]);
        nPE_r1->SetBinContent(bin+1,totPE[1][i]);
    }
    c0->cd();
    nPE_r0->Draw();
    nPE_r1->SetLineColor(kRed);
    nPE_r1->Draw("SAME");

    TCanvas *c1 = new TCanvas("c1","c1",1600,900);
    TH1D *nPE_r30= new TH1D("nPE_r30","nPE_r30",20,0,6000);
    TH1D *nPE_r31= new TH1D("nPE_r31","nPE_r31",20,0,6000);
    for(int i=0;i<CalibPosNum;++i){
        int bin = int(positionR3[i]/300);
        nPE_r30->SetBinContent(bin+1,totPE[0][i]);
        nPE_r31->SetBinContent(bin+1,totPE[1][i]);
    }
    c1->cd();
    nPE_r30->Draw();
    nPE_r31->SetLineColor(kRed);
    nPE_r31->Draw("SAME");
}
