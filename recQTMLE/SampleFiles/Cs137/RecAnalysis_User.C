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
const int eid = 0;
const int wid = 0;
const double SigE = 0.6617;
const int CalibPosNum = 311;
const int startseed = 0; 
const std::string calibsource="Cs137";
const std::string vmethod="QTMLE";

float emean;
float esigma;
TH1F* h_E;

void RecAnalysis_User() 
{
    void RecAnalysisSingleEnergy(const char* simFilePath, const char* recFilePath, const char* elecFilePath, int posid, int jobNum, int baseline);
    emean = 0;
    esigma = 0;

    //std::string topdir = "/junofs/users/wenjie/job_SLC6/optical_model/Positron/uniform_J16v1r4/";
    //std::string subdir = "e+_1.022MeV";
    
    int evtnum = 0;
    int jobnum = 40;
    int Energy[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    
    string posdir = "/junofs/users/huanggh/Data/ACU_CLS_3D_Pos";
    double positionX[CalibPosNum];
    double positionY[CalibPosNum];
    double positionZ[CalibPosNum];

    std::vector<TString>* StrPosX = new std::vector<TString>;
    std::vector<TString>* StrPosY = new std::vector<TString>;
    std::vector<TString>* StrPosZ = new std::vector<TString>;

    int posid = 0;
    std::ifstream cfile;
    cfile.open((posdir+"/ACU-CLS-GT_XYZPos.txt").c_str(), std::ios::in);
    for(int i=0;i<CalibPosNum;i++) {
        cfile>>posid;
        cfile>>positionX[i];
        cfile>>positionY[i];
        cfile>>positionZ[i];
        std::cout<<positionX[i]<<'\t'<<positionZ[i]<<std::endl;
    }
    cfile.close();

    cfile.open( (posdir+"/ACU-CLS-GT_XYZPos.txt").c_str(), std::ios::in);
    for(int i=0;i<CalibPosNum;i++) {
        cfile>>posid;
        TString temp;
        cfile>>temp;
        StrPosX->push_back(temp);
        cfile>>temp;
        StrPosY->push_back(temp);
        cfile>>temp;
        StrPosZ->push_back(temp);
        std::cout<<StrPosX->at(i)<<'\t'<<StrPosZ->at(i)<<std::endl;
    }

  
    for(int jobA=START;jobA<START+1;jobA++) {
        int seed = 0;
    
        TString recdir = Form("/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/1/ACU/%s/%s_" + StrPosX->at(jobA) + "_" + StrPosY->at(jobA) + "_" + StrPosZ->at(jobA) + "/rec_%s/user-root", calibsource.c_str(), calibsource.c_str(), vmethod.c_str());
        TString simdir = Form("/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/1/ACU/%s/%s_" + StrPosX->at(jobA) + "_" + StrPosY->at(jobA) + "_" + StrPosZ->at(jobA) + "/detsim_and_elecsim/user-root", calibsource.c_str(), calibsource.c_str());
        TString elecdir = Form("/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/1/ACU/%s/%s_" + StrPosX->at(jobA) + "_" + StrPosY->at(jobA) + "_" + StrPosZ->at(jobA) + "/detsim_and_elecsim/user-root", calibsource.c_str(), calibsource.c_str());
        cout<< recdir << endl;
        RecAnalysisSingleEnergy(simdir, recdir, elecdir, jobA, jobnum, seed);
    }
}


void RecAnalysisSingleEnergy(const char* simFilePath, const char* recFilePath, const char* elecFilePath, int posid, int jobNum, int baseline){
    gStyle->SetOptFit(1111);

    TH1D* hSignal_QTEn = new TH1D("hSignal_QTEn", "hSignal_QTEn", 2000, 0., 10.);
    TH1D* hSignal_QEn = new TH1D("hSignal_QEn", "hSignal_QEn", 2000, 0., 10.);
    TH1D* hBkg_QTEn = new TH1D("hBkg_QTEn", "hBkg_QTEn", 2000, 0., 10.);
    TH1D* hBkg_QEn = new TH1D("hBkg_QEn", "hBkg_QEn", 2000, 0., 10.);

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
        TString recFileAdd = Form("%s/user-rec-%s.root",recFilePath,n_flag.c_str());
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

    TFile* sfile = new TFile(Form("Assemb_%s_%d.root", vmethod.c_str(), posid),"RECREATE");


    TTree* rectree = new TTree("evt","evt");
    Float_t recx, recy, recz, recQx, recQy, recQz, recQTx, recQTy, recQTz;
    Float_t timeRecx, timeRecy, timeRecz, timeRecE, timeRect0, Truex, Truey, Truez, TrueQx, TrueQy, TrueQz;
    Float_t QEn, QTEn, NQE, TrueR, TrueQR, QTR, QR, QTt0, Tt0, m_TR,time, nPhotons;
    Float_t Qstat, Tstat, QTstat, memory;
    Double_t triggerT, dTriggerT, corrEvtT;
    Float_t edep, edepX, edepY, edepZ;

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
    rectree->Branch("time",&time,"time/F");
    rectree->Branch("triggerT",&triggerT,"triggerT/F");
    rectree->Branch("dTriggerT",&dTriggerT,"dTriggerT/F");
    rectree->Branch("corrEvtT",&corrEvtT,"corrEvtT/F");

    rectree->Branch("memory",&memory,"memory/F");
    rectree->Branch("edep",&edep,"edep/F");
    rectree->Branch("edepX",&edepX,"edepX/F");
    rectree->Branch("edepY",&edepY,"edepY/F");
    rectree->Branch("edepZ",&edepZ,"edepZ/F");

    while (( recChEl=(TChainElement*)recnext() )){
        TFile* recf = TFile::Open(recChEl->GetTitle(), "READ");
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

        rec_ch -> SetBranchAddress("Truex",&Truex);
        rec_ch -> SetBranchAddress("Truey",&Truey); 
        rec_ch -> SetBranchAddress("Truez",&Truez);
        rec_ch -> SetBranchAddress("TrueQx",&TrueQx);
        rec_ch -> SetBranchAddress("TrueQy",&TrueQy); 
        rec_ch -> SetBranchAddress("TrueQz",&TrueQz);

        rec_ch -> SetBranchAddress("m_NQE",&NQE);
        rec_ch -> SetBranchAddress("m_QEn",&QEn);
        rec_ch -> SetBranchAddress("m_QTEn",&QTEn);

        rec_ch -> SetBranchAddress("m_TR",&m_TR);
        rec_ch -> SetBranchAddress("m_QR",&QR);
        rec_ch -> SetBranchAddress("m_QTR",&QTR);
        rec_ch -> SetBranchAddress("TrueR",&TrueR);
        rec_ch -> SetBranchAddress("TrueQR",&TrueQR);
        rec_ch -> SetBranchAddress("recQTt0",&QTt0);

        rec_ch -> SetBranchAddress("m_Qstat",&Qstat);
        rec_ch -> SetBranchAddress("m_Tstat",&Tstat);
        rec_ch -> SetBranchAddress("m_QTstat",&QTstat);
        rec_ch -> SetBranchAddress("m_QTtime",&time);
        rec_ch -> SetBranchAddress("m_triggerT",&triggerT);
        rec_ch -> SetBranchAddress("m_dTriggerT",&dTriggerT);
        rec_ch -> SetBranchAddress("m_FadcEvtT",&corrEvtT);

        rec_ch -> SetBranchAddress("m_memory",&memory);
        //rec_ch -> SetBranchAddress("edep", &edep);


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
        if(elec_ch->GetEntries()!=rec_ch->GetEntries()) continue;

        int badfile = 0;
        for(int i=0; i<rec_ch->GetEntries(); i++){
            elec_ch->GetEntry(i);
            rec_ch->GetEntry(i);
            int simk = entries->at(0);
            vector<int>::iterator iter = entries->begin();
            while(iter!=entries->end()) {
                if(*iter<simk) simk = *iter;
                iter++;
            }
            if(simk>500) break;
            sim_ch->GetEntry(simk);

            if(QEn/edep < 0.7) badfile++;
        }
        if(badfile>5) {cout<< ">>>>>>>>>>>>>>> Find unmatch file <<<<<<<<<<<<<<<"<<endl;continue;}

        for(int i=0; i<rec_ch->GetEntries(); i++){
            elec_ch->GetEntry(i);
            rec_ch->GetEntry(i);
            int simk = entries->at(0);
            vector<int>::iterator iter = entries->begin();
            while(iter!=entries->end()) {
                if(*iter<simk) simk = *iter;
                iter++;
            }
            if(simk>500) break;
            sim_ch->GetEntry(simk);

            rectree->Fill(); 

            if(edep < SigE*(1.-1.e-4)) {hBkg_QTEn->Fill(QTEn); hBkg_QEn->Fill(QEn);}
            else if(edep < SigE*(1.+1.e-4)) {
                if(QTR<17200) hSignal_QTEn->Fill(QTEn);
                if(QR<17200) hSignal_QEn->Fill(QEn);
            } else {hBkg_QTEn->Fill(QTEn); hBkg_QEn->Fill(QEn);}
  
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
    hBkg_QTEn->Write();
    hBkg_QEn->Write();
    hSignal_QTEn->Write();
    hSignal_QEn->Write();

    sfile->Close();
    
    delete hSignal_QTEn;
    delete hSignal_QEn;
    delete hBkg_QTEn;
    delete hBkg_QEn;
}
