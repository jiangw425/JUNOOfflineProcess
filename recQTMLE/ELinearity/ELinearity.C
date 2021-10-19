
const int parId = 1;
const int NPoint = 37;
const int CalibPosNum = 293;
const int ENum = 5;
const int PNum = 5;
void ELinearity()
{
    string particle[PNum] = {"Ge68", "Cs137", "Co60", "AmC", "AmC"};
    double ETrue[ENum] = {1.022, 0.6617, 2.50574, 2.223, 6.13};
    double EffE[ENum] = {0.511, 0.6617, 1.253, 2.223, 6.13};
    double EffEerr[ENum] = {0., 0., 0., 0., 0.};

    TFile* SpecDataFile = new TFile("SpecDataFile.root", "RECREATE");

    string posdir = "/junofs/users/huanggh/Data/ACU_CLS_3D_Pos";
    double positionX[CalibPosNum];
    double positionY[CalibPosNum];
    double positionZ[CalibPosNum];
    double Zpos[NPoint] = {};

    int posid = 0;
    std::ifstream cfile;
    cfile.open((posdir+"/ACU-CLS-GT_XYZPos.txt").c_str(), std::ios::in);
    for(int i=0;i<CalibPosNum;i++) {
        cfile>>posid;
        cfile>>positionX[i];
        cfile>>positionY[i];
        cfile>>positionZ[i];
        Zpos[i] = positionZ[i];
        std::cout<<positionX[i]<<'\t'<<positionZ[i]<<std::endl;
    }
    cfile.close();

    int arrangePosId[NPoint] = {39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};


    double ArraZpos[NPoint] = {};
    double ArraZposErr[NPoint] = {};
    

    double ErecQTEn[NPoint][ENum];
    double EsigQTEn[NPoint][ENum];
    double EresQTEn[NPoint][ENum];
    double ErecQTEnErr[NPoint][ENum];
    double EsigQTEnErr[NPoint][ENum];
    double EresQTEnErr[NPoint][ENum];


    double ErecQEn[NPoint][ENum];
    double EsigQEn[NPoint][ENum];
    double EresQEn[NPoint][ENum];
    double ErecQEnErr[NPoint][ENum];
    double EsigQEnErr[NPoint][ENum];
    double EresQEnErr[NPoint][ENum];

    
    for(int pid=0;pid<5;pid++) {
        cout<<" -------------- Processing particle -------------- :" << particle[pid] << endl;
        for(int posid=0; posid<NPoint; posid++) {
            std::cout<<posid<<std::endl;
            ArraZpos[posid] = Zpos[arrangePosId[posid]]/1.e3;
            ArraZposErr[posid] = 0.;   

            TFile* filetemp = TFile::Open(Form("../AssembFiles/%s/Assemb_QTMLE_%d.root", (particle[pid]).c_str(), arrangePosId[posid]));
            TH1D* hQTEnSpec = (TH1D*)filetemp->Get("hSignal_QTEn");
            if(pid==PNum-1) hQTEnSpec = (TH1D*)filetemp->Get("hBkg_QTEn");

            TFile* filetemp2 = TFile::Open(Form("../AssembFiles/%s/Assemb_QTMLE_%d.root", (particle[pid]).c_str(), arrangePosId[posid]));
            TH1D* hQEnSpec = (TH1D*)filetemp2->Get("hSignal_QEn");
            if(pid==PNum-1) hQEnSpec = (TH1D*)filetemp2->Get("hBkg_QEn");
            
            hQTEnSpec->SetName(Form("hQTEnSpec_%d", posid));
            if(pid<PNum-2) hQTEnSpec->Fit("gaus", "Q0");
            else if(pid<PNum-1) hQTEnSpec->Fit("gaus", "Q0R", "", 2., 2.9);
            else hQTEnSpec->Fit("gaus", "QR0", "", 6.4, 9.);
            TF1* fitGaus = (TF1*)hQTEnSpec->GetFunction("gaus");
            ErecQTEn[posid][pid] = fitGaus->GetParameter(1);
            ErecQTEnErr[posid][pid] = fitGaus->GetParError(1);

            EsigQTEn[posid][pid] = fitGaus->GetParameter(2);
            EsigQTEnErr[posid][pid] = fitGaus->GetParError(2);

            EresQTEn[posid][pid] = 100.*EsigQTEn[posid][pid]/ErecQTEn[posid][pid];
            EresQTEnErr[posid][pid] = EresQTEn[posid][pid]*sqrt(ErecQTEnErr[posid][pid]*ErecQTEnErr[posid][pid]/ErecQTEn[posid][pid]/ErecQTEn[posid][pid] + EsigQTEnErr[posid][pid]*EsigQTEnErr[posid][pid]/EsigQTEn[posid][pid]/EsigQTEn[posid][pid]);

            ErecQTEn[posid][pid] = ErecQTEn[posid][pid]/ETrue[pid];
            ErecQTEnErr[posid][pid] = ErecQTEnErr[posid][pid]/ETrue[pid];


            //////////////////////
            hQEnSpec->SetName(Form("hQEnSpec_%d", pid));
            if(pid<PNum-2) hQEnSpec->Fit("gaus", "Q0");
            else if(pid<PNum-1) hQEnSpec->Fit("gaus", "Q0R", "", 2., 2.9);
            else hQEnSpec->Fit("gaus", "QR0", "", 6.4, 9.);
            fitGaus = (TF1*)hQEnSpec->GetFunction("gaus");
            ErecQEn[posid][pid] = fitGaus->GetParameter(1);
            ErecQEnErr[posid][pid] = fitGaus->GetParError(1);

            EsigQEn[posid][pid] = fitGaus->GetParameter(2);
            EsigQEnErr[posid][pid] = fitGaus->GetParError(2);

            EresQEn[posid][pid] = 100.*EsigQEn[posid][pid]/ErecQEn[posid][pid];
            EresQEnErr[posid][pid] = EresQEn[posid][pid]*sqrt(ErecQEnErr[posid][pid]*ErecQEnErr[posid][pid]/ErecQEn[posid][pid]/ErecQEn[posid][pid] + EsigQEnErr[posid][pid]*EsigQEnErr[posid][pid]/EsigQEn[posid][pid]/EsigQEn[posid][pid]);

            ErecQEn[posid][pid] = ErecQEn[posid][pid]/ETrue[pid];
            ErecQEnErr[posid][pid] = ErecQEnErr[posid][pid]/ETrue[pid];



            cout<<arrangePosId[posid]<<'\t'<< ArraZpos[posid] << '\t'<<ErecQTEn[posid][pid]<<'\t'<<EsigQTEn[posid][pid]<<endl;
            cout<<arrangePosId[posid]<<'\t'<< ArraZpos[posid] << '\t'<<ErecQEn[posid][pid]<<'\t'<<EsigQEn[posid][pid]<<endl;

            SpecDataFile->cd();
            hQTEnSpec->Write();
            delete filetemp;
            delete filetemp2;

        }
    }
    
        
    double QTEnEref[NPoint];
    double QEnEref[NPoint];
    double QTEnErefErr[NPoint];
    double QEnErefErr[NPoint];

    double ErecQTEnRatio[NPoint][ENum];
    double ErecQTEnRatioErr[NPoint][ENum];
    double ErecQEnRatio[NPoint][ENum];
    double ErecQEnRatioErr[NPoint][ENum];
    
    TCanvas* cENL = new TCanvas("cENL", "cENL", 700, 500);
    auto  legend = new TLegend(0.17, 0.7, 0.37, 0.8);
    legend->SetTextSize(0.05);
    legend->SetFillStyle(0);
    for(int posid=0; posid<NPoint; posid++) {
        QTEnEref[posid] = ErecQTEn[posid][PNum-2];
        QEnEref[posid] = ErecQEn[posid][PNum-2];
        QTEnErefErr[posid] = ErecQTEnErr[posid][PNum-2];
        QEnErefErr[posid] = ErecQEnErr[posid][PNum-2];

        for(int pid=0;pid<PNum;pid++) {
            ErecQTEnRatio[posid][pid] = ErecQTEn[posid][pid]/QTEnEref[posid];
            ErecQTEnRatioErr[posid][pid] = ErecQTEnRatio[posid][pid]*sqrt(ErecQTEnErr[posid][pid]*ErecQTEnErr[posid][pid]/ErecQTEn[posid][pid]/ErecQTEn[posid][pid] + QTEnErefErr[posid]*QTEnErefErr[posid]/QTEnEref[posid]/QTEnEref[posid]);
            ErecQEnRatio[posid][pid] = ErecQEn[posid][pid]/QEnEref[posid];
            ErecQEnRatioErr[posid][pid] = ErecQEnRatio[posid][pid]*sqrt(ErecQEnErr[posid][pid]*ErecQEnErr[posid][pid]/ErecQEn[posid][pid]/ErecQEn[posid][pid] + QEnErefErr[posid]*QEnErefErr[posid]/QEnEref[posid]/QEnEref[posid]);
        }

        
        TGraphErrors* GrENL = new TGraphErrors(PNum, EffE, ErecQTEnRatio[posid], EffEerr, ErecQTEnRatioErr[posid]);
        GrENL->SetMarkerColor(posid+1);
        GrENL->SetLineColor(posid+1);
        if(TMath::Abs(ArraZpos[posid])<=1e-3) {
            GrENL->SetMarkerColor(1);
            GrENL->SetMarkerStyle(24);
            GrENL->SetMarkerSize(2.);
            TLegendEntry* l1 = legend->AddEntry(GrENL, "Center","p");
            l1->SetTextColor(1);

            ofstream dataout;
            dataout.open("NL_gh.txt", ios::out);
            for(int i=i;i<PNum;i++) {
                dataout<< EffE[i]<< '\t' << ErecQTEnRatio[posid][i] << endl;
            }
        }
        if(posid==20) {
            GrENL->SetMarkerColor(1);
            GrENL->SetMarkerStyle(24);
            TLegendEntry* l1 = legend->AddEntry(GrENL, "ACU","p");
            l1->SetTextColor(1);
        }

        else GrENL->SetMarkerStyle(24);
        GrENL->SetTitle("");
        GrENL->GetXaxis()->SetTitle("Gamma energy [MeV]");
        GrENL->GetYaxis()->SetTitle("E_{rec}/E_{true}");
        GrENL->GetYaxis()->SetTitleOffset(1.15);
        GrENL->GetYaxis()->SetRangeUser(0.9, 1.06);
        if(posid==0) GrENL->Draw("AP");
        else GrENL->Draw("SAME P");
    }
    TLatex* lt1 = new TLatex(0.8, 0.91, "^{68}Ge");
    TLatex* lt2 = new TLatex(0.5, 0.94, "^{137}Cs");
    TLatex* lt3 = new TLatex(1.1, 0.985, "^{60}Co");
    TLatex* lt4 = new TLatex(1.9, 1.01, "AmC-nH");
    TLatex* lt5 = new TLatex(4.5, 1.04, "AmC-^{16}O*");
    TLatex* lt = new TLatex(0.5, 1.04, "QTMLE");
    lt1->Draw("SAME");
    lt2->Draw("SAME");
    lt3->Draw("SAME");
    lt4->Draw("SAME");
    lt5->Draw("SAME");
    lt->Draw("SAME");

    legend->Draw("SAME");


    TCanvas* cEres = new TCanvas("cEres", "cEres", 700, 500);
    for(int posid=0; posid<1; posid++) {

        TGraphErrors* GrEres = new TGraphErrors(PNum, ErecQTEn[posid], EresQTEn[posid], ErecQTEnErr[posid], EresQTEnErr[posid]);

        if(posid==0) GrEres->SetMarkerStyle(20);
        else GrEres->SetMarkerStyle(24);
        GrEres->SetMarkerColor(posid+1);
        GrEres->SetLineColor(posid+1);
        GrEres->SetTitle("");
        GrEres->GetXaxis()->SetTitle("Gamma energy [MeV]");
        GrEres->GetYaxis()->SetTitle("E_{rec}/E_{true}");
        GrEres->GetYaxis()->SetTitleOffset(1.15);
        if(posid==0) GrEres->Draw("AP");
        else GrEres->Draw("SAME P");
    }


}
        

