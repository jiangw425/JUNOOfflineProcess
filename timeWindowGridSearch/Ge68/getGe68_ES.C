#include<vector>
const int eNum = 1;
const int cNum = 1;//3: QTMLE,QLH,TLH?
const bool enableUpdateBin = true;
double energyscale = 1.;
void getGe68_ES() {
    
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

    double EColor[10] = {1, 4, 6, 801, 417, 6, 3, 4};
    double EMarker[10] = {20, 21, 23, 22, 33, 23,20};
    // double P[eNum] = {0, 1., 2., 3., 4., 5., 6., 7., 8.};
    double P[eNum] = {0};
    int jw[eNum] = {0};
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

    string binFile = "Ge68_BinN_0.txt";
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

    // ofstream ResOut;
    // ResOut.open("kRes.txt",ios::out);
    ofstream ojw("Ge68_ES.txt");
    double jw_nl[eNum];
    for(int  ci=0;ci<cNum;ci++) {

        TString recdir = Form("./Ge68");
        for(int kNum=0;kNum<eNum;kNum++) {
            double ra = TrueEnergy[kNum]/2.;
            double rb = TrueEnergy[kNum]*2.;
            double binRange = rb - ra;
            cout<< binNum[0][ci][kNum] << '\t' << binNum[1][ci][kNum] << '\t' << binNum[2][ci][kNum] << endl;
            TH1F* h_E = new TH1F(Form("h_E_%0.2fMeV",TrueEnergy[kNum]),Form("E_{rec}_%0.2fMeV",TrueEnergy[kNum]), int(binNum[0][ci][kNum]), ra, rb);
            TFile* sfile = TFile::Open(recdir+Form("/Assemb.root"),"READ");
            std::cout<< recdir + Form("/Assemb.root") <<std::endl;


            TTree* stree = (TTree*)sfile->Get("evt");
            Float_t rec_x, rec_y, rec_z, n_fit, chi2, firedPMT, nPhotons, recR, edep;
        
            stree->SetBranchAddress("edep",&edep);
            bool checkQ = true;
            stree->SetBranchAddress("m_QTEn",&n_fit);
            stree->SetBranchAddress("recx",&rec_x);
            stree->SetBranchAddress("recy",&rec_y);
            stree->SetBranchAddress("recz",&rec_z);

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
            }

            h_E->Fit("gaus","Q");
            TF1* func_temp = (TF1*)h_E->GetFunction("gaus");
            double mean = func_temp->GetParameter(1);
            energyscale = 1.022/mean;
            Res[kNum] = 100*func_temp->GetParameter(2)/mean;
            ResErr[kNum] = 100*func_temp->GetParError(2)/mean;
            Mean[kNum] = mean;
            MeanErr[kNum] = func_temp->GetParError(1);
            Visb[kNum] = mean*energyscale;
            VisbErr[kNum] = MeanErr[kNum]*energyscale;

            if(enableUpdateBin) BinNOut0<< binRange/0.2/func_temp->GetParameter(2)<<endl;

            ojw << energyscale << endl;
            jw_nl[kNum] = mean/TrueEnergy[kNum];

            delete h_E;
            sfile->Close();
        }
        return;
    }
}
