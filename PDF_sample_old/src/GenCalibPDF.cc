
#include "GenCalibPDF.h"
#include "TLegend.h"
#include "TLine.h"
#include "TTree.h"
#include "TH1.h"
#include "TGraph.h"
#include "TString.h"
#include "TSystem.h"
#include "TStopwatch.h"
#include "Minuit2/FCNBase.h"
#include "SniperKernel/AlgFactory.h"
#include "Event/RecHeader.h"
#include "Event/CalibHeader.h"
#include "Context/TimeStamp.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
#include <boost/filesystem.hpp>
#include "RootWriter/RootWriter.h"
#include "PMTCalibSvc/PMTCalibSvc.h"

#include "DataRegistritionSvc/DataRegistritionSvc.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "TAxis.h"
#include "TMath.h"
#include "TROOT.h"

namespace fs = boost::filesystem;
DECLARE_ALGORITHM(GenCalibPDF);


GenCalibPDF::GenCalibPDF(const std::string& name)
:   AlgBase(name)
{
    TotalLPMT = 17612;
    PMT_R = 19.434;
    Ball_R = 19.246;
    LS_R = 17.7 + 0.12;
    SignalWindow = 280;
    PedThres = 0.1;
    enableTruePMTPara = false;
    enableAmC = false;
    PMTtype = 0;
    calibdir = "";
    usercalibdir = "";
    posdir = "";
    paradir = "";
    calibsource = "AmC";
    filenum = 10;
    startseed = 50000;

    RfrIndxLS = 1.5;
    RfrIndxWR = 1.355;

    jobA=0;
    jobB=240;
    declProp("jobA", jobA);
    declProp("jobB", jobB);
    declProp("RfrIndxLS", RfrIndxLS);
    declProp("RfrIndxWR", RfrIndxWR);
    declProp("SignalWindow", SignalWindow);
    declProp("PMTtype", PMTtype);
    declProp("filenum", filenum);
    declProp("startseed", startseed);
    declProp("enableAmC", enableAmC);
    declProp("calibdir", calibdir);
    declProp("usercalibdir", usercalibdir);
    declProp("posdir", posdir);
    declProp("paradir", paradir);
    declProp("calibsource", calibsource);


}

GenCalibPDF::~GenCalibPDF() {}

