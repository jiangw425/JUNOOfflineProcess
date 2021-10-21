void check_eplus_rec_det(){
    const int filenum = 100;
    std::string path="/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/11/e+/e+_Uniform";
    std::string treename[3]={"evt","eventindex","TRec"};
    std::string sim_type[3]={"detsim","elecsim","recQTMLE"};
    for(int e=0;e<9;e++) for(int i=0;i<filenum;i++){
        long long entries[3];
        for(int t=0;t<3;t++){
            TFile *tmpf = TFile::Open(Form("%s/%dMeV/%s/user-root/user-%s-%d.root",path.c_str(),e,sim_type[t].c_str(),sim_type[t].c_str(),i));
            // if(!tmpf){
            //     cout << Form("%dMeV_%dfile_%s",e,i,sim_type[t].c_str()) << endl;
            //     return;
            // }
            entries[t]=((TTree*)tmpf->Get(treename[t].c_str()))->GetEntries();
            tmpf->Close();
            cout << entries[t] << "\t";
        }
        if(entries[0]!=entries[1]||entries[0]!=entries[2])  cout << "Attention: " <<Form("%dMeV_%dfile",e,i);
        cout << endl;
    }
}