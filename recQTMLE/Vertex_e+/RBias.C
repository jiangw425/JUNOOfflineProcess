#include<vector>
const int eNum = 5;
const int cNum = 1;
void RBias() {
    
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


    double EColor[10] = {6, 4, 8, 3, 6, 3, 3, 4};
    double EMarker[10] = {20,21,22, 23,20, 23,20};
    double P[eNum] = {0, 2., 5., 8., 10.};
    int jw[eNum] = {0,2,5,8,10};
    double TrueEnergy[eNum] = {};
    for(int i=0;i<eNum;i++) {
        TrueEnergy[i] = sqrt(P[i]*P[i]+0.511*0.511)+0.511;
    }
    double aBin[eNum] = {0.5, 1.5, 4.5, 7.5 , 8.5};
    double bBin[eNum] = {1.5, 3.5, 7.5, 11.5, 13.5};
    double binNum[3][eNum] = { {165, 195, 175, 173, 192}, {163, 191, 172, 170, 188}, {160, 185, 165, 164, 181}};
    double binNumV[3][eNum] = {{55, 86, 103, 112, 115}, {126, 234, 315, 348, 350}};

    std::vector<TGraphErrors*> vecGr;

    // ofstream ResOut;
    // ResOut.open("kRes000.txt",ios::out);
    
    for(int  ci=0;ci<cNum;ci++) {

    TString recdir = Form("../e+/");
    for(int kNum=0;kNum<eNum;kNum++) {
        TH1F* h_E = new TH1F(Form("h_E_%0.2fMeV",TrueEnergy[kNum]),Form("E_{rec}_%0.2fMeV",TrueEnergy[kNum]), binNum[0][kNum], aBin[kNum], bBin[kNum]);
        TH1F* h_dR = new TH1F(Form("h_R_%0.2fMeV",TrueEnergy[kNum]),Form("E_{rec}_%0.2fMeV",TrueEnergy[kNum]), binNumV[ci][kNum], -2000, 2000);
        TFile* sfile = TFile::Open(recdir+Form("%dMeV/Assemb.root",jw[kNum]),"READ"); 
        std::cout<< recdir + Form("%dMeV/Assemb.root",jw[kNum]) <<std::endl;

        TTree* stree = (TTree*)sfile->Get("evt");
        Float_t Coor_x, Coor_y, Coor_z, coor_x, coor_y, coor_z, TR, QR, TQR, RecR, n_fit, chi2, firedPMT, nPhotons;
        
        bool checkQ = false;
        stree->SetBranchAddress("TrueR",&TR); 
        // stree->SetBranchAddress("Coorx", &Coor_x);
        // stree->SetBranchAddress("Coory", &Coor_y);
        // stree->SetBranchAddress("Coorz", &Coor_z);

        // if(ci==0) {
        //     stree->SetBranchAddress("m_QR",&RecR);
        //     stree->SetBranchAddress("m_QEn",&n_fit);
        //     stree->SetBranchAddress("coorQx",&coor_x);
        //     stree->SetBranchAddress("coorQy",&coor_y);
        //     stree->SetBranchAddress("coorQz",&coor_z);
        // }


        // else if(ci==1){
            stree->SetBranchAddress("m_QTR",&RecR);
            stree->SetBranchAddress("m_QTEn",&n_fit);
            stree->SetBranchAddress("Truex",&coor_x);
            stree->SetBranchAddress("Truey",&coor_y);
            stree->SetBranchAddress("Truez",&coor_z);
        // }

        int Nentries = stree->GetEntries();
        for(int i=0;i<Nentries;i++) {
            stree->GetEntry(i);
            if(TMath::Power(coor_x*coor_x+coor_y*coor_y+coor_z*coor_z,0.5)/1E3 < 17.2) {
                h_dR->Fill(RecR - TR);
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


        h_dR->Fit("gaus","Q");
        TF1* func_temp_dR = (TF1*)h_dR->GetFunction("gaus");
        double mean_dR = func_temp_dR->GetParameter(1);
        VRes[kNum] = func_temp_dR->GetParameter(2);
        VResErr[kNum] = func_temp_dR->GetParError(2);
        VMean[kNum] = mean_dR;
        VMeanErr[kNum] = func_temp_dR->GetParError(1);

        //ResOut<<Visb[kNum]<<'\t'<<mean<<'\t'<<func_temp->GetParameter(2)/mean<<'\t'<<func_temp->GetParError(2)/mean<<endl;
        cout<<"---------------- mean -------------:"<<mean_dR<<'\t'<<func_temp_dR->GetParameter(2)<<'\t'<<4000./0.2/func_temp_dR->GetParameter(2)<<endl;
    }
     
    //TGraphErrors* grRM = new TGraphErrors(eNum, Visb, Res,VisbErr,ResErr);
    TGraphErrors* grRM = new TGraphErrors(eNum, Visb, VRes,VisbErr,VResErr);
    TGraphErrors* grRB = new TGraphErrors(eNum, Visb, VMean,VisbErr,VMeanErr);

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
    grRM->GetYaxis()->SetTitle("Z resolution [mm]");
  
    grRB->SetMarkerStyle(EMarker[ci]);
    grRB->SetMarkerColor(EColor[ci]);
    grRB->SetLineColor(EColor[ci]);
    grRB->SetTitle("");
    grRB->GetXaxis()->SetTitle("Reconstructed energy [MeV]");
    grRB->GetYaxis()->SetTitle("R bias [mm]");

    vecGr.push_back(grRB);

    }
    TCanvas* cRM = new TCanvas("cRM","Graph Example",200,10,700,500);
    cRM->SetGrid();
    for(int  ci=0;ci<cNum;ci++) {
        //if(ci==0||ci==2||ci==3||ci==4||ci==6) continue;
        if(ci==0) {
            vecGr[ci]->Draw("APL");
            //vecGr[ci]->GetYaxis()->SetRangeUser(0, 300);
        }
        else vecGr[ci]->Draw("PL SAME");
    }
    auto  legend = new TLegend(0.1,0.7,0.38,0.9);
    
    /*
    legend->AddEntry(vecGr[0],"PEMLE [#sigma_{SPE}=0.0pe]","l");
    legend->AddEntry(vecGr[1],"QMLE [#sigma_{SPE}=0.3pe]","l");
    legend->AddEntry(vecGr[2],"QMLE [Long-tail SPEs]","l");
    */
    TLegendEntry* l2 = legend->AddEntry(vecGr[0],"QTMLE","lp");
    l2->SetTextColor(EColor[0]);
    // TLegendEntry* l3 = legend->AddEntry(vecGr[1],"QTMLE","lp");
    // l3->SetTextColor(EColor[1]);
    
    
    legend->SetFillStyle(0);
    legend->SetTextSize(0.05);
    legend->SetFillColor(0);
    legend->Draw();
    /*
    TGraphErrors* grL = new TGraphErrors(1,LVisb,LRes,LVisbErr,LResErr);
    grL->Draw("SAME P");
    grL->SetMarkerStyle(20);
    grL->SetMarkerColor(kRed);
    
    TGraphErrors* grM = new TGraphErrors(1,MVisb,MRes,MVisbErr,MResErr);
    grM->Draw("SAME P");
    grM->SetMarkerStyle(20);
    grM->SetMarkerColor(kViolet);

    legend->AddEntry(grL,"Gaussian-Series SPEs","p");
    legend->AddEntry(grM,"W/ P(1|Q) = 90%","p");
    legend->SetTextSize(0.05);
    legend->Draw();
      */
    /*
    TCanvas* cmean = new TCanvas("cmean","Graph Example",200,10,700,500);
    cmean->SetGrid();
    TGraph* grmean = new TGraphErrors(eNum,Visb,Mean,VisbErr, MeanErr);
    grmean->Draw("APL");
    grmean->SetMarkerStyle(20);
    grmean->SetMarkerColor(kGreen);
    grmean->SetTitle("Reco-Energy vs. SPEs Resolution [E=1.022MeV]");
    grmean->GetXaxis()->SetTitle("SPEs Resolution [pe]");
    grmean->GetYaxis()->SetTitle("Reco-Energy");
    */
    // ResOut.close();
}
