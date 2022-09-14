#include "/junofs/users/jiangw/include/pmtflag.h"
#include "/junofs/users/jiangw/include/CdID2pmtID.h"
const int adcl = ADCL;
void seeHitTime(){
    bool *hmmtpmt = pmtflag();
    gROOT->ProcessLine("#include <vector>");
    TChain *chain = new TChain("calibevt");
    chain->Add("user-calib-*");
    std::vector<float> *Time = new std::vector<float>;
    std::vector<int> *detID = new std::vector<int>;
    chain->SetBranchAddress("Time",&Time);
    chain->SetBranchAddress("PMTID",&detID);

    TH1D *h = new TH1D("hitTime","hitTime",adcl,0,adcl);
    cout<< "total event: "<<chain->GetEntries()<<endl;
    for(int i=0; i<chain->GetEntries(); ++i){//chain->GetEntries()
        Time->clear();
        detID->clear();
        if(i%1000==0) cout<<"Proccessing "<<i<<endl;
        chain->GetEntry(i);
        for(int j=0; j<Time->size(); ++j){
            if(!hmmtpmt[CdID2pmtID(detID->at(j))])
                h->Fill(Time->at(j));
        }
    }
    delete Time;
    delete detID;
    h->Draw();
}