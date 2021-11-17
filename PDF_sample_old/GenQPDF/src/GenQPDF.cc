#include "GenQPDF.h"
#include "TMath.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TGraphSmooth.h"
#include "TString.h"
#include "TStopwatch.h"
#include "Minuit2/FCNBase.h"
#include "SniperKernel/AlgFactory.h"
#include "Event/RecHeader.h"
#include "Event/CalibHeader.h"
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
#include "TROOT.h"
#include <Math/Functor.h> 
#include <Math/OneDimFunctionAdapter.h> 
#include <functional> 
#include "Math/Integrator.h"

namespace fs = boost::filesystem;

DECLARE_ALGORITHM(GenQPDF);


GenQPDF::GenQPDF(const std::string& name)
:   AlgBase(name)
{
    m_iEvt = -1.;
    PMTID = 0;
    TotalPMT = 17612;
    qThres = 0.125;
    aThres = 0.1;
    MoveL = 5;
    high_mu = 0.5;

    spe_bin_width = 0.05;
    spebina = -2.0;
    spebinb = 18.;
    spebin_num = (spebinb-spebina)/spe_bin_width;
    startBin = 1;
    endBin = 1;

    SPEGain = 1.;
    SPESgima = 0.3;
    
    //////////////
    // run options
    peRange = 1;
    pdfNSample = 1E7;
    enableHighMuFit = false;
    enableUserSPEs = false;
    userSPEFile = "";
    jobA = 0;
    jobB = 1;
    runmode = 1;
    outdir = "Test";

    declProp("jobA", jobA = 0);
    declProp("jobB", jobB = 1);
    declProp("peRange", peRange = 1);
    declProp("runmode", runmode = 1);
    declProp("enableHighMuFit", enableHighMuFit = false);
    declProp("enableUserSPEs", enableUserSPEs = false);
    declProp("userSPEFile", userSPEFile);
    declProp("outdir", outdir = "Test");

}


GenQPDF::~GenQPDF()
{
    std::vector< TH1D* >::iterator hIterEnd = HSPEs.end();
    std::vector< TGraph* >::iterator GraphIterEnd = GrSPEs.end();
    hIter = HSPEs.begin();
    GraphIter = GrSPEs.begin();
    while(hIter!=hIterEnd) {delete *hIter;*hIter = NULL; ++hIter;}
    while(GraphIter!=GraphIterEnd) {delete *GraphIter;*GraphIter=NULL;++GraphIter;}

    hIterEnd = HNPEs.end();
    hIter = HNPEs.begin();
    while(hIter!=hIterEnd) {delete *hIter;*hIter = NULL; ++hIter;}
    
}



class  PrdSPEs {
    public:
    PrdSPEs(TGraph* SPEs_):grSPEs(SPEs_){}
    PrdSPEs(){}
    void SetSpec(TGraph* SPEs_) {
        grSPEs = SPEs_;
    }

    Double_t operator()(Double_t* x, Double_t* par) {
        double result = grSPEs->Eval(x[0]);
        if(result>1e-16) return grSPEs->Eval(x[0]);
        else return 1e-16;
    }
    private:
    TGraph* grSPEs;
};