bool GenCalibPDF::initialize()
{
    SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
    if ( rgSvc.invalid()) {
        LogError << "Failed to get RecGeomSvc instance!" << std::endl;
        return false;
    }
    m_cdGeom = rgSvc->getCdGeom();
    TotalLPMT = m_cdGeom->findPmt20inchNum();
    TotalSPMT = m_cdGeom->findPmt3inchNum();

    LogInfo << "Total LPMT: " << TotalLPMT << '\t' <<TotalSPMT<<std::endl;
    for(int i = 0; i<TotalLPMT; i++){
        unsigned int all_pmt_id = (unsigned int)i;
        Identifier all_id = Identifier(CdID::id(all_pmt_id,0));
        PmtGeom *all_pmt = m_cdGeom->getPmt(all_id);
        if ( !all_pmt ) {
            LogError << "Wrong Pmt Id " << i << std::endl;
        }
        TVector3 all_pmtCenter = all_pmt->getCenter();
        ALL_LPMT_pos.push_back(all_pmtCenter);
    }
    

    TFile* TestFile = new TFile("ParaFile.root", "RECREATE");
    TH1D* hToff = new TH1D("hToff", "hToff", 400, -50, 50);
    TH1D* hDCR = new TH1D("hDCR", "hDCR", 1200, 0, 120000);
    TH1D* hGain = new TH1D("hGain", "hGain", 500, 0, 2);
    TH1D* hQRes = new TH1D("hQRes", "hQRes", 100, 0, 1);
    TH1D* hPDE = new TH1D("hPDE", "hPDE", 500, 0, 2);
    for(int i=0;i<TotalLPMT;i++) {
        timeOffset.push_back(0.);
        Gain.push_back(1.);
        QRes.push_back(0.3);
        DC.push_back(0.);
        PDE.push_back(1.);
    }

    std::string base = getenv("JUNOTOP");
    std::ifstream datain;
    datain.open(base + "/data/Simulation/ElecSim/Hamamatsu_pmtID.txt", std::ios::in);

    IsDyn = new bool[MaxPMTNum];
    for(int i=0;i<MaxPMTNum;++i) IsDyn[i] = false;
    int dTemp = 0;
    while(datain>>dTemp) {
        IsDyn[dTemp] = true;
    }
    datain.close();
    std::cout<< "hha3" << std::endl;

    datain.open(base + "/data/Simulation/ElecSim/pmtdata.txt", std::ios::in);
    double idtemp = 0., DEtemp = 0.;
    std::string typetemp = "";

    double LPDE_Avg = 0.;
    for(int i=0;i<TotalLPMT;i++) {
        datain>>idtemp;
        datain>>typetemp;
        datain>> PDE[i];
        LPDE_Avg += PDE[i];
    }
    LPDE_Avg /= double(TotalLPMT);
    for(int i=0;i<TotalLPMT;i++) PDE[i] /= LPDE_Avg;
    /*
    SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
    if (calSvc.invalid()) {
        LogError << "Can't Locate  PMTCalibSvc."
                 << std::endl;
        return false;
    }
    if((calSvc->getGain()).at(0)>1e-8) Gain = calSvc->getGain();

    if((calSvc->getDarkRate()).at(0)>1e-8) DC = calSvc->getDarkRate();

    if((calSvc->getRelativeDE()).at(0)>1e-8) PDE = calSvc->getRelativeDE();

    if((calSvc->getTimeOffset()).at(0)>1e-8) timeOffset = calSvc->getTimeOffset();
    */
    //SPEs = calSvc->oetChargeSpec();

    TFile* CalibSPEs_m = new TFile( (paradir+"/output_deconv.root").c_str(), "READ");
    for(int i=0;i<TotalLPMT;i++) {
        TH1F* spes_temp = (TH1F*)CalibSPEs_m->Get(Form("ch%d_charge_spec", i));
        SPEs.push_back(spes_temp);
    }

    for(int i=0;i<TotalLPMT;i++) {
        SPEs[i]->Fit("gaus", "RQ0", "", 0.5, 1.5);
        TF1* fitfunc = SPEs[i]->GetFunction("gaus");
        Gain[i] = fitfunc->GetParameter(1);
        QRes[i] = fitfunc->GetParameter(2);
        if(!IsDyn[i]) Gain[i] = SPEs[i]->GetMean();
        delete fitfunc;
    }

    LogDebug<<"PDE[0]:"<<PDE[0]<<"   "<<"DC[0]:"<<DC[0]<<"   "<<"Gain[0]:"<<Gain[0]<<"   "<<"timeOffset[1]:"<<timeOffset[1]<<std::endl;

    //////////////
    // Update para
    double id_temp = 0.;
    double gain_temp = 0.;
    double meangain_temp = 0.;
    double dr_temp = 0.;
    double de_temp = 0.;
    double time_temp = 0.;
    std::ifstream InputGainPara;
    InputGainPara.open( (paradir+"/PmtPrtData_deconv.txt").c_str(), std::ios::in);
    for(int i=0;i<TotalLPMT;i++) {
        InputGainPara>>id_temp;
        InputGainPara>>de_temp;
        InputGainPara>>gain_temp;
        InputGainPara>>time_temp;
        InputGainPara>>dr_temp;
        InputGainPara>>meangain_temp;

        PDE[i] = de_temp;
        timeOffset[i] = time_temp;
        timeOffset[i] = 0.;
        DC[i] = dr_temp;

    }

    LogDebug<<"PDE[0]:"<<PDE[0]<<'\t'<<"DC[0]:"<<DC[0]<<'\t'<<"Gain[0]:"<<Gain[0]<< '\t'<<"timeOffset[0]:"<<timeOffset[0]<< std::endl;
    LogDebug<<"PDE[17611]:"<<PDE[17611]<<'\t'<<"DC[17611]:"<<DC[17611]<<'\t'<<"Gain[17611]:"<<Gain[17611]<< '\t'<<"timeOffset[17611]:"<<timeOffset[17611]<< std::endl;

    
    LPDE_Avg = 0.;
    for(int i=0;i<TotalLPMT;i++) {
        LPDE_Avg += PDE[i];
    }
    LPDE_Avg /= double(TotalLPMT);
    for(int i=0;i<TotalLPMT;i++) PDE[i] /= LPDE_Avg;
 
    for(int i=0;i<TotalLPMT;i++) LDarkMu += DC[i];

    LDarkMu *= ADCL;
    LDarkMu /= (1.0e9 * double(TotalLPMT));

    LogInfo<<"========= Dark noise is available =========:"<< LDarkMu*double(TotalLPMT) << '\t'<<(LDarkMu*double(TotalLPMT))/250.+sqrt((LDarkMu * double(TotalLPMT))/250.)<<std::endl;

    pmt_theta = new double[MaxPMTNum];
    pmt_phi = new double[MaxPMTNum];
    for(int i=0;i<MaxPMTNum;i++) {pmt_theta[i] = 0.; pmt_phi[i] = 0.;}


    if(enableTruePMTPara) {
        LDarkMu = 0.;
        std::string PmtDataBase = base + "/data/Simulation/ElecSim/PmtData.root";

        double timeOffsettemp = 0., ttstemp = 0., dntemp = 0., qrestemp = 0.;
        TFile* PmtDataFile = TFile::Open( PmtDataBase.c_str(), "READ");
        TTree* PmtData = (TTree*)PmtDataFile->Get("PmtData");
        PmtData->SetBranchAddress("sigmaGain", &qrestemp);
        PmtData->SetBranchAddress("timeOffset", &timeOffsettemp);
        PmtData->SetBranchAddress("timeSpread", &ttstemp);
        PmtData->SetBranchAddress("darkRate", &dntemp);
        for(int i=0;i<TotalLPMT;i++) {
            PmtData->GetEntry(i);
            DC[i] = dntemp;
            LDarkMu += DC[i];
            //timeOffset[i] = timeOffsettemp - 40.;
        }
        delete PmtData;
        delete PmtDataFile;

        LDarkMu *= ADCL;
        LDarkMu /= (1.0e9 * double(TotalLPMT));
        LogInfo<<"Adding toy dark noise ..."<<std::endl;
        LogInfo<<"The dark noise mu & threshold of LPMT is around:" << LDarkMu << '\t'
               <<(LDarkMu*double(TotalLPMT))/(ADCL/LFHSBinW)+sqrt((LDarkMu * double(TotalLPMT))/(ADCL/LFHSBinW))<<std::endl;

    }  

    for(int i=0;i<TotalLPMT;i++) {
        hToff->Fill(timeOffset[i]);
        hDCR->Fill(DC[i]);
        hGain->Fill(Gain[i]);
        hQRes->Fill(QRes[i]);
        hPDE->Fill(PDE[i]);
    }
    TestFile->cd();
    hToff->Write();
    hDCR->Write();
    hGain->Write();
    hQRes->Write();
    hPDE->Write(); 
    TestFile->Close();
 
    return true;
}



