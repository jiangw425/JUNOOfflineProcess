void compare_C14_detsim_hitTime(){
    const int num_file = 3;

    std::string path[num_file] = {
        "00/user-detsim-0.root",
        "11/user-detsim-0.root",
        "22/user-detsim-0.root",
    };
    std::string lable[num_file] = {"00","11","J21v2"};

    std::vector<double> *hitTime = new vector<double>;
    int color[num_file] = {1,4,2};
    TFile *rfile[num_file];
    TH1D *ht[num_file];
    for(int fn=0;fn<num_file;++fn){
        rfile[fn] = TFile::Open(path[fn].c_str());
        TTree *t = (TTree*)rfile[fn]->Get("evt");
        t->SetBranchStatus("*",0);
        t->SetBranchStatus("hitTime",1);
        t->SetBranchAddress("hitTime",&hitTime);
        TString tmpname = Form("%s_C14_detsim_hitTime",lable[fn].c_str());
        ht[fn] = new TH1D(tmpname,tmpname,1350,-50,1300);
        ht[fn]->SetLineColor(color[fn]);
        for(int i=0;i<t->GetEntries();++i){
            if(i%100==0) cout << "Processing " << i << endl;
            t->GetEntry(i);
            for(int j=0;j<hitTime->size();++j){
                ht[fn]->Fill(hitTime->at(j));
            }
        }
    }

    TCanvas *c[num_file];
    for(int fn=0;fn<num_file;fn++){
        c[fn] = new TCanvas(lable[fn].c_str(),lable[fn].c_str(),1600,900);
        c[fn]->cd();
        ht[fn]->Draw();
    }

    TCanvas *cc = new TCanvas("c","c",1600,900);
    cc->cd();
    for(int fn=0;fn<num_file;fn++) ht[fn]->Draw("SAME");
}