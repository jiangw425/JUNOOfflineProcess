// #include "/junofs/users/jiangw/include/myincludes.h"
void ck_nPE_Ge68_r_4()
{
	TH1::AddDirectory(kFALSE);
    const int num_pmt = 17612;
    // bool* hmmtpmt = pmtflag();
    int CalibPosNum = 43;
    const int nc = 4;
    std::string Ge68_path[nc] = {
        "/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/00/ACU-CLS/Ge68/Ge68",
        "/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/11/ACU-CLS/Ge68/Ge68",
		"/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/22/ACU-CLS/Ge68/Ge68",
        "/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre0/ACU-CLS/Ge68/Ge68",
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
    double totPE[nc+1][CalibPosNum];
    double totPEres[nc][CalibPosNum];

    for(int t=0;t<nc;++t) for(int i=0;i<CalibPosNum;i++) {
        TString simFilePath = Form("%s_"+ StrPosX->at(i) + "_" + StrPosY->at(i) + "_" + StrPosZ->at(i) +  "/detsim/user-root/user-detsim-*", Ge68_path[t].c_str());
        TChain *tc = new TChain("evt");
        tc->Add(simFilePath);
        tc->SetBranchStatus("*",0);
        if(t==nc-1){
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
        totPE[t][i] = mean;
        totPEres[t][i] = 100*func_temp->GetParameter(2)/mean;
        cout << i <<"\tavg_nPE: "<< totPE[t][i] << "  \tmean: " << mean << "\tres: " << totPEres[t][i] << "\tentries: " << tc->GetEntries() <<endl;
        tc->~TChain();
    }
    for(int i=0;i<CalibPosNum;i++){
        // totPE[nc][i] = totPE[nc-1][i] - totPE[0][i];
        // cout << i << ":\t" << totPE[0][i] << "\tup:\t" << 100.*totPE[nc][i]/totPE[0][i] << "\%" << endl;

        totPE[nc][i] = totPE[2][i] - totPE[1][i];
        cout << i << ":\t" << totPE[1][i] << "\tup:\t" << 100.*totPE[nc][i]/totPE[1][i] << "\%" << endl;
    }

    for(int i=0;i<CalibPosNum;i++){
        double tmp = 100. * (totPEres[0][i] - totPEres[nc-1][i]) / totPEres[nc-1][i];
        cout << i << " " << positionZ[1][i] <<" :\t totalPE res increase :\t" << tmp << "\t\%"<<endl;
    }

    TCanvas *c0 = new TCanvas("c0","c0",1600,900);
    TGraph *nPE_r0 = new TGraph(CalibPosNum,positionR3[1],totPE[0]);
    TGraph *nPE_r1 = new TGraph(CalibPosNum,positionR3[1],totPE[1]);
    TGraph *nPE_r2 = new TGraph(CalibPosNum,positionR3[1],totPE[2]);
    TGraph *nPE_r3 = new TGraph(CalibPosNum,positionR3[1],totPE[3]);
    TGraph *nPE_r4 = new TGraph(CalibPosNum,positionR3[1],totPE[4]);
    // TH1D *nPE_r0= new TH1D("nPE_r0","nPE_r0",20,0,20);
    // TH1D *nPE_r1= new TH1D("nPE_r1","nPE_r1",20,0,20);
    // for(int i=0;i<CalibPosNum;++i){
    //     int bin = int(positionR[2][i]);
    //     nPE_r0->SetBinContent(bin+1,totPE[0][i]);
    //     nPE_r1->SetBinContent(bin+1,totPE[1][i]);
    // }
    c0->cd();
    nPE_r2->SetLineColor(kRed);
    nPE_r1->SetLineColor(kBlue);
    nPE_r3->SetLineColor(kGreen);
    nPE_r2->GetYaxis()->SetRangeUser(1100,1800);
    nPE_r2->Draw("AC*");
    nPE_r3->Draw("SAME C*");
    nPE_r1->Draw("SAME C*");
    nPE_r0->Draw("SAME C*");

    TCanvas *c1 = new TCanvas("c1","c1",1600,900);
    TGraph *nPE_r0res = new TGraph(CalibPosNum,positionR3[1],totPEres[0]);
    TGraph *nPE_r1res = new TGraph(CalibPosNum,positionR3[1],totPEres[1]);
    TGraph *nPE_r2res = new TGraph(CalibPosNum,positionR3[1],totPEres[2]);
    TGraph *nPE_r3res = new TGraph(CalibPosNum,positionR3[1],totPEres[3]);
    // TH1D *nPE_r30= new TH1D("nPE_r30","nPE_r30",20,0,6000);
    // TH1D *nPE_r31= new TH1D("nPE_r31","nPE_r31",20,0,6000);
    // for(int i=0;i<CalibPosNum;++i){
    //     int bin = int(positionR3[2][i]/300);
    //     nPE_r30->SetBinContent(bin+1,totPE[0][i]);
    //     nPE_r31->SetBinContent(bin+1,totPE[1][i]);
    // }
    c1->cd();
    nPE_r2res->SetLineColor(kRed);
    nPE_r1res->SetLineColor(kBlue);
    nPE_r3res->SetLineColor(kGreen);
    // nPE_r1res->GetYaxis()->SetRangeUser(1250,1600);
    nPE_r2res->Draw("AC*");
    nPE_r1res->Draw("SAME C*");
    nPE_r0res->Draw("SAME C*");
    nPE_r3res->Draw("SAME C*");

    TCanvas *c2 = new TCanvas("c2","c2",1600,900);
    c2->cd();
    nPE_r4->GetYaxis()->SetRangeUser(0,200);
    nPE_r4->Draw("AC*");
}
