#ifndef GenQPDF_h 
#define GenQPDF_h 

#include "EvtNavigator/NavBuffer.h"
#include "TString.h"
#include "Minuit2/FCNBase.h"
#include "SniperKernel/AlgBase.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
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
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "PMTCalibSvc/PMTCalibSvc.h"

using  std::string;
class RecGeomSvc;
class CdGeom;
 
class TTree;

class GenQPDF: public AlgBase 
{
    public:
        GenQPDF(const std::string& name); 
        ~GenQPDF(); 

        bool initialize(); 
        bool execute(); 
        bool finalize(); 
        void SetSPEsHistPara(int spebin_num_, double spebina_, double spebinb_, double spe_bin_width_) {
            spebin_num = spebin_num_;
            spebina = spebina_;
            spebinb = spebinb_;
            spe_bin_width = spe_bin_width_;
        }
        void PrintHistInfo() {
            LogInfo<<"spebin_num: "<< spebin_num << '\t'
                <<"spebina: "<< spebina << '\t'
                <<"spebinb: "<< spebinb << '\t'
                <<"spe_bin_width: "<< spe_bin_width <<std::endl;
        }
        bool PrimaryFit(TH1F *hSpec, int& endPos);

        double m_Calculate_Chi2(double mu, double N);
        class MyFCN: public ROOT::Minuit2::FCNBase {
            public:
                MyFCN(GenQPDF *alg) {m_alg = alg;}
                double operator() (const std::vector<double>& x) const {
                    return m_alg->m_Calculate_Chi2(x[0], x[1]);
                }
                double operator() (const double * x)const{
                    std::vector<double> p(x, x+2);
                    return (*this)(p);
                }

                double Up() const {return 0.5;}
            private:
                GenQPDF *m_alg;
        };



    private:
        ROOT::Math::Minimizer* qminuit;
        MyFCN* fcn;

        int m_iEvt;
        int PMTID;
        int MoveL;
        int TotalPMT;
        double aThres;
        double qThres;

        int pdfNSample;
        int peRange;
        double high_mu;

        TCanvas* cHSpes;
        TString plotName;
        std::string outdir;
        ////////////////
        // SPE hist info
        ////////////////
        int spebin_num;
        double spebina;
        double spebinb;
        double spe_bin_width;

        /////////////
        // Calib data
        /////////////
        std::vector<double> Gain;
        std::vector<double> DC;
        std::vector<double> PDE;
        std::vector<TH1D*> HSPEs;
        std::vector<TH1D*> HNPEs;
        std::vector<TGraph*> GrSPEs;
        std::vector<TH1F*>* nPEQPDF;

        std::vector< TH1D* >::iterator hIter;
        std::vector< TGraph* >::iterator GraphIter;

        TFile* m_PDFOutput;
        TFile* m_PDFInput;
        TFile* m_FitOutput;

        int jobA;
        int jobB;
        int runmode;
        bool enableHighMuFit;
        bool enableUserSPEs;
        std::string userSPEFile;

        TH1F* hChi2_ndf;
        TH1F* hMu;
        TH1F* hGain_Smooth;
        TH1F* hGain_Hist;
        TH1F* hMu_Compare;

        double SPEGain;
        double SPESgima;

        int startBin;
        int endBin;

};
#endif
