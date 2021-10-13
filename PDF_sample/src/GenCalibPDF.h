#ifndef GenCalibPDF_h
#define GenCalibPDF_h

#include "EvtNavigator/NavBuffer.h"
#include "TString.h"
#include "Minuit2/FCNBase.h"
#include "SniperKernel/AlgBase.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TChainElement.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TString.h"
#include "TProfile.h"
#include "RootWriter/RootWriter.h"
#include "TGraph.h"
#include <string>
#include <map>
#include <vector>
#include "TAxis.h"
#include "TRandom3.h"

class RecGeomSvc;
class CdGeom;

const int CalibPosNum = 311;
const int MaxPMTNum = 20000;

const int qBinN = 50;
const int dBinN = 200;
const int tBinN = 400;


const double qRange = 10.;
const double dRange = 40.;
const double tRange = 600.;

const double c = 2.99792458e8;

const double LFHSBinW = 5.;
const double ADCL = 1250.;


class TTree;
class GenCalibPDF: public AlgBase
{
    public:
        GenCalibPDF(const std::string& name);
        ~GenCalibPDF();

        bool initialize();
        bool execute();
        bool finalize();
        void GenPDF(const char *SavePath, TChain* calibChain, TChain* usercalibChain, int PlaceID, const double PositionX, const double PositionZ, 
                    std::vector<double>& m_theta, std::vector<double>& Mean_PE, TCanvas* cHv_, TString plotName_, 
                    double* BinTotalRecDyn_, double* RECvsTRUTHRecDyn_, double* BinTotalRecMcp_, double* RECvsTRUTHRecMcp_);
        
    private:
        std::vector<double> Readout_PE;
        std::vector<double> Readout_hit_time;
        std::string calibdir;
        std::string usercalibdir;
        std::string posdir;
        std::string paradir;
        std::string calibsource;
        int TotalLPMT;
        int TotalSPMT;
        double PMT_R;
        double Ball_R;
        double LS_R; 
        std::vector<TVector3> PMT_pos;
        CdGeom*  m_cdGeom;
        int jobA;
        int jobB;
        std::string PMT_DATA;

        double SignalWindow;
        double LDarkMu;
        double PedThres;
        std::vector<TH1F*>  SPEs;
        std::vector<double> timeOffset;
        std::vector<double> Gain;
        std::vector<double> QRes;
        std::vector<double> PDE;
        std::vector<double> DC;

        double* pmt_theta;
        double* pmt_phi;

        double RfrIndxLS;
        double RfrIndxWR;

        std::vector<TVector3> ALL_LPMT_pos;
        bool enableTruePMTPara;
        bool enableAmC;
        bool* IsDyn;
        int PMTtype;
        int filenum;
        int startseed;
        
};


#endif