bool GenQPDF::initialize()
{
    SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
    if (calSvc.invalid()) {
        LogError << "Can't Locate  PMTCalibSvc."
                 << std::endl;
        return false;
    }

    hChi2_ndf = new TH1F("hChi2_ndf", "hChi2_ndf", 400, 0, 5); 
    hMu = new TH1F("hMu", "hMu", 200, 0, 2);
    hGain_Smooth = new TH1F("hGain_Smooth", "hGain_Smooth", 200, 0.8, 1.2);
    hGain_Hist = new TH1F("hGain_Hist", "hGain_Hist", 200, 0.8, 1.2); 
    hMu_Compare = new TH1F("hMu_Compare", "hMu_Compare", 1000, 0, 10);
    ///////////////////////
    // Loading SPE spectrum
    ///////////////////////

    if((calSvc->getRelativeDE()).at(0)>1e-8) PDE = calSvc->getRelativeDE();
    LogDebug<<"PDE[0]:"<<PDE[0]<<std::endl;

    if((calSvc->getDarkRate()).at(0)>1e-8) DC = calSvc->getDarkRate();
    LogDebug<<"DC[0]:"<<DC[0]<<std::endl;

    if(enableUserSPEs) {
        TFile* SPEsFile = TFile::Open(userSPEFile.c_str(), "READ");
        for(int i=0;i<TotalPMT;i++) {
            TH1D* spe_temp = (TH1D*)SPEsFile->Get(Form("ch%d_charge_spec", i));
            HSPEs.push_back(spe_temp);
        }
    }
    else HSPEs = calSvc->getChargeSpec(); 
    

    HSPEs[0]->Rebin(1);
    double bin_width_temp = HSPEs[0]->GetBinWidth(1);
    int nbin_temp = HSPEs[0]->GetNbinsX();

    SetSPEsHistPara(nbin_temp, HSPEs[0]->GetBinCenter(1) - bin_width_temp/2., HSPEs[0]->GetBinCenter(nbin_temp) + bin_width_temp/2., bin_width_temp);
    PrintHistInfo();

    LogInfo<<"Total number of PMTs:"<<TotalPMT<<std::endl;
    
    startBin = HSPEs[0]->FindBin(qThres);
    endBin = HSPEs[0]->FindBin(spebinb);

    for(int i=0;i<TotalPMT;i++) {
        if(i>0) HSPEs[i]->Rebin(1);
        double EntrySPEs = HSPEs[i]->Integral(startBin, endBin);
        HSPEs[i]->Scale(1./EntrySPEs);
    }
    ///////////////////////////
    // Loading high mu spectrum
    ///////////////////////////
    if(enableHighMuFit) {
        for(int i=0;i<TotalPMT;i++) {
            DC[i] *= 1250.;
            DC[i] /= 1.0e9;
        }
        TFile* NPEsFile = TFile::Open("/junofs/users/zhangxt/20inch/rec/deconvolution/runs/20191009samplephy/Laser/center/photon_110000/user_calibCorr.root", "READ");
        for(int i=0;i<TotalPMT;i++) {
            TH1D* npes_temp = (TH1D*)NPEsFile->Get(Form("ch%d_charge_spec", i));
            npes_temp->SetName(Form("ch%d_highMu_charge_spec", i));
            HNPEs.push_back(npes_temp);
            double N = npes_temp->Integral(startBin, endBin);
            double mu_cal = -log((19800.-N)/19800.) - DC[i];
            hMu->Fill(mu_cal);
            hMu_Compare->Fill(mu_cal/PDE[i]);
        }
    }
    

    fcn = new MyFCN(this);
    qminuit = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");

    m_PDFOutput = new TFile(Form("%s/NPEPDF/SPEsQPDF_%d.root", outdir.c_str(), jobA), "RECREATE");
    hMu->Write();
    hMu_Compare->Write();
    //if(runmode == 2) m_PDFInput = TFile::Open("NPEQpdf.root", "READ");
    //m_FitOutput = new TFile("FitResultsJ19.root", "RECREATE");

    cHSpes = new TCanvas("cHSpes","Graph Example",200,10,700,500);
    cHSpes->SetLogy();
    plotName = Form("%s/PHOLog/QSpecFitPlots_run%d", outdir.c_str(), jobA);
    if(enableHighMuFit) cHSpes->Print(plotName+".pdf[", "pdf");
 
    PMTID = jobA;


    LogInfo  << objName()
        << "   initialized successfully"
        << std::endl; 

    return true; 
}


