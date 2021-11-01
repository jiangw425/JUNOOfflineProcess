#include<vector>
const int eNum = 5;
const int cNum = 2;
const bool enableUpdateBin = true;
std::string path[2] = {
    "/scratchfs/juno/jiangw/J21_result_new/00/e+/",
    "/scratchfs/juno/jiangw/J21_result_new/11/e+/"
};
void YResDiff() {
    gStyle->SetOptFit(0);
    
    double Visb[eNum] = {};
    double VisbErr[eNum] = {};
    double Res[eNum] = {};
    double ResErr[eNum] = {};
    double Mean[eNum] = {};
    double MeanErr[eNum] = {};
    double coef[eNum] = {};

    double VRes[eNum] = {};
    double VResErr[eNum] = {};
    double VMean[eNum] = {};
    double VMeanErr[eNum] = {};


    // double EColor[10] = {6, 4, 8, 1, 6, 3, 3, 4};
    double EColor[2] = {1, 2};
    double EMarker[10] = {20,21,22, 23,20, 23,20};
    //double P[eNum] = {0, 1., 2., 3., 4., 5., 6., 7., 8.};
    double P[eNum] = {0, 1., 3., 5., 8.};
    int jw[eNum] = {0, 1, 3, 5, 8};
    double TrueEnergy[eNum] = {};
    for(int i=0;i<eNum;i++) {
        TrueEnergy[i] = P[i] + 1.022;
    }
    double aBin[eNum] = {0.5, 0.5,  2.5,  4.5, 7.5};
    double bBin[eNum] = {1.5, 3.5,  5.5,  7.5, 10.5};
    double binNum[cNum][eNum];

    double binNumV[cNum][eNum];
    for(int j=0;j<cNum;j++) {
        for(int k=0;k<eNum;k++) {
            binNum[j][k]=200.;
            binNumV[j][k]=300.;
        }
    }
    string binFile = "BinN_1.txt";
    ifstream BinNIn1;
    BinNIn1.open(binFile.c_str(), ios::in);
    if(BinNIn1) {
        cout << "Reading " + binFile + " !" << endl;
        for(int i=0;i<cNum;i++) {
            for(int j=0;j<eNum;j++) {
                BinNIn1>>binNumV[i][j];
            }
        }
    }
    BinNIn1.close();
    ofstream BinNOut1;
    if(enableUpdateBin) BinNOut1.open(binFile.c_str(), ios::out);


    std::vector<TGraphErrors*> vecGr;

    ofstream ResOut;
    ResOut.open("kRes000.txt",ios::out);
    
    for(int  ci=0;ci<cNum;ci++) {

    TString recdir = Form("%s",path[ci].c_str());
    for(int kNum=0;kNum<eNum;kNum++) {
        TH1F* h_E = new TH1F(Form("h_E_%0.2fMeV",TrueEnergy[kNum]),Form("E_{rec}_%0.2fMeV",TrueEnergy[kNum]), binNum[0][kNum], aBin[kNum], bBin[kNum]);
        TH1F* h_dR = new TH1F(Form("h_R_%0.2fMeV",TrueEnergy[kNum]),Form("E_{rec}_%0.2fMeV",TrueEnergy[kNum]), int(binNumV[ci][kNum]), -2000, 2000);
        TFile* sfile = TFile::Open(recdir+Form("%dMeV/Assemb.root",jw[kNum]),"READ"); 
        std::cout<< recdir + Form("%dMeV/Assemb.root",jw[kNum]) <<std::endl;

        TTree* stree = (TTree*)sfile->Get("evt");
        Float_t True_x, True_y, True_z, rec_x, rec_y, rec_z, TrueR, TR, QR, TQR, RecR, n_fit, chi2, firedPMT, nPhotons;

        bool checkQ = false;
        stree->SetBranchAddress("TrueR",&TrueR);
        stree->SetBranchAddress("Truex", &True_x);
        stree->SetBranchAddress("Truey", &True_y);
        stree->SetBranchAddress("Truez", &True_z);

        if(ci==0) {
            stree->SetBranchAddress("m_QTR",&RecR);
            stree->SetBranchAddress("m_QTEn",&n_fit);
            stree->SetBranchAddress("recx",&rec_x);
            stree->SetBranchAddress("recy",&rec_y);
            stree->SetBranchAddress("recz",&rec_z);
        }
        else if(ci==1){
            stree->SetBranchAddress("m_QTR",&RecR);
            stree->SetBranchAddress("m_QTEn",&n_fit);
            stree->SetBranchAddress("recx",&rec_x);
            stree->SetBranchAddress("recy",&rec_y);
            stree->SetBranchAddress("recz",&rec_z);
        }
        else if(ci==2){
            stree->SetBranchAddress("m_QTR",&RecR);
            stree->SetBranchAddress("m_QTEn",&n_fit);
            stree->SetBranchAddress("recx",&rec_x);
            stree->SetBranchAddress("recy",&rec_y);
            stree->SetBranchAddress("recz",&rec_z);
        } 
        else if(ci==3) {
            stree->SetBranchAddress("m_QR",&RecR);
            stree->SetBranchAddress("m_QEn",&n_fit);
            stree->SetBranchAddress("recx",&rec_x);
            stree->SetBranchAddress("recy",&rec_y);
            stree->SetBranchAddress("recz",&rec_z);
        }
        int Nentries = stree->GetEntries();
        for(int i=0;i<Nentries;i++) {
            stree->GetEntry(i);
            //if(TrueR<17200 && TrueR > 15600) {
            if(TrueR<17200 ) {
                h_dR->Fill(rec_y - True_y);
                h_E->Fill(n_fit);
            }
        }

        h_E->Fit("gaus","Q");
        TF1* func_temp = (TF1*)h_E->GetFunction("gaus");
        double mean = func_temp->GetParameter(1);
        Res[kNum] = 100*func_temp->GetParameter(2)/mean;
        ResErr[kNum] = 100*func_temp->GetParError(2)/mean;
        Mean[kNum] = mean;
        MeanErr[kNum] = func_temp->GetParError(1);
        Visb[kNum] = mean*1.022/Mean[0];
        VisbErr[kNum] = MeanErr[kNum]*1.022/Mean[0];

        //Visb[kNum] = TrueEnergy[kNum];
        VisbErr[kNum] = 0.;

        h_dR->Fit("gaus","Q");
        TF1* func_temp_dR = (TF1*)h_dR->GetFunction("gaus");
        double mean_dR = func_temp_dR->GetParameter(1);
        VRes[kNum] = func_temp_dR->GetParameter(2);
        VResErr[kNum] = func_temp_dR->GetParError(2);
        VMean[kNum] = mean_dR;
        VMeanErr[kNum] = func_temp_dR->GetParError(1);

        //ResOut<<Visb[kNum]<<'\t'<<mean<<'\t'<<func_temp->GetParameter(2)/mean<<'\t'<<func_temp->GetParError(2)/mean<<endl;
        cout<<"---------------- mean -------------:"<<mean_dR<<'\t'<<func_temp_dR->GetParameter(2)<<'\t'<<4000./0.2/func_temp_dR->GetParameter(2)<<endl;
        if(enableUpdateBin) BinNOut1<< 4000./0.2/func_temp_dR->GetParameter(2)<<endl;
    }
     
    //TGraphErrors* grRM = new TGraphErrors(eNum, Visb, Res,VisbErr,ResErr);
    TGraphErrors* grRM = new TGraphErrors(eNum, Visb, VRes, VisbErr,VResErr);
    TGraphErrors* grRB = new TGraphErrors(eNum, Visb, VMean, VisbErr,VMeanErr);

    TF1* fun1= new TF1("fun1", "TMath::Sqrt([0]*[0]/x+[1]*[1])", 0.9, 13);
    fun1->SetParameter(0, 83);
    fun1->SetParameter(1, 30.);
    //fun1->SetParameter(2, 0.);

    //if(ci==1) fun1->FixParameter(0 , 83.57);
    fun1->SetParNames("A", "B");
    fun1->SetLineColor(EColor[ci]);
    grRM->Fit("fun1","QR");
 

    //grRM->Draw("AP");
    grRM->SetMarkerStyle(EMarker[ci]);
    grRM->SetMarkerColor(EColor[ci]);
    grRM->SetLineColor(EColor[ci]);
    grRM->SetTitle("");
    grRM->GetXaxis()->SetTitle("Reconstructed energy [MeV]");
    grRM->GetYaxis()->SetTitle("Y resolution [mm]");
  
    vecGr.push_back(grRM);

    }
    TCanvas* cRM = new TCanvas("cRM","Graph Example",200,10,700,500);
    cRM->SetGrid();
    
    for(int  ci=0;ci<cNum;ci++) {
        //if(ci==0||ci==2||ci==3||ci==4||ci==6) continue;
        if(ci==0) {
            vecGr[ci]->Draw("AP");
            vecGr[ci]->GetYaxis()->SetRangeUser(0, 120);
        }
        else vecGr[ci]->Draw("P SAME");
        /*    
        cRM->Update();
        auto stats1 = (TPaveStats*)vecGr[ci]->GetListOfFunctions()->FindObject("stats");
        stats1->SetTextColor(EColor[ci]);
        stats1->SetTextSize(0.045);
        if(ci<3) {stats1->SetX1NDC(1.0 - ci*0.28 - 0.28); stats1->SetX2NDC(1.0 - ci*0.28); stats1->SetY1NDC(0.66);stats1->SetY2NDC(0.91);}
        else {stats1->SetX1NDC(0.3); stats1->SetX2NDC(0.6); stats1->SetY1NDC(0.36);stats1->SetY2NDC(0.61);}
        stats1->SetFillStyle(0);
        */
        
    }
    auto  legend = new TLegend(0.6, 0.5, 0.88, 0.7);
    
    TLegendEntry* l1 = legend->AddEntry(vecGr[0],"e+(00)","lp");
    l1->SetTextColor(EColor[0]);
    TLegendEntry* l2 = legend->AddEntry(vecGr[1],"e+(11)","lp");
    l2->SetTextColor(EColor[1]);
    // TLegendEntry* l3 = legend->AddEntry(vecGr[2],"^{68}Ge+ true.e-","lp");
    // l3->SetTextColor(EColor[2]);
    //TLegendEntry* l4 = legend->AddEntry(vecGr[3], "TLH","lp");
    //l4->SetTextColor(EColor[3]);
    
    
    legend->SetFillStyle(0);
    legend->SetTextSize(0.05);
    legend->SetFillColor(0);
    legend->Draw();

    TText* restext = new TText(5.5, 100., "QTMLE (r < 17.2 m)");
    restext->SetTextSize(0.06);
    restext->Draw("SAME");


    ResOut.close();
}
