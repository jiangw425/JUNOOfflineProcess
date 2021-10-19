
const int parId = 3;
const int NPoint = 293;
const int CalibPosNum = 293;
void EResCompare_Ge68()
{
    string particle[4] = {"Co60", "Cs137", "Ge68", "AmC"};
    TFile* SpecDataFile = new TFile("SpecDataFile.root", "RECREATE");

    string posdir = "/junofs/users/huanggh/Data/ACU_CLS_3D_Pos";
    double positionX[CalibPosNum];
    double positionY[CalibPosNum];
    double positionZ[CalibPosNum];

    int posidtemp = 0;
    std::ifstream cfile;
    cfile.open((posdir+"/ACU-CLS-GT_XYZPos.txt").c_str(), std::ios::in);
    for(int i=0;i<CalibPosNum;i++) {
        cfile>>posidtemp;
        cfile>>positionX[i];
        cfile>>positionY[i];
        cfile>>positionZ[i];
        std::cout<<positionX[i]<<'\t'<<positionY[i]<<'\t'<<positionZ[i]<<std::endl;
    }
    cfile.close();

    vector<double>    ArraZpos;
    vector<double> ArraZposErr;

    vector<double>    ErecQTEn;
    vector<double> ErecQTEnErr;
    vector<double>    EsigQTEn;
    vector<double> EsigQTEnErr;
    vector<double>    EresQTEn;
    vector<double> EresQTEnErr;

    vector<double>     ErecQEn;
    vector<double>  ErecQEnErr;
    vector<double>     EsigQEn;
    vector<double>  EsigQEnErr;
    vector<double>     EresQEn;
    vector<double>  EresQEnErr;

    for(int posid=0; posid<NPoint; posid++) {
        double R = sqrt(positionX[posid]*positionX[posid] + positionY[posid]*positionY[posid] + positionZ[posid]*positionZ[posid]);
        double R3 = R*R*R/1.e9;


        TFile* filetemp = TFile::Open(Form("../AssembFiles/%s/Assemb_QTMLE_%d.root", (particle[parId-1]).c_str(), posid));
        TH1D* hQTEnSpec = (TH1D*)filetemp->Get("hNPE");

        TFile* filetemp2 = TFile::Open(Form("../AssembFiles/%s/Assemb_QTMLE_%d.root", (particle[parId-1]).c_str(), posid));
        TH1D* hQEnSpec = (TH1D*)filetemp2->Get("hNPE");

        
        if(hQTEnSpec->GetEntries()<100) continue;
        ArraZpos.push_back(R3);
        ArraZposErr.push_back(0.);   

        hQTEnSpec->SetName(Form("hQTEnSpec_%d", posid));
        hQTEnSpec->Fit("gaus", "Q0");
        TF1* fitGaus = (TF1*)hQTEnSpec->GetFunction("gaus");
        double Erec = fitGaus->GetParameter(1);
        double ErecErr =  fitGaus->GetParError(1);
        ErecQTEn.push_back(Erec);
        ErecQTEnErr.push_back(ErecErr);

        double Esig = fitGaus->GetParameter(2);
        double EsigErr = fitGaus->GetParError(2);
        EsigQTEn.push_back(Esig);
        EsigQTEnErr.push_back(EsigErr);

        double Eres = 100.*Esig/Erec;
        double EresErr = Eres*sqrt(ErecErr*ErecErr/Erec/Erec + EsigErr*EsigErr/Esig/Esig);
        EresQTEn.push_back(Eres);
        EresQTEnErr.push_back(EresErr);

        cout<<posid << '\t'<< R << '\t' << R3 << "\tEres: "<<Eres<<"\tErec: "<<Erec<<endl;

        //////////////////////
        hQEnSpec->SetName(Form("hQEnSpec_%d", posid));
        hQEnSpec->Fit("gaus", "Q0");
        fitGaus = (TF1*)hQEnSpec->GetFunction("gaus");
        Erec = fitGaus->GetParameter(1);
        ErecErr =  fitGaus->GetParError(1);
        ErecQEn.push_back(Erec);
        ErecQEnErr.push_back(ErecErr);;

        Esig = fitGaus->GetParameter(2);
        EsigErr = fitGaus->GetParError(2);
        EsigQEn.push_back(Esig);
        EsigQEnErr.push_back(EsigErr);

        Eres = 100.*Esig/Erec;
        EresErr = Eres*sqrt(ErecErr*ErecErr/Erec/Erec + EsigErr*EsigErr/Esig/Esig);
        EresQEn.push_back(Eres);
        EresQEnErr.push_back(EresErr);



        SpecDataFile->cd();
        hQTEnSpec->Write();
        delete filetemp;
        delete filetemp2;

    }
    double ErangA = 0.97;
    double ErangB = 4;//1.05;
    double rangA = 2.8;
    double rangB = 3.9;

    double dmx[2] = {-17.7*17.7*17.7, 17.7*17.7*17.7};
    double dmy[2] = {0., 10.};
    TGraph* grEDummy = new TGraph(2, dmx, dmy);
    grEDummy->SetTitle("");
    grEDummy->GetXaxis()->SetTitle("r^{3} [m^{3}]");
    grEDummy->GetYaxis()->SetTitle("E_{rec} [MeV]");
    grEDummy->GetYaxis()->SetTitleOffset(1.2);
    grEDummy->GetXaxis()->SetRangeUser(-17.7*17.7*17.7, 17.7*17.7*17.7);
    grEDummy->GetYaxis()->SetRangeUser(ErangA, ErangB);

    
    TBox* Ebox0= new TBox(-17.7*17.7*17.7, ErangA, -17.2*17.2*17.2, ErangB);
    Ebox0->SetFillColor(kPink);
    Ebox0->SetFillStyle(3003);

    TBox* Ebox1= new TBox(17.2*17.2*17.2, ErangA, 17.7*17.7*17.7, ErangB);
    Ebox1->SetFillColor(kPink);
    Ebox1->SetFillStyle(3003);

    TGraph* grDummy = new TGraph(2, dmx, dmy);
    grDummy->SetTitle("");
    grDummy->GetXaxis()->SetTitle("r^{3} [m^{3}]");
    grDummy->GetYaxis()->SetTitle("E_{res} [%]");
    grDummy->GetYaxis()->SetTitleOffset(1.2);
    grDummy->GetXaxis()->SetRangeUser(-17.7*17.7*17.7, 17.7*17.7*17.7);
    grDummy->GetYaxis()->SetRangeUser(rangA, rangB);
 
    TBox* box0= new TBox(-17.7*17.7*17.7, rangA, -17.2*17.2*17.2, rangB);
    box0->SetFillColor(kPink);
    box0->SetFillStyle(3003);

    TBox* box1= new TBox(17.2*17.2*17.2, rangA, 17.7*17.7*17.7, rangB);
    box1->SetFillColor(kPink);
    box1->SetFillStyle(3003);

    TCanvas* cErecQTEn = new TCanvas("cErecQTEn", "cErecQTEn", 700, 500);
    grEDummy->Draw("AP");
    Ebox0->Draw("SAME");
    Ebox1->Draw("SAME");

    TGraphErrors* GrErecQEn = new TGraphErrors(ArraZpos.size(), &ArraZpos[0], &ErecQEn[0], &ArraZposErr[0], &ErecQEnErr[0]);
    GrErecQEn->SetMarkerStyle(20);
    GrErecQEn->SetMarkerColor(1);
    GrErecQEn->SetLineColor(1);
    GrErecQEn->SetTitle("");
    GrErecQEn->GetXaxis()->SetTitle("r^{3} [m^{3}]");
    GrErecQEn->GetYaxis()->SetTitle("E_{rec} [MeV]");
    GrErecQEn->GetYaxis()->SetTitleOffset(1.15);
    //GrErecQEn->Draw("SAME P");
    TGraphErrors* GrErecQTEn = new TGraphErrors(ArraZpos.size(), &ArraZpos[0], &ErecQTEn[0], &ArraZposErr[0], &ErecQTEnErr[0]);
    GrErecQTEn->SetMarkerStyle(22);
    GrErecQTEn->SetMarkerColor(4);
    GrErecQTEn->SetLineColor(4);
    GrErecQTEn->Draw("SAME P");

    auto  legend = new TLegend(0.2,0.2,0.45,0.4);
    //TLegendEntry* l1 = legend->AddEntry(GrErecQEn, "TLH","lp");
    //l1->SetTextColor(1);
    TLegendEntry* l2 = legend->AddEntry(GrErecQTEn,"QTMLE","lp");
    l2->SetTextColor(4); 

    legend->SetFillStyle(0);
    legend->SetTextSize(0.05);
    legend->SetFillColor(0);
    legend->Draw();


    TCanvas* cEresQTEn = new TCanvas("cEresQTEn", "cEresQTEn", 700, 500);
    grDummy->Draw("AP");
    grDummy->GetYaxis()->SetRangeUser(rangA, rangB);
    box0->Draw("SAME");
    box1->Draw("SAME");

    TGraphErrors* GrEresQEn = new TGraphErrors(ArraZpos.size(), &ArraZpos[0], &EresQEn[0], &ArraZposErr[0], &EresQEnErr[0]);
    GrEresQEn->SetMarkerStyle(20);
    GrEresQEn->SetMarkerColor(1);
    GrEresQEn->SetLineColor(1);
    GrEresQEn->SetTitle("");
    GrEresQEn->GetXaxis()->SetTitle("r^{3} [m^{3}]");
    GrEresQEn->GetYaxis()->SetTitle("E_{res} [%]");
    GrEresQEn->GetYaxis()->SetRangeUser(rangA, rangB);
    //GrEresQEn->Draw("SAME P");
    TGraphErrors* GrEresQTEn = new TGraphErrors(ArraZpos.size(), &ArraZpos[0], &EresQTEn[0], &ArraZposErr[0], &EresQTEnErr[0]);
    GrEresQTEn->SetMarkerStyle(22);
    GrEresQTEn->SetMarkerColor(4);
    GrEresQTEn->SetLineColor(4);
    GrEresQTEn->Draw("SAME P");

    

}
        