bool GenQPDF::execute()
{
    m_iEvt++;
    
    // std::ofstream fitResultOut;
//if(enableHighMuFit) fitResultOut.open(Form("%s/FITLog/fitResult_%d.txt", outdir.c_str(), jobA), std::ios::out);
    double gain_smooth = 0., gain_hist = 0., gain_fit = 0.;

    for(PMTID=jobA;PMTID<jobB&&PMTID<TotalPMT;PMTID++) {
        
        LogDebug << " ---------------------------------PMT channel:" << PMTID <<std::endl;
        TH1F* hSpec = new TH1F("hSpec", "hSpec", spebin_num, spebina, spebinb);

        int bintemp1 = HSPEs[0]->FindBin(qThres);
        int bintemp2 = HSPEs[0]->FindBin(spebinb);
        for(int i=bintemp1;i<=bintemp2;i++) {
            hSpec->SetBinContent(i, HSPEs[PMTID]->GetBinContent(i));
        }
        std::cout<<hSpec->GetSumOfWeights()<<std::endl;

        PrintHistInfo();

    
        nPEQPDF = new std::vector< TH1F* >;

        /*
        int endPos = spebin_num;
        PrimaryFit(hSpec, endPos);

        ROOT::Math::IntegratorOneDimOptions::SetDefaultRelTolerance(1.E-6);
        PrdSPEs* PrdSPEs_PMT = new PrdSPEs(GrSPEs[PMTID-jobA]);
        TF1* funcPrdSPEs_PMT = new TF1("funcPrdSPEs_PMT", PrdSPEs_PMT, qThres, hSpec->GetBinCenter(endPos), 0, "PrdSPEs");
        LogInfo<<"Upper charge limit of smooth SPEs:"<<hSpec->GetBinCenter(endPos)<<std::endl;
        */
        double* RecordQ_temp = (double*)malloc(sizeof(double)*pdfNSample);
        for(int nPE = 0; nPE < peRange; nPE ++) {
            LogInfo<<"---------------------------------:"<<nPE<<std::endl;

            double* RecordQ = (double*)malloc(sizeof(double)*pdfNSample);

            TH1F* htemp = new TH1F(Form("channel%d_nPE%d_qpdf", PMTID, nPE+1), Form("channel%d_nPE%d_qpdf", PMTID, nPE+1), 200./spe_bin_width, 0, 200);
            nPEQPDF->push_back(htemp);
            for(int i=0;i<pdfNSample;i++) {   
                //double temp = funcPrdSPEs_PMT->GetRandom();
                double temp = hSpec->GetRandom();

                if(nPE==0) *(RecordQ + i) = temp;
                else *(RecordQ + i) = temp + *(RecordQ_temp + i);
                
                *(RecordQ_temp + i) = *(RecordQ + i);

                nPEQPDF->at(nPE)->Fill(*(RecordQ + i));
            }
            free(RecordQ);
            double entry_temp = nPEQPDF->at(nPE)->GetSumOfWeights();
            nPEQPDF->at(nPE)->Scale(1./entry_temp);
            if(runmode == 2) {
                m_PDFOutput->cd();
                nPEQPDF->at(nPE)->Write();
            }

            if(nPE==0) {
                gain_smooth = nPEQPDF->at(nPE)->GetMean();
                
                /*
                int bintemp1 = HSPEs[0]->FindBin(qThres);
                TH1F* hSpec = (TH1F*)HSPEs[PMTID]->Clone("hSpec");
                for(int i=1;i<bintemp1;i++) {
                    hSpec->SetBinContent(i, 0.);
                }*/
                gain_hist = hSpec->GetMean();
                hGain_Hist->Fill(gain_hist);

                hSpec->Fit("gaus", "MREQ0", "", 0.5, 1.5);
                TF1* GausTemp = (TF1*)hSpec->GetFunction("gaus");
                gain_fit = GausTemp->GetParameter(1);
                SPEGain = gain_fit;
                SPESgima = GausTemp->GetParameter(2);

                delete GausTemp;
            }

        }
        free(RecordQ_temp);

        //delete PrdSPEs_PMT;
        //delete funcPrdSPEs_PMT;
        
        if(enableHighMuFit) {
            ROOT::Math::Functor recfunc(*fcn, 2);
            qminuit->SetFunction(recfunc);
            qminuit->SetMaxFunctionCalls(1E6);
            qminuit->SetMaxIterations(1E6);
            qminuit->SetTolerance(1e-6);

            
            double Entry = HNPEs[PMTID]->Integral(startBin, endBin);
            qminuit->SetVariable(0,"mu", high_mu, 0.001);
            qminuit->SetVariable(1,"evts", Entry, 1.);

            qminuit->SetUpperLimitedVariable(0, "mu", high_mu, 0.001, 200);
            qminuit->SetLowerLimitedVariable(0, "mu", high_mu, 0.001, 1.e-12);

            qminuit->SetFixedVariable(1, "evts", Entry);

            int ief = qminuit->Minimize();
            LogDebug << "The ief is: " << ief << std::endl;
            const double *xs = qminuit->X();
            const double *xe = qminuit->Errors();
            double mu_fit = xs[0];
            double N_fit = xs[1];
            double mu_fit_err = xe[0];
            double chi2_ndf = m_Calculate_Chi2(mu_fit, N_fit);
        
            mu_fit -= DC[PMTID];
            double mu_cal = -log((19800.-N_fit)/19800.) - DC[PMTID];

            // fitResultOut<<PMTID<<'\t'<<chi2_ndf<<'\t'<<mu_fit<<'\t'<<mu_fit_err<<'\t'<<mu_cal<<'\t'<<mu_fit/PDE[PMTID]<<'\t'<<gain_smooth<<'\t'<<gain_hist<<'\t'<<gain_fit<<std::endl;
            if(chi2_ndf>2.) LogDebug<<"Found largeChi2 !"<<std::endl;

            LogDebug<<"==========================================================="<<std::endl;
            LogDebug<<"The fitted mu is "<<mu_fit<<'\t'<<mu_cal<<'\t'<<mu_fit_err<<std::endl;
            LogDebug<<"The gain is "<<gain_smooth<<'\t'<<gain_hist<<'\t'<<gain_fit<<std::endl;
            LogDebug<<"The fitted evts is "<<N_fit<<std::endl;
            LogDebug<<"The fitted chi2/ndf is "<<chi2_ndf<<std::endl;
            LogDebug<<"==========================================================="<<std::endl;
            TH1F* FitResults = new TH1F(Form("FitQSpec_%d",PMTID), Form("FitQSpec_%d",PMTID), spebin_num, spebina, spebinb);
            double binwf = HNPEs[PMTID]->GetBinWidth(1)/nPEQPDF->at(0)->GetBinWidth(1);
            binwf = HNPEs[PMTID]->GetBinWidth(1);
            std::cout<<binwf<<std::endl;
            double Q1 = SPEGain;
            double S1 = SPESgima;
            int kRange = 8;
            for(int i=1;i<HNPEs[PMTID]->GetNbinsX();i++) {
                double X = HNPEs[PMTID]->GetBinCenter(i);
                double Y = 0.;
                for(int npe=1;npe<kRange;npe++) {
                    double kk = double(npe);
                    //double prob_temp = nPEQPDF->at(npe-1)->Interpolate(X);
                    double prob_temp =  TMath::Gaus(X, kk*Q1, sqrt(kk)*S1)/sqrt(2.*TMath::Pi()*kk*S1*S1);
                    if(prob_temp<1e-12) continue;
                    Y += TMath::Poisson(double(npe), mu_fit)*prob_temp*binwf;
                }
                Y *= 1./(1.-exp(-mu_fit));
                FitResults->SetBinContent(i, N_fit*Y);
            }
            /*
            m_FitOutput->cd();
            if(PMTID==2) {
                FitResults->Write();      
                HNPEs[2]->Write();
            }*/
            if(runmode==1) {
                if(PMTID<100) {
                    cHSpes->cd();
                    cHSpes->Clear();
                    HNPEs[PMTID]->Draw();
                    HNPEs[PMTID]->SetTitle(Form("FitMu = %0.2f, Chi2/ndf = %0.2f", mu_fit, chi2_ndf));
                    HNPEs[PMTID]->GetXaxis()->SetRangeUser(-1, 10);
                    FitResults->SetLineColor(2);
                    FitResults->SetLineWidth(1);
                    FitResults->Draw("SAME");

                    cHSpes->Print(plotName+".pdf","pdf");
                }
            }
            delete FitResults;
        
            qminuit->Clear();

        } 
        delete hSpec;
        std::vector< TH1F* >::iterator hIterTemp = nPEQPDF->begin();
        while(hIterTemp!=nPEQPDF->end()) {delete *hIterTemp;*hIterTemp = NULL; ++hIterTemp;}
        delete nPEQPDF;       

    }    
    return true; 

}


