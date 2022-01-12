#define TH_MIN -2000
#define TH_MAX 2000
#define TH_NUM 1000
#define EN_MIN 0.0
#define EN_MAX 10.0
#define EN_NUM 3000

#include <iostream>
#include <sstream>
#include <stdlib.h>  
#include "stdio.h"  
#include <unistd.h>

using std::cout;
using std::endl;

const int parId = 4;
const int eid = ENERGY;
const int wid = 0;
const double E = 1.022;

float emean;
float esigma;
TH1F* h_Z;
TH1F* h_E;
TH2F* th2_energy;
TProfile* pro_energy;

//#define VResCheck
#define ERes

void RecAnalysis_User_eplus() 
{
    void RecAnalysisSingleEnergy(const char* simFilePath, const char* recFilePath, const char* elecFilePath, int evtNum, int jobNum, int baseline);
    emean = 0;
    esigma = 0;

    //std::string topdir = "/junofs/users/wenjie/job_SLC6/optical_model/Positron/uniform_J16v1r4/";
    //std::string subdir = "e+_1.022MeV";
    
    int evtnum = 500;
    int jobnum = FILENUM;
    int Energy[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int seed = 0;
    
    
    TString recdir = Form("root://junoeos01.ihep.ac.cn//eos/juno/user/jiangw/TS/SIGNALWINDOW/%dMeV/recQTMLE/user-root", Energy[eid]);
    TString simdir = Form("/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/e+/e+_Uniform/%dMeV/detsim/user-root", Energy[eid]);
    TString elecdir = Form("/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/e+/e+_Uniform/%dMeV/elecsim/user-root", Energy[eid]);

    RecAnalysisSingleEnergy(simdir, recdir, elecdir, evtnum, jobnum, seed);
}


void RecAnalysisSingleEnergy(const char* simFilePath, const char* recFilePath, const char* elecFilePath, int evtNum, int jobNum, int baseline){
    gStyle->SetOptFit(1111);
    h_Z = new TH1F("h_Z_1.022MeV","Z_{rec}-Z_{edep}_1.022MeV",600,0,6000);
    h_E = new TH1F("h_E_1.022MeV","E_{rec}_1.022MeV",2000,0,20000);
    pro_energy = new TProfile("profile_1.022MeV_energy","profile_1.022MeV_energy",100,0,18,0,3);

    TH1D* hSignal = new TH1D("hSignal", "hSignal", 2000, 0., 10.);
    TH1D* hSpec = new TH1D("hSpec", "hSpec", 2000, 0., 10.);
    TH1D* hBkg = new TH1D("hBkg", "hBkg", 2000, 0., 10.);
    TH1D* hNPESignal = new TH1D("hNPESignal", "hNPESignal", 2000, 0., 10.);
    TChain ch_rec("TRec");
    TChain ch_sim("evt");
    TChain ch_elec("eventindex");
    

    ifstream  failIDin;
    failIDin.open("errorfileid.txt", ios::in);
    vector<int> vFailId;
    int idtemp = 1;
    while(failIDin>>idtemp) {
        vFailId.push_back(idtemp);
    }

    for(int j=0;j<jobNum;j++){
        string n_flag;
        stringstream ss;
        int k = baseline + j;
        bool isContinue = false;
        for(int id = 0;id<vFailId.size(); id++) {
            if(vFailId[id]==k) {isContinue = true;break;}
        }
        if(isContinue) continue;
        ss  <<  k;
        ss >> n_flag;
        TString recFileAdd = Form("%s/user-recQTMLE-%s.root",recFilePath,n_flag.c_str());
        TString simFileAdd = Form("%s/user-detsim-%s.root",simFilePath,n_flag.c_str());
        TString elecFileAdd = Form("%s/user-elecsim-%s.root",elecFilePath,n_flag.c_str());
        ch_rec.Add(recFileAdd);
        ch_sim.Add(simFileAdd);
        ch_elec.Add(elecFileAdd);
    }

    TObjArray* recFileElements = ch_rec.GetListOfFiles();
    TIter recnext(recFileElements);
    TChainElement* recChEl = 0;
    TObjArray* simFileElements = ch_sim.GetListOfFiles();
    TIter simnext(simFileElements);
    TChainElement* simChEl = 0;
    TObjArray* elecFileElements = ch_elec.GetListOfFiles();
    TIter elecnext(elecFileElements);
    TChainElement* elecChEl = 0;
 
    #ifdef ERes
    TFile* sfile = new TFile("Assemb.root","RECREATE");
    #endif

    #ifdef VResCheck
    TFile* sfile = new TFile("Assemb_VResCheck.root","RECREATE");
    #endif

    TTree* rectree = new TTree("evt","evt");
    Float_t recx, recy, recz, recQx, recQy, recQz, recQTx, recQTy, recQTz;
    Float_t timeRecx, timeRecy, timeRecz, timeRecE, timeRect0, Truex, Truey, Truez, TrueQx, TrueQy, TrueQz;
    Float_t QEn, QTEn, NQE, TrueR, TrueQR, QTR, QR, QTt0, Tt0, m_TR, Qtime, Ttime, QTtime, nPhotons;
    Float_t Qstat, Tstat, QTstat, memory, edep, edepX, edepY, edepZ;
    Double_t triggerT, dTriggerT, corrEvtT;
    rectree->Branch("recx",&recx,"recx/F");
    rectree->Branch("recy",&recy,"recy/F");
    rectree->Branch("recz",&recz,"recz/F");

    rectree->Branch("recQx",&recQx,"recQx/F");
    rectree->Branch("recQy",&recQy,"recQy/F");
    rectree->Branch("recQz",&recQz,"recQz/F");


    rectree->Branch("recQTx",&recQTx,"recQTx/F");
    rectree->Branch("recQTy",&recQTy,"recQTy/F");
    rectree->Branch("recQTz",&recQTz,"recQTz/F");

    rectree->Branch("timeRecx",&timeRecx,"timeRecx/F");
    rectree->Branch("timeRecy",&timeRecy,"timeRecy/F");
    rectree->Branch("timeRecz",&timeRecz,"timeRecz/F");
    rectree->Branch("timeRecE",&timeRecE,"timeRecE/F");
    rectree->Branch("timeRect0",&timeRect0,"timeRect0/F");

    rectree->Branch("Truex",&Truex,"Truex/F");
    rectree->Branch("Truey",&Truey,"Truey/F");
    rectree->Branch("Truez",&Truez,"Truez/F");

    rectree->Branch("TrueQx",&TrueQx,"TrueQx/F");
    rectree->Branch("TrueQy",&TrueQy,"TrueQy/F");
    rectree->Branch("TrueQz",&TrueQz,"TrueQz/F");

    rectree->Branch("NQE",&NQE,"NQE");
    rectree->Branch("m_QEn",&QEn,"QEn/F");
    rectree->Branch("m_QTEn",&QTEn,"TQEn/F");

    rectree->Branch("m_Qstat",&Qstat,"Qstat/F");
    rectree->Branch("m_Tstat",&Tstat,"Tstat/F");
    rectree->Branch("m_QTstat",&QTstat,"QTstat/F");

    rectree->Branch("TrueR",&TrueR,"TrueR/F");
    rectree->Branch("m_QR",&QR,"QR/F");
    rectree->Branch("m_TR",&m_TR,"m_TR/F");
    rectree->Branch("m_QTR",&QTR,"QTR/F");

    rectree->Branch("QTt0",&QTt0,"QTt0/F");
    rectree->Branch("Qtime",&Qtime,"Qtime/F");
    rectree->Branch("Ttime",&Ttime,"Ttime/F");
    rectree->Branch("QTtime",&QTtime,"QTtime/F");
    rectree->Branch("triggerT",&triggerT,"triggerT/F");
    rectree->Branch("dTriggerT",&dTriggerT,"dTriggerT/F");
    rectree->Branch("corrEvtT",&corrEvtT,"corrEvtT/D");

    rectree->Branch("memory",&memory,"memory/F");
    rectree->Branch("edep", &edep,"edep/F");
    rectree->Branch("edepX",&edepX,"edepX/F");
    rectree->Branch("edepY",&edepY,"edepY/F");
    rectree->Branch("edepZ",&edepZ,"edepZ/F");

    while (( recChEl=(TChainElement*)recnext() )){
        TFile* recf = TFile::Open(recChEl->GetTitle());
        cout << recChEl->GetTitle() << endl;
        if(recf->TestBit(TFile::kRecovered)) continue;
        if(recf->IsZombie()) continue;
        TTree* rec_ch = (TTree*)recf -> Get("TRec");
        if(rec_ch->IsZombie()) continue;


        rec_ch -> SetBranchAddress("recx",&recx);
        rec_ch -> SetBranchAddress("recy",&recy); 
        rec_ch -> SetBranchAddress("recz",&recz);
        rec_ch -> SetBranchAddress("recQx",&recQx);
        rec_ch -> SetBranchAddress("recQy",&recQy); 
        rec_ch -> SetBranchAddress("recQz",&recQz);
        rec_ch -> SetBranchAddress("timeRecx",&timeRecx);
        rec_ch -> SetBranchAddress("timeRecy",&timeRecy); 
        rec_ch -> SetBranchAddress("timeRecz",&timeRecz);
        rec_ch -> SetBranchAddress("timeRecE",&timeRecE);
        rec_ch -> SetBranchAddress("timeRect0",&timeRect0);

        // rec_ch -> SetBranchAddress("Truex",&Truex);
        // rec_ch -> SetBranchAddress("Truey",&Truey); 
        // rec_ch -> SetBranchAddress("Truez",&Truez);
        rec_ch -> SetBranchAddress("TrueQx",&TrueQx);
        rec_ch -> SetBranchAddress("TrueQy",&TrueQy); 
        rec_ch -> SetBranchAddress("TrueQz",&TrueQz);

        rec_ch -> SetBranchAddress("m_NQE",&NQE);
        rec_ch -> SetBranchAddress("m_QEn",&QEn);
        rec_ch -> SetBranchAddress("m_QTEn",&QTEn);

        rec_ch -> SetBranchAddress("m_TR",&m_TR);
        rec_ch -> SetBranchAddress("m_QR",&QR);
        rec_ch -> SetBranchAddress("m_QTR",&QTR);
        // rec_ch -> SetBranchAddress("TrueR",&TrueR);
        rec_ch -> SetBranchAddress("TrueQR",&TrueQR);
        rec_ch -> SetBranchAddress("recQTt0",&QTt0);

        rec_ch -> SetBranchAddress("m_Qstat",&Qstat);
        rec_ch -> SetBranchAddress("m_Tstat",&Tstat);
        rec_ch -> SetBranchAddress("m_QTstat",&QTstat);
        rec_ch -> SetBranchAddress("m_Qtime",&Qtime);
        rec_ch -> SetBranchAddress("m_Ttime",&Ttime);
        rec_ch -> SetBranchAddress("m_QTtime",&QTtime);
        rec_ch -> SetBranchAddress("m_triggerT",&triggerT);
        rec_ch -> SetBranchAddress("m_dTriggerT",&dTriggerT);
        rec_ch -> SetBranchAddress("m_FadcEvtT",&corrEvtT);

        rec_ch -> SetBranchAddress("m_memory",&memory);
        // rec_ch -> SetBranchAddress("edep", &edep);
        
        simChEl=(TChainElement*)simnext();
        TFile* simf = new TFile(simChEl->GetTitle());
        cout << simChEl->GetTitle() << endl;
        if(simf->TestBit(TFile::kRecovered)) continue;
        if(simf->IsZombie()) continue;
        TTree* sim_ch = (TTree*)simf -> Get("evt");
        if(sim_ch->IsZombie()) continue;
        sim_ch -> SetBranchAddress("edep",&edep);
        sim_ch -> SetBranchAddress("edepX",&edepX);
        sim_ch -> SetBranchAddress("edepY",&edepY);
        sim_ch -> SetBranchAddress("edepZ",&edepZ);

        elecChEl=(TChainElement*)elecnext();
        TFile* elecf = new TFile(elecChEl->GetTitle());
        cout << elecChEl->GetTitle() << endl;
        if(elecf->TestBit(TFile::kRecovered)) continue;
        if(elecf->IsZombie()) continue;
        TTree* elec_ch = (TTree*)elecf -> Get("eventindex");
        if(elec_ch->IsZombie()) continue;
        vector<int>* entries = new vector<int>;
        elec_ch -> SetBranchAddress("entries",&entries);

        cout<<sim_ch->GetEntries()<< '\t' << elec_ch->GetEntries()<< '\t' << rec_ch->GetEntries()<< endl;

        for(int i=0; i<rec_ch->GetEntries(); i++){
            elec_ch->GetEntry(i);
            rec_ch->GetEntry(i);
            int simk = entries->at(0);
            vector<int>::iterator iter = entries->begin();
            while(iter!=entries->end()) {
                if(*iter<simk) simk = *iter;
                iter++;
            }
            sim_ch->GetEntry(simk);

            TrueR = sqrt(edepX*edepX+edepY*edepY+edepZ*edepZ);
            #ifdef VResCheck 
            bool IsBadFile = false;
            if(TMath::Abs(QR - TrueR)>2000) {cout << "####### Wrong File! #######" << recChEl->GetTitle() << endl;IsBadFile = true; break;}
            if(IsBadFile) continue;
            #endif

            Truex=edepX;
            Truey=edepY;
            Truez=edepZ;
            rectree->Fill(); 

  
            Float_t edep_r = sqrt(recx*recx + recy*recy + recz*recz);
            pro_energy->Fill(pow(edep_r/1000.,3), QTEn,1);
            
            
            if(edep_r < 17200.) {
                hSignal->Fill(QEn);
                hNPESignal->Fill(nPhotons);
            }
        }

        delete rec_ch;
        delete elec_ch;
        delete sim_ch;
        delete recf;
        delete elecf;
        delete simf;
    }

    sfile->cd();
    rectree->Write();
    hBkg->Write();
    //hSpec->Write();
    hSignal->Write();
    hNPESignal->Write();

    sfile->Close();
    
    TCanvas* myc=new TCanvas("myc","a canvas",10,10,700,500);
    h_Z->GetXaxis()->SetTitle("R^{3}[m^{3}]");
    h_Z->Draw();

    TCanvas* myc1=new TCanvas("myc1","a canvas",10,10,700,500);
    //h_E->Fit("gaus","W","C",EN_MIN,EN_MAX);
    //TF1* fitE = (TF1*)h_E->GetFunction("gaus");
    //Float_t energy_mean = fitE ->GetParameter(1);
    //Float_t energy_sigma = fitE->GetParameter(2)/fitE->GetParameter(1);
    h_E->GetXaxis()->SetTitle("energy[MeV]");
    h_E->Draw();

    TCanvas* myc_energy=new TCanvas("myc_energy","energy distribution",10,10,900,500);
    myc_energy->Divide(2,1);
    pro_energy->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
    pro_energy->GetYaxis()->SetTitle("energy[MeV]");
    pro_energy->GetYaxis()->SetTitleOffset(1.2);
    pro_energy->GetYaxis()->CenterTitle();
    pro_energy->SetLineColor(2);
    myc_energy->cd(1);
    myc_energy->cd(2);
    pro_energy->GetYaxis()->SetRangeUser(0,3);
    pro_energy->Draw();

//    emean = energy_mean;
//    esigma = 100*energy_sigma;
//    cout << "*******************************************" << endl;
//    cout << "    energy mean:" <<  energy_mean << " MeV"  << endl;
//    cout << "    energy resolution: " << 100*energy_sigma << " % " << endl;
//    cout << "*******************************************" << endl;
    cout << "Successfully" << endl;
}
