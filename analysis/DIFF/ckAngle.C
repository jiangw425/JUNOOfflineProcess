void ckAngle()
{
	TH1::AddDirectory(kFALSE);
    TFile *ojw = TFile::Open("10101.8_0_5832.29.root","recreate");
    const int n = 20000; 
    int pmtID[n];
    float local_z[n], local_x[n], local_y[n];
    //std::vector<float> *x = new std::vector<float>;
    //std::vector<float> *y = new std::vector<float>;
    //std::vector<float> *z = new std::vector<float>;
    int evtID;
    float x, y, z;
    // std::vector<int> *pmtID = new std::vector<int>;
    // std::vector<float> *local_z = new std::vector<float>;
    // TFile *f = TFile::Open("/junofs/production/data-production/Pre-Releases/J20v2r0-Pre0/e+/0-10MeV/0MeV/detsim/user-root/user-detsim-123.root");
    // TTree *c = (TTree*)f->Get("evt");
    TChain *c = new TChain("evt");
    
    std::string path = "root://junoeos01.ihep.ac.cn//eos/juno/user/jiangw/ckAngle/10101.8_0_5832.29/user-root/user-detsim-";
    //std::string path = "/junofs/production/data-production/Pre-Releases/J20v2r0-Pre0/ACU+CLS/Laser/photon_11522/Laser_0_0_0/detsim/user-root/user-detsim-";
    //std::string path = "/junofs/production/data-production/Pre-Releases/J20v2r0-Pre0/e+/0-10MeV/10MeV/detsim/user-root/user-detsim-";
    int begin = 0;
    int num = 121;
    for(int i=begin; i<begin+num; ++i)  c->Add(Form("%s%d.root",path.c_str(),i));
    c->SetBranchAddress("pmtID",&pmtID[0]);
    c->SetBranchAddress("LocalPosX",&local_x[0]);
    c->SetBranchAddress("LocalPosY",&local_y[0]);
    c->SetBranchAddress("LocalPosZ",&local_z[0]);

    TH1D *z1D = new TH1D("z1D","pmt local z",200,0,200);
    TTree *t = new TTree("local","pmt local hit pos");
    t->Branch("evtID",&evtID,"evtID/I");
    t->Branch("local_xx",&x,"local_x/F");
    t->Branch("local_yy",&y,"local_y/F");
    t->Branch("local_zz",&z,"local_z/F");

    cout<<"Total entries: "<<c->GetEntries()<<endl;
    for(int i=0; i<c->GetEntries();++i)
    {
	//x->clear();
	//y->clear();
	//z->clear();
        if(i%500==0) cout<<"Processing "<<i<<endl;
        memset(pmtID,0,sizeof(pmtID));
        memset(local_x,0,sizeof(local_x));
	memset(local_y,0,sizeof(local_y));
	memset(local_z,0,sizeof(local_z));
        //pmtID->clear();
        //local_z->clear();
        c->GetEntry(i);
	evtID=i;
        for(int j=0; j<n;++j)
        {
            if(pmtID[j]>20000) continue;
            if(local_z[j]==0) continue;
            if(pmtID[j]==8705)
	    {
            	z1D->Fill(local_z[j]);
	    	x=local_x[j];//->push_back(local_x[j]);
	    	y=local_y[j];//->push_back(local_y[j]);
	    	z=local_z[j];//->push_back(local_z[j]);
		t->Fill();
            }
	}
	//t->Fill();
    }
    z1D->Draw();
    //TFile *ojw = TFile::Open("1251.root","recreate");
    ojw->cd();
    t->Write();
    ojw->Close();
}
