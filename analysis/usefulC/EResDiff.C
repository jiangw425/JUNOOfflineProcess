#include<vector>
const int eNum = 9;
const int cNum = 2;//3: QTMLE,QLH,TLH?
const bool enableUpdateBin = true;
std::string path[2] = {
    "/scratchfs/juno/jiangw/J21_result_new/00/e+/",
    "/scratchfs/juno/jiangw/J21_result_new/11/e+/"
};
void EResDiff() {
    
    double Visb[eNum] = {};
    double Mean[eNum] = {};
    double Res[eNum] = {};
    double VisbErr[eNum] = {};
    double MeanErr[eNum] = {};
    double ResErr[eNum] = {};

    double Visb1[eNum] = {};
    double Mean1[eNum] = {};
    double Res1[eNum] = {};
    double VisbErr1[eNum] = {};
    double MeanErr1[eNum] = {};
    double ResErr1[eNum] = {};

    double Visb2[eNum] = {};
    double Mean2[eNum] = {};
    double Res2[eNum] = {};
    double VisbErr2[eNum] = {};
    double MeanErr2[eNum] = {};
    double ResErr2[eNum] = {};

    double Visb3[eNum] = {};
    double Mean3[eNum] = {};
    double Res3[eNum] = {};
    double VisbErr3[eNum] = {};
    double MeanErr3[eNum] = {};
    double ResErr3[eNum] = {};

    // double EColor[10] = {1, 4, 6, 801, 417, 6, 3, 4};
    double EColor[2] = {1, 2};
    double EMarker[10] = {20, 21, 23, 22, 33, 23,20};
    double P[eNum] = {0, 1., 2., 3., 4., 5., 6., 7., 8.};
    double TrueEnergy[eNum] = {};
    for(int i=0;i<eNum;i++) {
        TrueEnergy[i] = P[i] + 1.022;
    }
    double binNum[4][cNum][eNum];
    for(int i=0;i<4;i++) {
        for(int j=0;j<cNum;j++) {
            for(int k=0;k<eNum;k++) {
                binNum[i][j][k]=200.;
            }
        }
    }
    std::vector<TGraphErrors*> vecGr;
    std::vector<TGraphErrors*> vecGr1;
    std::vector<TGraphErrors*> vecGr2;
    std::vector<TGraphErrors*> vecGr3;

    string binFile = "BinN_0.txt";
    ifstream BinNIn0;
    BinNIn0.open(binFile.c_str(), ios::in);
    if(BinNIn0) {
        cout << "Reading " + binFile + " !" << endl;
        for(int i=0;i<cNum;i++) {
            for(int j=0;j<eNum;j++) {
                BinNIn0>>binNum[0][i][j];
            }
        }
    }
    BinNIn0.close();
    ofstream BinNOut0;
    if(enableUpdateBin) BinNOut0.open(binFile.c_str(), ios::out);


    binFile = "BinN_1.txt";
    ifstream BinNIn1;
    BinNIn1.open(binFile.c_str(), ios::in);
    if(BinNIn1) {
        cout << "Reading " + binFile + " !" << endl;
        for(int i=0;i<cNum;i++) {
            for(int j=0;j<eNum;j++) {
                BinNIn1>>binNum[1][i][j];
            }
        }
    }
    BinNIn1.close();
    ofstream BinNOut1;
    if(enableUpdateBin) BinNOut1.open(binFile.c_str(), ios::out);
    
    binFile = "BinN_2.txt";
    ifstream BinNIn2;
    BinNIn2.open(binFile.c_str(), ios::in);
    if(BinNIn2) {
        cout << "Reading " + binFile + " !" << endl;
        for(int i=0;i<cNum;i++) {
            for(int j=0;j<eNum;j++) {
                BinNIn2>>binNum[2][i][j];
            }
        }
    }
    BinNIn2.close();
    ofstream BinNOut2;
    if(enableUpdateBin) BinNOut2.open(binFile.c_str(), ios::out);

    binFile = "BinN_3.txt";
    ifstream BinNIn3;
    BinNIn3.open(binFile.c_str(), ios::in);
    if(BinNIn3) {
        cout << "Reading " + binFile + " !" << endl;
        for(int i=0;i<cNum;i++) {
            for(int j=0;j<eNum;j++) {
                BinNIn3>>binNum[3][i][j];
            }
        }
    }
    BinNIn3.close();
    ofstream BinNOut3;
    if(enableUpdateBin) BinNOut3.open(binFile.c_str(), ios::out);


    ofstream ResOut;
    ResOut.open("kRes.txt",ios::out);
    
    for(int  ci=0;ci<cNum;ci++) {

        TString recdir = Form("%s",path[ci].c_str());
        for(int kNum=0;kNum<eNum;kNum++) {
            double ra = TrueEnergy[kNum]/2.;
            double rb = TrueEnergy[kNum]*2.;
            double binRange = rb - ra;
            cout<< binNum[0][ci][kNum] << '\t' << binNum[1][ci][kNum] << '\t' << binNum[2][ci][kNum] << endl;
            TH1F* h_E = new TH1F(Form("h_E_%0.2fMeV",TrueEnergy[kNum]),Form("E_{rec}_%0.2fMeV",TrueEnergy[kNum]), int(binNum[0][ci][kNum]), ra, rb);
            TH1F* h_E1 = new TH1F(Form("h_E1_%0.2fMeV",TrueEnergy[kNum]),Form("E1_{rec}_%0.2fMeV",TrueEnergy[kNum]), int(binNum[1][ci][kNum]), ra, rb);
            TH1F* h_E2 = new TH1F(Form("h_E2_%0.2fMeV",TrueEnergy[kNum]),Form("E2_{rec}_%0.2fMeV",TrueEnergy[kNum]), int(binNum[2][ci][kNum]), ra, rb);
            TH1F* h_E3 = new TH1F(Form("h_E3_%0.2fMeV",TrueEnergy[kNum]),Form("E3_{rec}_%0.2fMeV",TrueEnergy[kNum]), int(binNum[3][ci][kNum]), ra, rb);
            TFile* sfile = TFile::Open(recdir+Form("%dMeV/Assemb.root",kNum),"READ");
            std::cout<< recdir + Form("%dMeV/Assemb.root",kNum) <<std::endl;


            TTree* stree = (TTree*)sfile->Get("evt");
            Float_t rec_x, rec_y, rec_z, n_fit, chi2, firedPMT, nPhotons, recR, edep;
        
            stree->SetBranchAddress("edep",&edep);
            // bool checkQ = true;
            // if(ci==0&&checkQ) {
                stree->SetBranchAddress("m_QTEn",&n_fit);
                stree->SetBranchAddress("recx",&rec_x);
                stree->SetBranchAddress("recy",&rec_y);
                stree->SetBranchAddress("recz",&rec_z);
            // } else if(ci==0&&!checkQ) { //this is for QMLE, not for QTMLE, attention
                // stree->SetBranchAddress("m_QEn",&n_fit);
                // stree->SetBranchAddress("recQx",&rec_x);
                // stree->SetBranchAddress("recQy",&rec_y);
                // stree->SetBranchAddress("recQz",&rec_z);
                // stree->SetBranchAddress("m_QR",&recR);
            // } else if(ci<=1&&checkQ) {
            //     stree->SetBranchAddress("m_QEn",&n_fit);
            //     stree->SetBranchAddress("recQx",&rec_x);
            //     stree->SetBranchAddress("recQy",&rec_y);
            //     stree->SetBranchAddress("recQz",&rec_z);
            // } else if(ci<=2&&checkQ) {
            //     stree->SetBranchAddress("m_QEn",&n_fit);
            //     stree->SetBranchAddress("recx",&rec_x);
            //     stree->SetBranchAddress("recy",&rec_y);
            //     stree->SetBranchAddress("recz",&rec_z);
            //     stree->SetBranchAddress("m_QR",&recR);
            // }


            double R = 0.;
            int Nentries = stree->GetEntries();
            for(int i=0;i<Nentries;i++) {
                stree->GetEntry(i);
                double R = TMath::Power(rec_x*rec_x+rec_y*rec_y+rec_z*rec_z,0.5);
                double R3 = TMath::Power(rec_x*rec_x+rec_y*rec_y+rec_z*rec_z,1.5)/1.e9;
                if(edep < P[kNum]+1.022 - 1.e-4 || edep > P[kNum]+1.022 + 1.e-4) continue;
                if(R/1E3<17.2) {
                    h_E->Fill(n_fit);
                }
                if (R3 < 3800.) {
                    h_E1->Fill(n_fit);
                } else if (R3 < 17.2*17.2*17.2) {
                    h_E2->Fill(n_fit);
                } else if (R3 < 17.39*17.39*17.39) {
                    h_E3->Fill(n_fit);
                }
            }

            h_E->Fit("gaus","Q");
            TF1* func_temp = (TF1*)h_E->GetFunction("gaus");
            double mean = func_temp->GetParameter(1);
            Res[kNum] = 100*func_temp->GetParameter(2)/mean;
            ResErr[kNum] = 100*func_temp->GetParError(2)/mean;
            Mean[kNum] = mean;
            MeanErr[kNum] = func_temp->GetParError(1);
            //Visb[kNum] = mean*1.022/Mean[0];
            //VisbErr[kNum] = MeanErr[kNum]*1.022/Mean[0];
            
            // if(ci==0) {
                Visb[kNum] = mean*2.223/2.464;
                VisbErr[kNum] = MeanErr[kNum]*2.223/2.464;
            // } else if(ci==1) {
            //     Visb[kNum] = mean*2.223/2.46;
            //     VisbErr[kNum] = MeanErr[kNum]*2.223/2.46;
            // } else if(ci==2) {
            //     Visb[kNum] = mean*2.223/2.467;
            //     VisbErr[kNum] = MeanErr[kNum]*2.223/2.467;
            // }
            if(enableUpdateBin) BinNOut0<< binRange/0.2/func_temp->GetParameter(2)<<endl;

            h_E1->Fit("gaus","Q");
            func_temp = (TF1*)h_E1->GetFunction("gaus");
            double mean1 = func_temp->GetParameter(1);
            Res1[kNum] = 100*func_temp->GetParameter(2)/mean1;
            ResErr1[kNum] = 100*func_temp->GetParError(2)/mean1;
            Mean1[kNum] = mean1;
            MeanErr1[kNum] = func_temp->GetParError(1);
            Visb1[kNum] = mean1*1.022/Mean1[0];
            VisbErr1[kNum] = MeanErr1[kNum]*1.022/Mean1[0];
            if(enableUpdateBin) BinNOut1<< binRange/0.2/func_temp->GetParameter(2)<<endl;

            h_E2->Fit("gaus","Q");
            func_temp = (TF1*)h_E2->GetFunction("gaus");
            double mean2 = func_temp->GetParameter(1);
            Res2[kNum] = 100*func_temp->GetParameter(2)/mean2;
            ResErr2[kNum] = 100*func_temp->GetParError(2)/mean2;
            Mean2[kNum] = mean2;
            MeanErr2[kNum] = func_temp->GetParError(1);
            Visb2[kNum] = mean2*1.022/Mean2[0];
            VisbErr2[kNum] = MeanErr2[kNum]*1.022/Mean2[0];
            if(enableUpdateBin) BinNOut2<< binRange/0.2/func_temp->GetParameter(2)<<endl;

            if(kNum==0) h_E3->Fit("gaus","QR", "", 0.6, 1.5);
            else h_E3->Fit("gaus","Q");
            func_temp = (TF1*)h_E3->GetFunction("gaus");
            double mean3 = func_temp->GetParameter(1);
            Res3[kNum] = 100*func_temp->GetParameter(2)/mean3;
            ResErr3[kNum] = 100*func_temp->GetParError(2)/mean3;
            Mean3[kNum] = mean3;
            MeanErr3[kNum] = func_temp->GetParError(1);
            Visb3[kNum] = mean3*1.022/Mean3[0];
            VisbErr3[kNum] = MeanErr3[kNum]*1.022/Mean3[0];
            if(enableUpdateBin) BinNOut3<< binRange/0.2/func_temp->GetParameter(2)<<endl;

            //Visb[kNum] = TrueEnergy[kNum];
            //VisbErr[kNum] = MeanErr[kNum];

            cout<<"---------- mean ---------:"<<mean<<'\t'<<func_temp->GetParameter(2)<<'\t'<<binRange/0.2/func_temp->GetParameter(2)<<endl;
            cout<<"---------- Res ----------:"<<Res[kNum] << '\t' << ResErr[kNum] << std::endl;
            ResOut<< Res[kNum] << '\t' << ResErr[kNum] << std::endl;

            delete h_E;
            delete h_E1;
            delete h_E2;
            delete h_E3;
            sfile->Close();
        }
        
        std::cout << std::endl;
        TGraphErrors* grRes = new TGraphErrors(eNum, Visb, Res, VisbErr, ResErr);
        TF1* resfun= new TF1("resfun", "TMath::Sqrt([0]*[0]/x+[1]*[1]+[2]*[2]/x/x)", 0.5, 13);
        resfun->SetParNames("a", "b", "c");
        resfun->SetParameter(0, 2.9);
        resfun->SetParameter(1, 0.7);
        resfun->SetParameter(2, 1.);

        resfun->SetLineColor(EColor[ci]);
        grRes->Fit("resfun","QR");

        double a = resfun->GetParameter(0);
        double b = resfun->GetParameter(1);
        double c = resfun->GetParameter(2);
        std::cout<< "CD <17.2m      Energy resolution at 1 MeV:\t" << sqrt(a*a + b*b + c*c) << '\t' << sqrt(a*a + b*b*1.6*1.6 + c*c/1.6/1.6) << std::endl;

        grRes->SetMarkerStyle(EMarker[ci]);
        grRes->SetMarkerColor(EColor[ci]);
        grRes->SetLineColor(EColor[ci]);
        grRes->SetTitle("");
        grRes->GetXaxis()->SetTitle("E_{rec} [MeV]");
        grRes->GetYaxis()->SetTitle("Energy resolution [%]");
        vecGr.push_back(grRes);


        TGraphErrors* grRes1 = new TGraphErrors(eNum, Visb1, Res1, VisbErr1, ResErr1);
        TF1* resfun1= new TF1("resfun1", "TMath::Sqrt([0]*[0]/x+[1]*[1]+[2]*[2]/x/x)", 0.5, 13);
        resfun1->SetParNames("a", "b", "c");
        resfun1->SetParameter(0, 2.9);
        resfun1->SetParameter(1, 0.7);
        resfun1->SetParameter(2, 1.);
        resfun1->SetLineColor(EColor[ci]);
        grRes1->Fit("resfun1","QR");
        a = resfun1->GetParameter(0);
        b = resfun1->GetParameter(1);
        c = resfun1->GetParameter(2);
        std::cout<< "CD <15.6m      Energy resolution at 1 MeV:\t" << sqrt(a*a + b*b + c*c) << '\t' << sqrt(a*a + b*b*1.6*1.6 + c*c/1.6/1.6) << std::endl;
        grRes1->SetMarkerStyle(EMarker[ci]);
        grRes1->SetMarkerColor(EColor[ci]);
        grRes1->SetLineColor(EColor[ci]);
        grRes1->SetTitle("");
        grRes1->GetXaxis()->SetTitle("E_{rec} [MeV]");
        grRes1->GetYaxis()->SetTitle("Energy resolution [%]");
        vecGr1.push_back(grRes1);

        TGraphErrors* grRes2 = new TGraphErrors(eNum, Visb2, Res2, VisbErr2, ResErr2);
        TF1* resfun2= new TF1("resfun2", "TMath::Sqrt([0]*[0]/x+[1]*[1]+[2]*[2]/x/x)", 0.5, 13);
        resfun2->SetParNames("a", "b", "c");
        resfun2->SetParameter(0, 2.9);
        resfun2->SetParameter(1, 0.7);
        resfun2->SetParameter(2, 1.);
        resfun2->SetLineColor(EColor[ci]);
        grRes2->Fit("resfun2","QR");
        a = resfun2->GetParameter(0);
        b = resfun2->GetParameter(1);
        c = resfun2->GetParameter(2);
        std::cout<< "CD 15.6~17.2m  Energy resolution at 1 MeV:\t" << sqrt(a*a + b*b + c*c) << '\t' << sqrt(a*a + b*b*1.6*1.6 + c*c/1.6/1.6) << std::endl;
        grRes2->SetMarkerStyle(EMarker[ci]);
        grRes2->SetMarkerColor(EColor[ci]);
        grRes2->SetLineColor(EColor[ci]);
        grRes2->SetTitle("");
        grRes2->GetXaxis()->SetTitle("E_{rec} [MeV]");
        grRes2->GetYaxis()->SetTitle("Energy resolution [%]");
        vecGr2.push_back(grRes2);

        TGraphErrors* grRes3 = new TGraphErrors(eNum, Visb3, Res3, VisbErr3, ResErr3);
        TF1* resfun3= new TF1("resfun3", "TMath::Sqrt([0]*[0]/x+[1]*[1]+[2]*[2]/x/x)", 0.5, 13);
        resfun3->SetParNames("a", "b", "c");
        resfun3->SetParameter(0, 2.9);
        resfun3->SetParameter(1, 0.7);
        resfun3->SetParameter(2, 1.);
        resfun3->SetLineColor(EColor[ci]);
        grRes3->Fit("resfun3","QR");
        a = resfun3->GetParameter(0);
        b = resfun3->GetParameter(1);
        c = resfun3->GetParameter(2);
        std::cout<< "CD 17.2~17.39m Energy resolution at 1 MeV:\t" << sqrt(a*a + b*b + c*c) << '\t' << sqrt(a*a + b*b*1.6*1.6 + c*c/1.6/1.6) << std::endl;
        grRes3->SetMarkerStyle(EMarker[ci]);
        grRes3->SetMarkerColor(EColor[ci]);
        grRes3->SetLineColor(EColor[ci]);
        grRes3->SetTitle("");
        grRes3->GetXaxis()->SetTitle("E_{rec} [MeV]");
        grRes3->GetYaxis()->SetTitle("Energy resolution [%]");
        vecGr3.push_back(grRes3);

        std::cout << std::endl;
    }
    BinNOut1.close();
    BinNOut2.close();
    BinNOut3.close();
    ResOut.close();

    double topMargin = 0.0;
    double stW = 0.26;

    TCanvas* cR0 = new TCanvas("cR0","cR0", 700, 500);
    for(int  ci=0;ci<cNum;ci++) {
        if(ci==0) {
            vecGr[ci]->Draw("AP");
            vecGr[ci]->GetYaxis()->SetRangeUser(0.9, 3.5);
        }
        else vecGr[ci]->Draw("P SAME");

        cR0->Update();
        auto stats1 = (TPaveStats*)vecGr[ci]->GetListOfFunctions()->FindObject("stats");
        if(!stats1) {cout <<"wow!null pointer!";return;}
        stats1->SetTextColor(EColor[ci]);
        stats1->SetTextSize(0.04);
        if(ci<3) {stats1->SetX1NDC(1.0 - ci*stW - stW); stats1->SetX2NDC(1.0 - ci*stW); stats1->SetY1NDC(0.66*(1. - topMargin + 0.05));stats1->SetY2NDC(0.91*(1.-topMargin + 0.05));}
        else {stats1->SetX1NDC(1.0 - (ci-3)*stW - stW); stats1->SetX2NDC(1.0 - (ci-3)*stW); stats1->SetY1NDC(0.4*(1. - topMargin + 0.05));stats1->SetY2NDC(0.65*(1. - topMargin + 0.05));}
        stats1->SetFillStyle(0);
    }
    cR0->Update();
 

    double m_leftm = 0.16;
    double pW = 1./(1./(1. - m_leftm)  + 1. + 1./(1. - m_leftm));
    stW = 0.32;

    TCanvas* cR = new TCanvas("cR","cR",1500, 500);
    TPad* c1 = new TPad("c1", "c1", 0.0, 0.01, pW/(1. - m_leftm), 0.99);
    TPad* c2 = new TPad("c2", "c2", pW/(1. - m_leftm), 0.01, pW/(1. - m_leftm) + pW, 0.99);
    TPad* c3 = new TPad("c3", "c3", pW/(1. - m_leftm) + pW, 0.01, 1., 0.99);

    c1->SetLeftMargin(m_leftm);
    c1->SetRightMargin(0.);
    c2->SetLeftMargin(0.);
    c2->SetRightMargin(0.);
    c3->SetLeftMargin(0.);
    c3->SetRightMargin(m_leftm);


    c1->Draw();
    c2->Draw();
    c3->Draw();
    //cR->SetTopMargin(topMargin);
    //cR->SetGrid();
    c1->cd();
    for(int  ci=0;ci<cNum;ci++) {
        if(ci==0) {
            vecGr1[ci]->Draw("AP");
            vecGr1[ci]->GetYaxis()->SetRangeUser(0.9, 3.5);
        }
        else vecGr1[ci]->Draw("P SAME");
        
        c1->Update();
        auto stats1 = (TPaveStats*)vecGr1[ci]->GetListOfFunctions()->FindObject("stats");
        stats1->SetTextColor(EColor[ci]);
        stats1->SetTextSize(0.04);
        if(ci<3) {stats1->SetX1NDC(1.0 - ci*stW - stW); stats1->SetX2NDC(1.0 - ci*stW); stats1->SetY1NDC(0.66*(1. - topMargin + 0.05));stats1->SetY2NDC(0.91*(1.-topMargin + 0.05));}
        else {stats1->SetX1NDC(1.0 - (ci-3)*stW - stW); stats1->SetX2NDC(1.0 - (ci-3)*stW); stats1->SetY1NDC(0.4*(1. - topMargin + 0.05));stats1->SetY2NDC(0.65*(1. - topMargin + 0.05));}
        stats1->SetFillStyle(0);
    }
    c1->Update();

    c2->cd();
    for(int  ci=0;ci<cNum;ci++) {
        if(ci==0) {
            vecGr2[ci]->Draw("AP");
            vecGr2[ci]->GetYaxis()->SetRangeUser(0.9, 3.5);
        }
        else vecGr2[ci]->Draw("P SAME");

        c2->Update();
        auto stats1 = (TPaveStats*)vecGr2[ci]->GetListOfFunctions()->FindObject("stats");
        stats1->SetTextColor(EColor[ci]);
        stats1->SetTextSize(0.04);
        if(ci<3) {stats1->SetX1NDC(1.0 - ci*stW - stW); stats1->SetX2NDC(1.0 - ci*stW); stats1->SetY1NDC(0.66*(1. - topMargin + 0.05));stats1->SetY2NDC(0.91*(1.-topMargin + 0.05));}
        else {stats1->SetX1NDC(1.0 - (ci-3)*stW - stW); stats1->SetX2NDC(1.0 - (ci-3)*stW); stats1->SetY1NDC(0.4*(1. - topMargin + 0.05));stats1->SetY2NDC(0.65*(1. - topMargin + 0.05));}
        stats1->SetFillStyle(0);
    }
    c2->Update();

    c3->cd();
    for(int  ci=0;ci<cNum;ci++) {
        if(ci==0) {
            vecGr3[ci]->Draw("AP");
            vecGr3[ci]->GetYaxis()->SetRangeUser(0.9, 3.5);
        }
        else vecGr3[ci]->Draw("P SAME");

        c3->Update();
        auto stats1 = (TPaveStats*)vecGr3[ci]->GetListOfFunctions()->FindObject("stats");
        stats1->SetTextColor(EColor[ci]);
        stats1->SetTextSize(0.04);
        if(ci<3) {stats1->SetX1NDC(1.0 - ci*stW - stW); stats1->SetX2NDC(1.0 - ci*stW); stats1->SetY1NDC(0.66*(1. - topMargin + 0.05));stats1->SetY2NDC(0.91*(1.-topMargin + 0.05));}
        else {stats1->SetX1NDC(1.0 - (ci-3)*stW - stW); stats1->SetX2NDC(1.0 - (ci-3)*stW); stats1->SetY1NDC(0.4*(1. - topMargin + 0.05));stats1->SetY2NDC(0.65*(1. - topMargin + 0.05));}
        stats1->SetFillStyle(0);
    }
    c3->Update();   


    auto  legend = new TLegend(0.6,0.4,0.88,0.6);
    
    TLegendEntry* l1 = legend->AddEntry(vecGr1[0],"QTMLE00","lp");
    l1->SetTextColor(EColor[0]);
    TLegendEntry* l2 = legend->AddEntry(vecGr1[1],"QTMLE11","lp");
    l2->SetTextColor(EColor[1]);
    // TLegendEntry* l3 = legend->AddEntry(vecGr2[2],"TLH+QMLE","lp");
    // l3->SetTextColor(EColor[2]);

    legend->SetFillStyle(0);
    legend->SetTextSize(0.05);
    legend->SetFillColor(0);
    legend->Draw("SAME");

    cR0->cd();
    legend->Draw("SAME");   
    
    
}
