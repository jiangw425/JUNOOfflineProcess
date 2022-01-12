
//#define CheckDraw2D

const int CalibPosNum = 277;
const int ThetaPMTNum = 1440;
const int RNum = 1800 + 1;
const int ThetaNum = 18 + 1;
const int GridNum = RNum*ThetaNum;

//const double NormF=180./(17400.^3);

void Collect_R1()
{

    double EvtTheta[GridNum];
    double EvtR[GridNum];


    double LMu2D[GridNum];
    double SMu2D[GridNum];

    
    std::ifstream cfile;
    cfile.open("gridr.txt", std::ios::in);
    for(int g=0;g<GridNum;g++) {
        cfile>>EvtR[g];
        //EvtR[g] *= 100.;
    }
    cfile.close();

 
    cfile.open("gridtheta.txt", std::ios::in);
    double temp = 0.;
    for(int g=0;g<GridNum;g++) {
        cfile>>temp;
        EvtTheta[g] = temp*TMath::Pi()/180.;
    }
    cfile.close();


    int RBinN = 1800 + 1;
    int TBinN = 18 + 1;
    int PTBinN = 1440;

    double RRange = 180.;
    double TRange = TMath::Pi();
    double PTRange = 180.;
    double rstep = RRange/double(RBinN-1.);
    double tstep = TRange/double(TBinN-1.);
    double ptstep = PTRange/double(PTBinN);

    double rbins[RBinN];
    double thetabins[RBinN];
    for(int i=0;i<RBinN;i++) {
        rbins[i] = rstep*double(i);
    }
    for(int i=0;i<TBinN;i++) {
        thetabins[i] = tstep*double(i);
    }
 
    TFile* LnPEMapFile = new TFile("LnPEMapFile_R1.root", "RECREATE");
    TFile* SnPEMapFile = new TFile("SnPEMapFile_R1.root", "RECREATE");
    
    std::ifstream lfile; 
    lfile.open("lgridmu_SOURCE_R1.txt", std::ios::in);
    std::ifstream sfile; 
    sfile.open("lgridmu_SOURCE_R1.txt", std::ios::in);
    for(int ipt=0;ipt<ThetaPMTNum;ipt++) {
        cout<<ipt<<endl;
        TH2F* hLMu2D = new TH2F(Form("hLMu2D_%d", ipt), Form("hLMu2D_%d", ipt), RBinN, -rstep/2., RRange + rstep/2., TBinN, -tstep/2., TRange + tstep/2.);
        TH2F* hSMu2D = new TH2F(Form("hSMu2D_%d", ipt), Form("hSMu2D_%d", ipt), RBinN, -rstep/2., RRange + rstep/2., TBinN, -tstep/2., TRange + tstep/2.);
        hLMu2D ->SetTitle(Form("#theta_{PMT} = %d#circ", int(ipt*ptstep)));
        hLMu2D->GetXaxis()->SetTitle("r [m]");
        hLMu2D->GetYaxis()->SetTitle("#theta [rad]");
        hLMu2D->GetZaxis()->SetTitle("#mu_{0} [p.e.]");
        hLMu2D->GetXaxis()->SetTitleOffset(1.3);
        hLMu2D->GetYaxis()->SetTitleOffset(1.3);
        hLMu2D->GetZaxis()->SetTitleOffset(1.2);


        for(int g=0;g<GridNum;g++) {
            if(g<RNum*ThetaNum) {
                lfile>>LMu2D[g];
                sfile>>SMu2D[g];
                int rbin = hLMu2D->GetXaxis()->FindBin(EvtR[g]) ;
                int tbin = hLMu2D->GetYaxis()->FindBin(EvtTheta[g]) ;

                hLMu2D->SetBinContent(rbin, tbin, LMu2D[g]);
                hSMu2D->SetBinContent(rbin, tbin, SMu2D[g]);
            } 
        }
        /*
        for(int rbin = hLMu2D->GetXaxis()->FindBin(17410); rbin< hLMu2D->GetXaxis()->FindBin(17710); rbin++) {
            for(int tbin = 1; tbin <= TBinN; tbin ++) {
                hLMu2D->SetBinContent(rbin, tbin, hLMu2D->GetBinContent(hLMu2D->GetXaxis()->FindBin(17400), tbin));
                hSMu2D->SetBinContent(rbin, tbin, hSMu2D->GetBinContent(hSMu2D->GetXaxis()->FindBin(17400), tbin));
            }
        }*/
        LnPEMapFile->cd();
        hLMu2D->Smooth();
        hLMu2D->Write();
        SnPEMapFile->cd();
        hSMu2D->Smooth();
        hSMu2D->Write();

    }
    lfile.close();


}
