// #include "/junofs/users/jiangw/include/myincludes.h"
void ck_nPE_Ge68_r_4()
{
	TH1::AddDirectory(kFALSE);
    const int num_pmt = 17612;
    // bool* hmmtpmt = pmtflag();
    const int CalibPosNum = 43;
    const int nc = 4;
    int color[4] = {3,1,4,2};
    std::string Ge68_path[nc] = {
        "/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre0/ACU-CLS/Ge68/Ge68",
        "/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/00/ACU-CLS/Ge68/Ge68",
        "/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/11/ACU-CLS/Ge68/Ge68",
		"/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/22/ACU-CLS/Ge68/Ge68",
    };

    int order[CalibPosNum];
    for(int i=0;i<CalibPosNum;i++){
        if(i<21) order[i] = 42-i;
        else order[i] = i-21;
    }
    double positionX[2][CalibPosNum];
    double positionY[2][CalibPosNum];
    double positionZ[2][CalibPosNum];
    double positionR[2][CalibPosNum];
    double positionR3[2][CalibPosNum];
    std::vector<TString>* StrPosXo = new std::vector<TString>;
    std::vector<TString>* StrPosYo = new std::vector<TString>;
    std::vector<TString>* StrPosZo = new std::vector<TString>;
    std::vector<TString>* StrPosX = new std::vector<TString>;
    std::vector<TString>* StrPosY = new std::vector<TString>;
    std::vector<TString>* StrPosZ = new std::vector<TString>;

    int posid = 0;
    string posdir = "/junofs/users/huanggh/Data/ACU_CLS_3D_Pos";
    std::ifstream cfile;
    cfile.open((posdir+"/ACU-CLS-GT_XYZPos.txt").c_str(), std::ios::in);
    for(int i=0;i<CalibPosNum;i++) {
        cfile>>posid;
        cfile>>positionX[0][i];
        cfile>>positionY[0][i];
        cfile>>positionZ[0][i];
        // std::cout<<positionX[2][i]<<'\t'<<positionZ[2][i]<<std::endl;
        // positionR[0][i] = sqrt(positionX[0][i]*positionX[0][i]+positionY[0][i]*positionY[0][i]+positionZ[0][i]*positionZ[0][i])/1e3;
        positionR[0][i] = positionZ[0][i];
        positionR3[0][i]=positionR[0][i]*positionR[0][i]*positionR[0][i];
    }
    cfile.close();

    cfile.open( (posdir+"/ACU-CLS-GT_XYZPos.txt").c_str(), std::ios::in);
    for(int i=0;i<CalibPosNum;i++) {
        cfile>>posid;
        TString temp;
        cfile>>temp;
        StrPosXo->push_back(temp);
        cfile>>temp;
        StrPosYo->push_back(temp);
        cfile>>temp;
        StrPosZo->push_back(temp);
        // std::cout<<StrPosX->at(i)<<'\t'<<StrPosZ->at(i)<<std::endl;
    }

    for(int i=0;i<CalibPosNum;i++) {
        positionX[1][i] = positionX[0][order[i]];
        positionY[1][i] = positionY[0][order[i]];
        positionZ[1][i] = positionZ[0][order[i]];
        positionR[1][i] = positionR[0][order[i]];
        positionR3[1][i]=positionR3[0][order[i]];
        StrPosX->push_back(StrPosXo->at(order[i]));
        StrPosY->push_back(StrPosYo->at(order[i]));
        StrPosZ->push_back(StrPosZo->at(order[i]));
        cout << positionZ[1][i] << endl;
    }
    // TH1D *nPE_r = new TH1D("nPE_r", "nPE_r", 20,0,20);
    // TH1D *nPE_r3= new TH1D("nPE_r3","nPE_r3",20,0,20);
    int tmp_totPE;
    float edep;
    double totPE[2][2][nc][CalibPosNum]; // value,ratio | mean,sigma | file | calib pos

    for(int t=0;t<nc;++t) for(int i=0;i<CalibPosNum;i++) {
        TString simFilePath = Form("%s_"+ StrPosX->at(i) + "_" + StrPosY->at(i) + "_" + StrPosZ->at(i) +  "/detsim/user-root/user-detsim-*", Ge68_path[t].c_str());
        TChain *tc = new TChain("evt");
        tc->Add(simFilePath);
        tc->SetBranchStatus("*",0);
        if(t==0){
            tc->SetBranchStatus("totalPE",1);
            tc->SetBranchAddress("totalPE",&tmp_totPE);
        }
        else{
            tc->SetBranchStatus("LpmtPE",1);
            tc->SetBranchAddress("LpmtPE",&tmp_totPE);
        }
        tc->SetBranchStatus("edep",1);
        TH1D *nPE_pdf = new TH1D("npe","npe",400,800,2400);
        // int sumPE = 0;
        tc->SetBranchAddress("edep",&edep);
        for(int en=0;en<tc->GetEntries();++en){
            tc->GetEntry(en);
            if(edep<1.0215||edep>1.0225) continue;
            nPE_pdf->Fill(tmp_totPE);
            // sumPE+=tmp_totPE;
        }
        // totPE[t][i] = sumPE/tc->GetEntries();
        nPE_pdf->Fit("gaus","Q");
        TF1* func_temp = (TF1*)nPE_pdf->GetFunction("gaus");
        double mean = func_temp->GetParameter(1);
        totPE[0][0][t][i] = mean;
        totPE[0][1][t][i] = 100*func_temp->GetParameter(2)/mean;
        cout << i <<"\tmean: " << mean << "\tres: " << totPE[0][1][t][i] << "\tentries: " << tc->GetEntries() <<endl;
        tc->~TChain();
    }
    for(int t=0;t<nc;++t) for(int i=0;i<CalibPosNum;i++) for(int ms=0;ms<2;ms++){
        totPE[1][ms][t][i] = totPE[0][ms][t][i] / totPE[0][ms][0][i];
    }

    TCanvas *c0 = new TCanvas("c0","c0",1600,900);
    c0->Divide(2,2);
    TGraph *nPE_r[2][2][nc];
    float range[2][2][2] = {{{1100,1700},{2.5,5}},{{0.8,1.2},{0.8,1.2}}};
    for(int vr=0;vr<2;vr++) for(int ms=0;ms<2;ms++){
        c0->cd(ms+1+2*vr);
        for(int t=0;t<nc;++t){
            nPE_r[vr][ms][t] = new TGraph(CalibPosNum,positionR3[1],totPE[vr][ms][t]);
            nPE_r[vr][ms][t]->SetLineColor(color[t]);
            if(t==0){   nPE_r[vr][ms][t]->GetYaxis()->SetRangeUser(range[vr][ms][0],range[vr][ms][1]);}
            nPE_r[vr][ms][t]->Draw("SAME AC*");
        }
    }
}