double GenQPDF::m_Calculate_Chi2(double mu, double evtnum)
{
    double Chi2 = 0.;
    double binwf = HNPEs[PMTID]->GetBinWidth(1)/nPEQPDF->at(0)->GetBinWidth(1);
    binwf = HNPEs[PMTID]->GetBinWidth(1);
    double ndf = 0.;
    double Q1 = SPEGain;
    double S1 = SPESgima;
    int kRange = 8;
    for(int i=startBin;i<HNPEs[PMTID]->GetNbinsX();i++) {
        double X = HNPEs[PMTID]->GetBinCenter(i);
        double y = HNPEs[PMTID]->GetBinContent(i);
        double Y = 0.;
        if(y<10.) continue;
        for(int npe=1;npe<kRange;npe++) {
            double kk = double(npe);
            //double prob_temp = nPEQPDF->at(npe-1)->Interpolate(X);
            double prob_temp =  TMath::Gaus(X, kk*Q1, sqrt(kk)*S1)/sqrt(2.*TMath::Pi()*kk*S1*S1);
            if(prob_temp<1e-12) continue;
            Y += TMath::Poisson(double(npe), mu)*prob_temp*binwf;
        }
        Y *= 1./(1.-exp(-mu ));
        Chi2 += TMath::Power(y - evtnum*Y, 2.)/y;
        ndf ++;
    }
    //std::cout<<ndf<<'\t'<<Chi2/ndf<<std::endl;
    return Chi2/(ndf-1.);
}