bool GenCalibPDF::execute()
{
    std::string base = getenv("JUNOTOP");


    //read calibration position
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
   
    //read PMT position
    std::string PMTFile = base + "/offline/Simulation/DetSimV2/DetSimOptions/data/PMTPos_Acrylic_with_chimney.csv";
    std::ifstream sfile;
    sfile.open(PMTFile.c_str(),std::ios::in);

    Int_t m_pmtID = 1;
    Double_t m_theta=0., m_phi=0.;
    double temp = 0.;
    while (!sfile.eof()) {
        sfile >> m_pmtID;
        sfile >> temp >> temp >> temp;
        sfile >> m_theta >> m_phi;
        if (!sfile.eof()) {
            pmt_theta[m_pmtID] = m_theta;
            pmt_phi[m_pmtID] = m_phi;
        }
    }
    sfile.close();


    TCanvas* cHv = new TCanvas("cHv","Graph Example",200,10,700,500);
    TString plotName = Form("PEMeanMap_%d",jobA);
    cHv->Print(plotName+".pdf[", "pdf");


    double* BinTotalRecDyn = (double*)malloc(sizeof(double)*qBinN*dBinN);
    double* RECvsTRUTHRecDyn = (double*)malloc(sizeof(double)*qBinN*dBinN*tBinN);

    double* BinTotalRecMcp = (double*)malloc(sizeof(double)*qBinN*dBinN);
    double* RECvsTRUTHRecMcp = (double*)malloc(sizeof(double)*qBinN*dBinN*tBinN);


    for(int qk=0;qk<qBinN;qk++) {
        for(int dk=0;dk<dBinN;dk++) {
            *(BinTotalRecDyn + qk*dBinN + dk) = 0.;
            *(BinTotalRecMcp + qk*dBinN + dk) = 0.;
            for(int tk=0;tk<tBinN;tk++) {
                *(RECvsTRUTHRecDyn + qk*dBinN*tBinN + dk*tBinN + tk) = 0.;
                *(RECvsTRUTHRecMcp + qk*dBinN*tBinN + dk*tBinN + tk) = 0.;
            }
        }
    }

    
    for (int i=jobA;i<jobB;i++) {
        std::cout<<"-----------------"<<i<<"-----------------"<<std::endl;
        std::vector<double> thetapmt;
        std::vector<double> MEAN_PE;
        TChain* fCalibChain = new TChain("Event/Calib/CalibHeader");
        TChain* fUsercalibChain = new TChain("calibevt");
        for (int j=0;j<filenum;j++) {
            //int SEED = startseed + j + 40*jobA; 
            int SEED =  j; 
            TString CalibFileName;
            //CalibFileName = Form("%s/%s/%s_" + StrPosX->at(i) + "_" + StrPosY->at(i) + "_" + StrPosZ->at(i) + "/calib/root/calib-%d.root", calibdir.c_str(), "photon_11522_nofixingE", calibsource.c_str(), SEED);
            CalibFileName = Form("%s/%s/%s_" + StrPosX->at(i) + "_" + StrPosY->at(i) + "_" + StrPosZ->at(i) + "/calib/root/calib-%d.root", calibdir.c_str(), calibsource.c_str(), calibsource.c_str(), SEED);

            TString UserCalibFileName;
            //UserCalibFileName = Form("%s/%s/%s_" + StrPosX->at(i) + "_" + StrPosY->at(i) + "_" + StrPosZ->at(i) +"/calib/user-root/user-calib-%d.root", usercalibdir.c_str(), "photon_11522_nofixingE", calibsource.c_str(), SEED);
            UserCalibFileName = Form("%s/%s/%s_" + StrPosX->at(i) + "_" + StrPosY->at(i) + "_" + StrPosZ->at(i) +"/calib/user-root/user-calib-%d.root", usercalibdir.c_str(), calibsource.c_str(), calibsource.c_str(), SEED);
            //if(!gSystem->AccessPathName(CalibFileName) && !gSystem->AccessPathName(UserCalibFileName)) {
            if(TFile::Open(CalibFileName) && TFile::Open(UserCalibFileName)) {
                fCalibChain->Add(CalibFileName);
                fUsercalibChain->Add(UserCalibFileName);
                std::cout << "==> Adding " << CalibFileName << std::endl;
                std::cout << "==> Adding " << UserCalibFileName << std::endl;
            } else {
                std::cout << "==> Can't find " << CalibFileName << std::endl;
                std::cout << "==> Can't find " << UserCalibFileName << std::endl;
            }
        }

        if (fCalibChain->IsZombie() || fUsercalibChain->IsZombie()) {
            std::cout << "This is a zombie chain" << std::endl;
            continue;
        }
        GenPDF(calibdir.c_str(), fCalibChain, fUsercalibChain, i, positionX[i],positionZ[i], thetapmt, MEAN_PE, cHv, plotName, 
             BinTotalRecDyn, RECvsTRUTHRecDyn, BinTotalRecMcp, RECvsTRUTHRecMcp);

        delete fCalibChain;
    }

    std::cout<<"begin save time pdf data!"<<std::endl;
    std::ofstream DynTimePdfOut;
    DynTimePdfOut.open(Form("Tdyn_pos%d.txt", jobA ), std::ios::out);
    std::ofstream McpTimePdfOut;
    McpTimePdfOut.open(Form("Tmcp_pos%d.txt", jobA ), std::ios::out);

    for(int qk=0;qk<qBinN;qk++) {
        for(int dk=0;dk<dBinN;dk++) {
            for(int tk=0;tk<tBinN;tk++) {
                DynTimePdfOut<< (*(BinTotalRecDyn + qk*dBinN + dk))<<" "<<(*(RECvsTRUTHRecDyn + qk*dBinN*tBinN + dk*tBinN + tk))<< std::endl;
                McpTimePdfOut<< (*(BinTotalRecMcp + qk*dBinN + dk))<<" "<<(*(RECvsTRUTHRecMcp + qk*dBinN*tBinN + dk*tBinN + tk))<< std::endl;

                if((*(BinTotalRecDyn + qk*dBinN + dk))>1e-8)  (*(RECvsTRUTHRecDyn + qk*dBinN*tBinN + dk*tBinN + tk)) /= (*(BinTotalRecDyn + qk*dBinN + dk));
                if((*(BinTotalRecMcp + qk*dBinN + dk))>1e-8)  (*(RECvsTRUTHRecMcp + qk*dBinN*tBinN + dk*tBinN + tk)) /= (*(BinTotalRecMcp + qk*dBinN + dk));
            }

        }
    }
    DynTimePdfOut.close();
    McpTimePdfOut.close();

    cHv->Print(plotName+".pdf]", "pdf");
    return true;
}



