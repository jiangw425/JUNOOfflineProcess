// #include "/junofs/users/jiangw/include/pmtflag.h"
void seeDetsimHittime_3(){
    // bool *hmmtpmt = pmtflag();
    gROOT->ProcessLine("#include <vector>");
    const int num_file = 3;
    std::string path[num_file] = {
        "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/00/ACU-CLS/Ge68/Ge68_0_0_0/detsim/user-root",
        "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/11/ACU-CLS/Ge68/Ge68_0_0_0/detsim/user-root",
        "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/ACU-CLS/Ge68/Ge68_0_0_0/detsim/user-root"
    };
    std::string fsname[num_file] = {"00","11","22"};
    TH1D *ht1D[num_file];
    
    for(int f=0;f<num_file;++f){
        TChain *chain = new TChain("evt");
        chain->Add(Form("%s/user-detsim-*",path[f].c_str()));
        std::vector<double> *htime = new std::vector<double>;
        chain->SetBranchAddress("hitTime",&htime);

        TString name = Form("%s_hitTime",fsname[f].c_str());
        ht1D[f] = new TH1D(name,name,3000,0,3000);

        cout<< "total event: "<<chain->GetEntries()<<endl;
        for(int i=0;i<chain->GetEntries(); ++i){
        // for(int i=0;i<5000; ++i){
            htime->clear();
            if(i%1000==0) cout<<"Proccessing "<<i<<endl;
            chain->GetEntry(i);
            for(int j=0;j<htime->size();++j){
                ht1D[f]->Fill(htime->at(j));
            }
        }
        delete htime;
        chain->~TChain();
    }
    TCanvas *c = new TCanvas("c","c",1920,1080);
    c->cd();
    ht1D[0]->Draw();
    ht1D[1]->SetLineColor(4);
    ht1D[2]->SetLineColor(2);
    ht1D[1]->Draw("SAME");
    ht1D[2]->DrawCopy("SAME");

    TCanvas *c1 = new TCanvas("c1","c1",1920,1080);
    c1->cd();
    ht1D[2]->Add(ht1D[1],-1);
    ht1D[2]->Draw("HIST");
}