bool GenQPDF::PrimaryFit(TH1F *hSpec_, int& endPos_) 
{
    
    TH1F* hSpec = (TH1F*)hSpec_->Clone("hSpec");
    int mark = 0;
    for(int i=hSpec->FindBin(1.);i<=spebin_num;i++) {
        if(hSpec->GetBinContent(i)==0) mark++;
        else mark = 0;
        if(mark>=5) {endPos_ = i; break;} 
    }
    /////////////////
    // Smooth process
    int Bb = hSpec->FindBin(qThres);
    int Be = hSpec->FindBin(spebinb) - MoveL;
    const int MoveG = Be - Bb;
    
    double subX[MoveG][MoveL];
    double subY[MoveG][MoveL];
    for(int i=0;i<MoveG;i++) {
        for(int j=0;j<MoveL;j++) {
            subX[i][j] = hSpec->GetBinCenter(Bb+i+j);
            subY[i][j] = hSpec->GetBinContent(Bb+i+j);
        }
    }
    
    TGraph* subGr[MoveG];
    TGraph* subGrSmooth[MoveG];
    TGraphSmooth* gs[MoveG];
    double xLimitA[MoveG];
    double xLimitB[MoveG];
    for(int i=0;i<MoveG;i++) {
        subGr[i] = new TGraph(MoveL, subX[i], subY[i]);
        subGr[i]->SetName(Form("subGr%d",i));
        gs[i] = new TGraphSmooth("normal");
        //subGrSmooth[i] = gs[i]->SmoothSuper(subGr[i],"",3);
        subGrSmooth[i] = gs[i]->SmoothLowess(subGr[i],"",0.2);

        subGrSmooth[i]->SetName(Form("subGrSmooth%d",i));
        xLimitA[i] = subGrSmooth[i]->GetX()[0];
        xLimitB[i] = subGrSmooth[i]->GetX()[MoveL-1];
    }
    
    double X[spebin_num];
    double Y[spebin_num]; 
    double AvgN[spebin_num];
    for(int i = 0;i<spebin_num;i++) {
        AvgN[i] = 0.;
        Y[i] = 0.;
        X[i] = hSpec->GetBinCenter(i+1);
        for(int s=0;s<MoveG;s++) {
            if(X[i]>xLimitA[s]&&X[i]<xLimitB[s]) {Y[i] += subGrSmooth[s]->Eval(X[i]); AvgN[i]++;}
        }
        if(AvgN[i]>1e-8) Y[i] /= AvgN[i];
    }

    TGraph* SPEsSmooth = new TGraph(endPos_, X, Y);
    SPEsSmooth->SetName(Form("channel%d_SPEsSmooth", PMTID));
    GrSPEs.push_back(SPEsSmooth);
    
    m_PDFOutput->cd();
    SPEsSmooth->Write();
    
    delete hSpec;
    return true;
}




bool GenQPDF::finalize()
{
    cHSpes->Print(plotName+".pdf]", "pdf");
    
    if(runmode == 2) {
        m_PDFOutput->cd();
        for(int pmtid=jobA;pmtid<jobB&&PMTID<TotalPMT;pmtid++) {
            HSPEs[pmtid]->Write();
            if(enableHighMuFit) HNPEs[pmtid]->Write();
        }
        m_PDFOutput->Close();
    }
    LogInfo  << objName()
        << "   finalized successfully" 
        << std::endl; 
    return true; 
}