void GenCalibPDF::GenPDF(const char *SavePath, TChain* calibChain, TChain* usercalibChain, int PlaceID, const double PositionX, const double PositionZ,
            std::vector<double>& m_theta, std::vector<double>& Mean_PE, TCanvas* cHv_, TString plotName_, double* BinTotalRecDyn_, double* RECvsTRUTHRecDyn_,
            double* BinTotalRecMcp_, double* RECvsTRUTHRecMcp_) {

    // Read the PMT configuration file
    std::cout << "=== Start Generating ===" << std::endl;
    int PMTNUM = TotalLPMT;
    const int N = PMTNUM;
    double PMTTheta[N];
    double PMTX[N];
    double PMTY[N];
    double PMTZ[N];
    double PMTDist[N];
    double tmptheta;
    double tmpphi;
    double EventTheta;
    double EvtR = sqrt(PositionX*PositionX + PositionZ*PositionZ);
    if(TMath::Abs(EvtR)>1e-10) EventTheta = TMath::ACos(PositionZ/EvtR);
    else EventTheta = 0.;


    for (int i=0;i<N;i++) {
        
        
        tmptheta = pmt_theta[i]*TMath::Pi()/180.;
        tmpphi = pmt_phi[i]*TMath::Pi()/180.;

        /*
        PMTX[i] = PMT_R*1E3*TMath::Sin(tmptheta)*TMath::Cos(tmpphi);
        PMTY[i] = PMT_R*1E3*TMath::Sin(tmptheta)*TMath::Sin(tmpphi);
        PMTZ[i] = PMT_R*1E3*TMath::Cos(tmptheta);
        */

        PMTX[i] = ALL_LPMT_pos.at(i).X();
        PMTY[i] = ALL_LPMT_pos.at(i).Y();
        PMTZ[i] = ALL_LPMT_pos.at(i).Z();
        
 
        PMTTheta[i] = TMath::ACos(TMath::Sin(tmptheta)*TMath::Cos(tmpphi)*TMath::Sin(EventTheta)+
                                  TMath::Cos(tmptheta)*TMath::Cos(EventTheta))*180./TMath::Pi();
        PMTDist[i] = sqrt((PMTX[i] - PositionX)*(PMTX[i] - PositionX) + (PMTY[i] - 0.)*(PMTY[i] - 0.) + (PMTZ[i] - PositionZ)*(PMTZ[i] - PositionZ))/1.e3;
    }
    int BinNumOfTheta = 1440;
    
    std::cout << "Top dir is : " << SavePath << std::endl;
    TString SaveCalibFileName = Form("PE-theta-distribution-%d.root", PlaceID);
    TFile* distribution = new TFile(SaveCalibFileName,"RECREATE");
    std::cout << "Generating: " << SaveCalibFileName << std::endl;

    TH1D*  DAxis = new TH1D("DAxis", "DAxis", dBinN, 0., dRange);
    TH1D*  TAxis = new TH1D("TAxis", "TAxis", tBinN, 0., tRange);
    TProfile* PEdistribution = new TProfile("PEdistribution","mean PE of PMTs located at theta",BinNumOfTheta,-0.001,180.001);
    TProfile* RecNPMTdistribution = new TProfile("RecNPMTdistribution","mean hit number of  PMTs located at theta",BinNumOfTheta,-0.001,180.001,0,100);
    TProfile* dPEdistribution = new TProfile("dPEdistribution","dark PE of PMTs located at theta",BinNumOfTheta,-0.001,180.001);

    //debug hist
    TH1D* T1vsD_Dyn = new TH1D("T1vsD_Dyn", "T1vsD_Dyn", tBinN, 0., tRange);
    TH1D* T2vsD_Dyn = new TH1D("T2vsD_Dyn", "T2vsD_Dyn", tBinN, 0., tRange);

    TH1D* T1vsD_Mcp = new TH1D("T1vsD_Mcp", "T1vsD_Mcp", tBinN, 0., tRange);
    TH1D* T2vsD_Mcp = new TH1D("T2vsD_Mcp", "T2vsD_Mcp", tBinN, 0., tRange);

    int Nentries = calibChain->GetEntries();
    int UsercalibNentries = usercalibChain->GetEntries();
    int evtNum = 1;
    int UsercalibEvtNum = 1;

    Double_t m_nPE[N];
    Double_t m_AllQ[N];
    Double_t m_channel_evt[N];
    for(int ii=0;ii<N;++ii) {
        m_nPE[ii] = 0.;
        m_AllQ[ii] = 0.;
        m_channel_evt[ii] = 0.;
    }

    std::cout<<Nentries<<'\t' <<UsercalibNentries<<std::endl;
    //std::ofstream checkdata;
    //checkdata.open(Form("checkdata-%d-%d.txt",jobA,jobB),std::ios::out);
    
    TObjArray* calibFileElements1 = calibChain->GetListOfFiles();
    TIter calibnext1(calibFileElements1);
    TChainElement* calibChEl1 = 0;

    TObjArray* usercalibFileElements1 = usercalibChain->GetListOfFiles();
    TIter usercalibnext1(usercalibFileElements1);
    TChainElement* usercalibChEl1 = 0;
 
     
    std::vector<double> EvtTArray;
    TH1D* hTotalPE_Org = new TH1D("hTotalPE_Org", "hTotalPE_Org", 1000, 0, 20000);
    int kk =-1;
    while (( calibChEl1=(TChainElement*)calibnext1() )){
        //read calib.root
        TFile* calibf = TFile::Open(calibChEl1->GetTitle());
        std::cout <<":" << calibChEl1->GetTitle() << std::endl;
        if(calibf->IsZombie() || calibf->TestBit(TFile::kRecovered)) continue;
        TTree* calib_ch = (TTree*)calibf -> Get("Event/Calib/CalibEvent");

        if(calib_ch->IsZombie()) continue;
        JM::CalibEvent* CalibEvt = new JM::CalibEvent();
        calib_ch->SetBranchAddress("CalibEvent",&CalibEvt);
        evtNum = calib_ch->GetEntries();

        //read TrigTime from user-calib.root 
        usercalibChEl1=(TChainElement*)usercalibnext1();
        TFile* usercalibf = TFile::Open(usercalibChEl1->GetTitle());
        std::cout <<":" << usercalibChEl1->GetTitle() << std::endl;
        if(usercalibf->IsZombie() || usercalibf->TestBit(TFile::kRecovered)) continue;

        TTree* usercalib_ch = (TTree*)usercalibf -> Get("calibevt");
        if(usercalib_ch->IsZombie()) continue;
        double m_triggerTime;
        int TrigTimeSec;
        int TrigTimeNanoSec;
        usercalib_ch->SetBranchAddress("TrigTimeSec", &TrigTimeSec);
        usercalib_ch->SetBranchAddress("TrigTimeNanoSec", &TrigTimeNanoSec);
        UsercalibEvtNum = usercalib_ch->GetEntries();

        std::cout<<evtNum<<'\t'<<UsercalibEvtNum<<std::endl;

        for(int ri=0; ri<evtNum; ri++){
            usercalib_ch->GetEntry(ri);
			//std::cout<< ri<< std::endl;
            
            kk ++;
            double DEvtT = 1.e10;
            m_triggerTime = TrigTimeSec*1.e9 + TrigTimeNanoSec;
            EvtTArray.push_back(m_triggerTime);

            if(enableAmC) {
                if(ri>0) DEvtT = m_triggerTime - EvtTArray.at(kk-1);
                if(DEvtT>1.e6) continue;
            }

            calib_ch->GetEntry(ri);

            const std::list<JM::CalibPMTChannel*>& chhlist = CalibEvt->calibPMTCol();
            std::list<JM::CalibPMTChannel*>::const_iterator chit = chhlist.begin();
            
            double TotalQ = 0.;
            while (chit!=chhlist.end()) {
                JM::CalibPMTChannel* calib = *chit++;

                unsigned int pmtId = calib->pmtId();
                Identifier id = Identifier(pmtId);
                if (not CdID::is20inch(id)) {
                    continue;
                }
                double nPE = calib->nPE();
                TotalQ += nPE;
            }
            hTotalPE_Org->Fill(TotalQ);
        }
        delete CalibEvt;
        delete calib_ch;
        delete calibf;
        delete usercalib_ch;
        delete usercalibf;

    }

    int TotalMaxBin = hTotalPE_Org->GetMaximumBin();
    double a = hTotalPE_Org->GetBinCenter(TotalMaxBin-10);
    double b = hTotalPE_Org->GetBinCenter(TotalMaxBin+10);
    hTotalPE_Org->Fit("gaus", "","",a,b);
    TF1* fitGaus = hTotalPE_Org->GetFunction("gaus");
    a = fitGaus->GetParameter(1) - 3.5*fitGaus->GetParameter(2);
    b = fitGaus->GetParameter(1) + 3.5*fitGaus->GetParameter(2);
    std::cout<<a<<'\t'<<b<<std::endl;


    TObjArray* calibFileElements = calibChain->GetListOfFiles();
    TIter calibnext(calibFileElements);
    TChainElement* calibChEl = 0;

    TObjArray* usercalibFileElements = usercalibChain->GetListOfFiles();
    TIter usercalibnext(usercalibFileElements);
    TChainElement* usercalibChEl = 0;

    
    //debug hist
    TH1D* hFHT = new TH1D("hFHT", "hFHT", ADCL, 0, ADCL);
    TH1D* hTotalPE = new TH1D("hTotalPE", "hTotalPE", 1000, 0, 20000);
    TH1D* hTotalPE_Sel = new TH1D("hTotalPE_Sel", "hTotalPE_Sel", 1000, 0, 20000);

    kk =-1; 
    while (( calibChEl=(TChainElement*)calibnext() )){
        //std::cout<<"---------------------"<<kk<<"---------------------"<<std::endl;
        TFile* calibf = TFile::Open(calibChEl->GetTitle());
        std::cout <<":" << calibChEl->GetTitle() << std::endl;
        if(calibf->IsZombie()) continue;
        if(calibf->TestBit(TFile::kRecovered)) continue;
        TTree* calib_ch = (TTree*)calibf -> Get("Event/Calib/CalibEvent");
        if(calib_ch->IsZombie()) continue;

        JM::CalibEvent* CalibEvt = new JM::CalibEvent();
        calib_ch->SetBranchAddress("CalibEvent",&CalibEvt);
        evtNum = calib_ch->GetEntries();

        std::cout<<evtNum<<std::endl;

        for(int ri=0; ri<evtNum; ri++){ 
        //for(int ri=0; ri<1; ri++){
           
            kk++;
            if(enableAmC) {
                if(kk==0) continue;
                if(EvtTArray.at(kk) - EvtTArray.at(kk-1) > 1.e6 || ri==0) continue;
            }
 
            double TotalQ = 0.;
            calib_ch->GetEntry(ri);
            const std::list<JM::CalibPMTChannel*>& chhlist = CalibEvt->calibPMTCol();
            std::list<JM::CalibPMTChannel*>::const_iterator chit = chhlist.begin();

            
            TH1D* FHSA = (TH1D*)gROOT->FindObject(Form("FHT_Evt%d", ri-1));
            delete FHSA;
            TH1D* FHSB = (TH1D*)gROOT->FindObject(Form("FHT_TOF_Evt%d", ri-1));
            delete FHSB;
            FHSA = new TH1D(Form("FHT_Evt%d", ri), Form("FHT_Evt%d", ri), int(ADCL/LFHSBinW), 0, ADCL);
            FHSB = new TH1D(Form("FHT_TOF_Evt%d", ri), Form("FHT_TOF_Evt%d", ri), int(ADCL/LFHSBinW), 0, ADCL);

            TH1D* hRSDT = new TH1D(Form("hRSDT_%d", ri), Form("hRSDT_%d", ri), 150, 0., tRange); //4 ns bin width for alignment

            int HitPMTN = chhlist.size();
            double TOF[MaxPMTNum];
            double m_FHT[MaxPMTNum];
            double m_RSDT[MaxPMTNum];
            for(int i=0;i<MaxPMTNum;i++) {TOF[i] = 0.0; m_FHT[i] = 0.; m_RSDT[i] = 0.;}

            for(int ii=0;ii<TotalLPMT;++ii) {
                m_nPE[ii]=0;
                m_AllQ[ii]=0;
            }
            while (chit!=chhlist.end()) {
                JM::CalibPMTChannel* calib = *chit++;

                unsigned int pmtId = calib->pmtId();
                Identifier id = Identifier(pmtId);
                int pmtid = CdID::module(id);
                if (not pmtid) {
                    continue;
                }
				if(pmtid>=TotalLPMT) continue;
                double nPE = calib->nPE();
                TotalQ += nPE;
                Readout_hit_time = calib->time();
                double firstHitTime = calib->firstHitTime();
                double firstHitTime_v = firstHitTime;

                if(Readout_hit_time.size()) firstHitTime_v = Readout_hit_time[0];
                m_FHT[pmtid] = firstHitTime;

                PmtGeom *pmt = m_cdGeom->getPmt(id);
                if ( !pmt ) {
                    LogError << "Wrong Pmt Id " << id << std::endl;
                }
                TVector3 pmtCenter = pmt->getCenter();
                /*
                double pmt_pos_x = pmtCenter.X();
                double pmt_pos_y = pmtCenter.Y();
                double pmt_pos_z = pmtCenter.Z();
                */

                double pmt_pos_x = PMTX[pmtid];
                double pmt_pos_y = PMTY[pmtid];
                double pmt_pos_z = PMTZ[pmtid];
                
                double Rsp = sqrt( (pmt_pos_x-PositionX)*(pmt_pos_x-PositionX) 
                                 + (pmt_pos_y-0.)*(pmt_pos_y-0.) 
                                 + (pmt_pos_z-PositionZ)*(pmt_pos_z-PositionZ));

                double CosThetaVC = (Rsp*Rsp + PMT_R*PMT_R*1.e6 - EvtR*EvtR)/(Rsp*PMT_R*1.e3*2.);
                double LengthWater = 1.e3*PMT_R*CosThetaVC - 1.e3*sqrt(PMT_R*CosThetaVC*PMT_R*CosThetaVC - PMT_R*PMT_R + LS_R*LS_R);

                TOF[pmtid] = RfrIndxLS*(Rsp - LengthWater)*1.e6/c + RfrIndxWR*LengthWater*1.e6/c;

                FHSA->Fill(firstHitTime_v);
                FHSB->Fill(firstHitTime_v - TOF[pmtid] - timeOffset[pmtid]);

                hFHT->Fill(firstHitTime_v - TOF[pmtid] - timeOffset[pmtid]);
                //hFHT->Fill(firstHitTime_v - firstHitTime);

            }

            if(TotalQ < a || TotalQ > b) continue;

            //Remove dark noise
            int HitMaxBin = FHSB->GetMaximumBin();
            int WindowA = 0, WindowB = 0;
            double SignalThres = (LDarkMu * double(TotalLPMT))/(ADCL/LFHSBinW) + sqrt((LDarkMu * double(TotalLPMT))/(ADCL/LFHSBinW));
            for(int i = HitMaxBin;i>2; i--) {
                if(FHSB->GetBinContent(i)+FHSB->GetBinContent(i-1)+FHSB->GetBinContent(i-2)<SignalThres*3.0) {
                    WindowA = LFHSBinW/2. + (double(i)-1.0)*LFHSBinW - 3.*LFHSBinW;break;
                }
                else continue;
            }
            WindowB = SignalWindow + WindowA;

            chit = chhlist.begin();
            double nPE_Temp = 0.0;
            double TrueHitTime = 0.0;
            while (chit!=chhlist.end()) {
                const JM::CalibPMTChannel  *calib = *chit++; nPE_Temp = 0.0;

                unsigned int pmtId = calib->pmtId();
                Identifier id = Identifier(pmtId);
                int pmtid =  CdID::module(id);
                if (not pmtid) {
                    continue;
                }
                if(pmtid>=TotalLPMT) continue; 
                Readout_hit_time = calib->time();
                Readout_PE = calib->charge();
                std::vector<double>::iterator ChargeIter   = Readout_PE.begin(), ChargeEndIter = Readout_PE.end();
                std::vector<double>::iterator HitTimeIter   = Readout_hit_time.begin(), HitTimeEndIter = Readout_hit_time.end();

                while(ChargeIter!=ChargeEndIter) {
                    TrueHitTime = (*HitTimeIter) - TOF[pmtid] - timeOffset[pmtid];
                    if(TrueHitTime < WindowA-1.0) {ChargeIter++;HitTimeIter++;continue;}
                    else if(TrueHitTime < WindowB+1.0) {nPE_Temp += *ChargeIter;ChargeIter++;HitTimeIter++;}
                    else break;
                }
                m_nPE[pmtid] = nPE_Temp;
                m_AllQ[pmtid] = calib->nPE();;
            }
            hTotalPE->Fill(TotalQ);

            //move the peak of the time pdf to 100 ns
            for(int j=0;j<MaxPMTNum;j++) {
                if(m_FHT[j]!=0.)  m_RSDT[j] = m_FHT[j] - TOF[j] - timeOffset[j] + 100.;
                if(m_RSDT[j]!=0.) {hRSDT->Fill(m_RSDT[j]); }
            }
            int HitMaxBinRec = hRSDT->GetMaximumBin();
            double t0Rec = hRSDT->GetBinCenter(HitMaxBinRec) - 100;
            if(kk<10) {
                distribution->cd();
                hRSDT->Write();
            }
            delete hRSDT;
 
            //save hit info
            double totalPE_S = 0., totalDC_S = 0.;
            for (int j=0;j<TotalLPMT;j++) {            // step3 : Fill TProfile
                double QRec = m_nPE[j]/PDE[j]/Gain[j];
                double AllQ = m_AllQ[j]/PDE[j]/Gain[j];

                
                PEdistribution->Fill(PMTTheta[j], QRec, 1);
                dPEdistribution->Fill(PMTTheta[j], DC[j]*SignalWindow/1.e9/PDE[j], 1);

                totalPE_S += QRec;
                totalDC_S += DC[j]*SignalWindow/1.e9/PDE[j];

                double recFired = 0.;
                if(m_nPE[j]>1e-8) recFired = 1.;
                RecNPMTdistribution->Fill(PMTTheta[j], recFired/PDE[j], 1);


                int QBinRec = 0;
                if(AllQ<PedThres) QBinRec = 0.;
                else if(AllQ<1.01) QBinRec = 1.;
                else if(AllQ<2.5) QBinRec = 2.;
                else if(AllQ<3.5) QBinRec = 3.;
                else QBinRec = 4.;

                int DBin = DAxis->FindBin(PMTDist[j]);
                int TBinRec = TAxis->FindBin(m_RSDT[j] - t0Rec);

                if(QBinRec==0) continue;


                if(QBinRec==1) {
                    if(IsDyn[j]) T1vsD_Dyn->Fill(m_RSDT[j] - t0Rec);
                    else         T1vsD_Mcp->Fill(m_RSDT[j] - t0Rec);     
                }

                else if(QBinRec==2) {
                    if(IsDyn[j]) T2vsD_Dyn->Fill(m_RSDT[j] - t0Rec);
                    else         T2vsD_Mcp->Fill(m_RSDT[j] - t0Rec);
                }

                if(QBinRec<=qBinN && DBin<=dBinN && TBinRec<=tBinN && QBinRec>0 && DBin>0 && TBinRec>0) {
                    if(IsDyn[j]) {
                        (*(BinTotalRecDyn_ + (QBinRec-1)*dBinN + DBin-1)) ++;
                        (*(RECvsTRUTHRecDyn_ + (QBinRec-1)*dBinN*tBinN + (DBin-1)*tBinN +  TBinRec-1)) ++;
                    } else {
                        (*(BinTotalRecMcp_ + (QBinRec-1)*dBinN + DBin-1)) ++;
                        (*(RECvsTRUTHRecMcp_ + (QBinRec-1)*dBinN*tBinN + (DBin-1)*tBinN +  TBinRec-1)) ++;
                    }

                }
 
            }
            hTotalPE_Sel->Fill(totalPE_S - totalDC_S);
        }

        delete CalibEvt;
        delete calib_ch;
        delete calibf;

    }

    std::vector<double> RecMean_NPMT;    
    int Npoint = 0, recNPMTpoint = 0;
    for (int i=0;i<BinNumOfTheta;i++) {
        double tmp_avgPE = PEdistribution->GetBinContent(i+1) - dPEdistribution->GetBinContent(i+1);
        if (tmp_avgPE != 0) {
            Npoint++;
            m_theta.push_back(PEdistribution->GetBinCenter(i+1));
            Mean_PE.push_back(tmp_avgPE);
        }
    }
    

    for (int i=0;i<BinNumOfTheta;i++) {
        double tmp_RecAvgPE = -log(1. - RecNPMTdistribution->GetBinContent(i+1)) - dPEdistribution->GetBinContent(i+1);
        if (tmp_RecAvgPE != 0) {
            recNPMTpoint++;
            RecMean_NPMT.push_back(tmp_RecAvgPE);
        }
    }

    TGraph* RecMeanPEdistribution = new TGraph(Npoint,&m_theta[0],&Mean_PE[0]);
    RecMeanPEdistribution->SetName("RecGraph");
    RecMeanPEdistribution->SetTitle(Form("PE-Response@(R-%.0f, #theta-%.0f)", EvtR, EventTheta*180./TMath::Pi()));
    RecMeanPEdistribution->GetXaxis()->SetTitle("Angle [Degree]");
    RecMeanPEdistribution->GetYaxis()->SetTitle("Mean P.E.");

    TGraph* RecMeanNPMTdistribution = new TGraph(recNPMTpoint,&m_theta[0],&RecMean_NPMT[0]);
    RecMeanNPMTdistribution->SetName("RecNPMTGraph");
    RecMeanNPMTdistribution->SetLineColor(4);

    cHv_->cd();
    cHv_->Clear();
    RecMeanPEdistribution->Draw();
    cHv_->Print(plotName_ +".pdf","pdf");

    distribution->cd();
    RecMeanPEdistribution->Write();
    PEdistribution->Write();
    RecMeanNPMTdistribution->Write();
    hTotalPE_Org->Write();
    hTotalPE_Sel->Write();
    hTotalPE->Write();
    hFHT->Write();
    T1vsD_Dyn->Write();
    T2vsD_Dyn->Write();
    T1vsD_Mcp->Write();
    T2vsD_Mcp->Write();
    std::cout << "=== End of Generation ===" << std::endl;

    delete PEdistribution;
    delete RecMeanPEdistribution;
    delete distribution;
}



bool GenCalibPDF::finalize()
{
    LogInfo  << objName()
        << "   finalized successfully"
        << std::endl;
    return true;
}

    

