#include "/junofs/users/jiangw/include/pmtflag.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"
using namespace RooFit;
void rooRectimeH()
{
    const int n = 17612;
    const int MIN_BIN = 300;
    const int MAX_BIN = 600;
    bool *hmmt = pmtflag();
    gROOT->ProcessLine("#include <vector>");
    TFile *f = TFile::Open("time_corr.root");
    //TFile *ojw = TFile::Open("recTime.root","recreate");
    TTree *t = (TTree*)f->Get("TIMECORR");
    std::vector<int> *pmtID = new std::vector<int>;
    std::vector<double> *recTime = new std::vector<double>;
    t->SetBranchAddress("pmtID",&pmtID);
    t->SetBranchAddress("recTime",&recTime);

    double hitTime;
    TTree *time[n];// = new TTree("time","time");
    for(int i=0; i<n; ++i)
    {
        TString name = Form("ch%d_time",i);
        time[i] = new TTree(name,name);
        //time->Branch("pmtid",&pmtid,"pmtid/I");
        time[i]->Branch("hitTime",&hitTime,"hitTime/D");
    }
    TH1D *ch[17612];// = new TH1D("ch0","ch0_detltaT",1250,0,1250);
    for(int i=0; i<17612; ++i)
    {
        TString chName1=Form("ch%d_Time",i);
        ch[i] = new TH1D(chName1,chName1,MAX_BIN-MIN_BIN,MIN_BIN,MAX_BIN);
    }

    cout<< "total event: "<<t->GetEntries()<<endl;
    for(int i=0; i<t->GetEntries(); ++i)
    {
        pmtID->clear();
        recTime->clear();
        
        if(i%1000==0) cout<<"Transforming data: "<<i<<"/"<<t->GetEntries()<<endl;
        t->GetEntry(i);
        for(int j=0; j<recTime->size(); ++j)
        {
            if(recTime->at(j)<MIN_BIN||recTime->at(j)>MAX_BIN||pmtID->at(j)>17611) continue;
			//if(pmtID->at(j)!=11331) continue;
            //pmtid = pmtID->at(j);
            hitTime = recTime->at(j);
            time[pmtID->at(j)]->Fill();
            ch[pmtID->at(j)]->Fill(hitTime);
        }
    }
    
    double deltaT[n]={0};
    double deltaT_error[n]={0};
    double deltaT_sigma[n]={0};
    double sigma_error[n]={0};
    double chi2NDF[n]={0};
	int pn[2] = {13,7};
    for(int i=0;i<n;++i)
    //for(int i=11331;i<11332;++i)
    {
        cout<<"*******************now proccessing pmt: "<<i<<" ***********************"<<endl;
		
		int pos = ch[i]->GetMaximumBin();
		double peak = ch[i]->GetBinContent(pos);
		int tL,tR;
		double threshold = peak*0.5;
		for(int j=1; j<pos; j++){
            if(ch[i]->GetBinContent(pos-j) <= threshold){
            	tL = 300+pos-j; break;
            }
        }
		for(int j=1; j<pos; j++){
            if(ch[i]->GetBinContent(pos+j) <= threshold){
                tR = 300+pos+j; break;
            }
        }
		double mean_init=0;
		for(int k=0;k<pn[int(hmmt[i])];++k){
			int peak_pos = ch[i]->GetMaximumBin();
			//cout <<k<<": "<< peak_pos <<endl;
			mean_init += peak_pos;
			ch[i]->SetBinContent(peak_pos,0);
		}
        mean_init = round(mean_init/pn[int(hmmt[i])]) + MIN_BIN;
		//cout <<"avg:" << mean_init<<endl;        
	
		RooRealVar hitTime("hitTime", "hitTime", MIN_BIN, MAX_BIN);
        RooRealVar fitmean("fitmean","mean of gaussian", mean_init, mean_init-10, mean_init+10);
        RooRealVar fitsigma("fitsigma","sigma of gaussian", 5.0,0.1,20.0);
        RooRealVar fitA("fitA","A of gaussian",peak, peak-50, peak+50);
		//cout<<"mean_init: "<<mean_init<<", peak: "<<peak<<endl;
        if(hmmt[i])
        {
            hitTime.setRange(mean_init-7,mean_init+3);
            fitsigma.setVal(2.0);
            fitsigma.setRange(0.1, 5.0);
        }
        else
        {
            hitTime.setRange(mean_init-10, mean_init+7);
            fitsigma.setVal(6.0);
            fitsigma.setRange(1.0, 15.);
        }
		hitTime.setRange(tL,tR);
        RooGenericPdf gauss("gauss model","fitA * TMath::Exp(-(hitTime- fitmean)*(hitTime - fitmean)/2/fitsigma/fitsigma)", RooArgSet(hitTime,fitA,fitmean,fitsigma));
        
        RooPlot *xframe = hitTime.frame(Title("Gaussian p.d.f"));
        RooDataSet ds("ds","ds",hitTime,Import(*time[i]));
        ds.plotOn(xframe);

        gauss.fitTo(ds);
        gauss.plotOn(xframe);
        gauss.paramOn(xframe, Layout(0.55));

        Double_t chi2 = xframe->chiSquare();
        //cout <<"pmtID: " << i <<  "  chi2/ndf: " << chi2 << endl;

        deltaT[i] = fitmean.getVal();
        deltaT_error[i] = fitmean.getError();
        deltaT_sigma[i] = fitsigma.getVal();
        sigma_error[i] = fitsigma.getError();
        chi2NDF[i] = chi2;

        time[i]->Delete();
		//cout << "result:: "<<deltaT[i]<<endl;
    }
    f->Close();
    //TFile *ojw = TFile::Open("recTime.root","recreate");
    // ojw->cd();
    // for(int i=0; i<17612; ++i)
    // {
    //     time[i]->Write();
    // }
    // ojw->Close();

    ofstream out("timeOffset_roofit.txt");
    for(int i=0; i<n;i++){ out << i << "\t"<< deltaT[i] << "\t" <<deltaT_error[i] << "\t" 
        << deltaT_sigma[i] <<"\t" << sigma_error[i] << "\t" << chi2NDF[i]<< "\n";   }

    return;
}
