void calibHitTime(){
    const int nf = 2;
    string path[nf] = {
        "/scratchfs/juno/jiangw/J22-rc0-calib/C14/step11/user-root/user-calib-0.root",
        "/scratchfs/juno/jiangw/J22-rc1-calib/C14/step11/user-root/user-calib-0.root"
    };
    const double timeShift[nf] = {100,220};
    string flags[nf] = {"C14-rc0","C14-rc1"};
    vector<float> *Time = new vector<float>;
    TH1D *t1d[nf];

    for(int f=0;f<nf;f++){
        TString name = Form("%s_calibHitTime",flags[f].c_str());
        t1d[f] = new TH1D(name,name,1700,-500,1200);
        TFile *rf = TFile::Open(path[f].c_str());
        TTree *tr = (TTree*)rf->Get("calibevt");
        tr->SetBranchStatus("*",0);
        tr->SetBranchStatus("Time",1);
        tr->SetBranchAddress("Time",&Time);
        for(int i=0;i<tr->GetEntries();i++){
            Time->clear();
            tr->GetEntry(i);
            for(int j=0;j<Time->size();j++){
                t1d[f]->Fill(Time->at(j)-timeShift[f]);
            }
        }
        t1d[f]->SetLineColor(f+1);
        rf->Close();
    }

    TCanvas *c = new TCanvas("c","c",1024,768);
    c->cd();
    for(int f=0;f<nf;f++){
        t1d[f]->Draw("SAME");
    }
    c->BuildLegend();
}
