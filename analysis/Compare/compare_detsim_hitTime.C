void compare_detsim_hitTime(){
    const int num_file = 2;
    const std::string sname = "SNAME";
    std::string lable[num_file] = {"old","new"};
    std::string path[num_file] = {
        "OLD",
        "NEW",
    };

    const int length = 1250;
    std::vector<double> *v_hitTime = new vector<double>;
    int color[num_file] = {1,2};
    TH1D *hitTime[num_file];

    for(int fn=0;fn<num_file;++fn){
        v_hitTime->clear();
        TChain *t = new TChain("evt");
        TString spath = Form("%s/*.root",path[fn].c_str());
        t->Add(spath);
        t->SetBranchStatus("*",0);
        t->SetBranchStatus("hitTime",1);
        t->SetBranchAddress("hitTime",&v_hitTime);
        int totalEntries = t->GetEntries();
        cout << path[fn] << " total entries: " << totalEntries << endl;

        TString tmpname = Form("%s_%s_detsim_hitTime",lable[fn].c_str(),sname.c_str());
        hitTime[fn] = new TH1D(tmpname,tmpname,1350,-50,1300);
        hitTime[fn]->SetLineColor(color[fn]);

        int tmpentries = 2000 < totalEntries ? 2000 : totalEntries;
        for(int i=0;i<tmpentries;++i){
            if(i%100==0) cout << "Processing " << i << "/" << tmpentries << endl;
            t->GetEntry(i);
            for(int j=0;j<v_hitTime->size();++j){
                hitTime[fn]->Fill(v_hitTime->at(j));
            }
        }
        t->~TChain();
    }
    delete v_hitTime;

    TCanvas *c[num_file];
    for(int fn=0;fn<num_file;fn++){
        c[fn] = new TCanvas(lable[fn].c_str(),lable[fn].c_str(),1600,900);
        c[fn]->cd();
        hitTime[fn]->Draw();
    }

    TCanvas *cc = new TCanvas("c","c",1600,900);
    cc->cd();
    for(int fn=0;fn<num_file;fn++) hitTime[fn]->Draw("SAME");
    cc->Print(Form("%s_detsim_hitTime_diff.png",sname.c_str()));
}