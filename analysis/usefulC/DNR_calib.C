void DNR_calib(){
    gROOT->ProcessLine("#include <vector>");
    const int n_pmt = 17612;
    const int n_source = 4;

    int nPMT, PMTID;
    // tmp available: Co60 Cs137 Laser0.1 Laser0.05
    string sname[n_source] = {"Cs137","Co60","Laser0.1","Laser0.05"};
    TH1D *nPMT_hist[n_source];
    TF1 *fit_g[n_source];
    int lineColor[n_source] = {2,4,6,8};
    int pmt_dn[n_source][n_pmt];
    int dn_evt[n_source];

    double fit_result[n_source][3];
    int s;
    for(s=0; s<n_source; ++s){
        memset(pmt_dn[s], 0, sizeof(int));
        dn_evt[s] = 0;
        TChain *tElec = new TChain("Event/Elec/ElecEvent");
        tElec->Add(Form("root://junoeos01.ihep.ac.cn//eos/juno/user/jiangw/J21v1r0-Pre0/ForceTrigger/%s/elecsim/root/elecsim-*.root",sname[s].c_str()));
        JM::ElecEvent *ee = new JM::ElecEvent();
        tElec->SetBranchAddress("ElecEvent",&ee);

        string name = sname[s] + "_nPMT_hist";
        nPMT_hist[s] = new TH1D(name.c_str(),name.c_str(),200,450,1050);
        nPMT_hist[s]->SetLineColor(lineColor[s]);
        cout << endl << sname[s] << " Total event: " << tElec->GetEntries() << endl;
        
        vector<int> nFiredPMT;
        int i;
        for(i=0; i<tElec->GetEntries(); ++i){
            if(i%100==0) cout << "Processing " << i << endl;
            tElec->GetEntry(i);
            const JM::ElecFeeCrate &efc = ee->elecFeeCrate();
            JM::ElecFeeCrate *m_crate = const_cast<JM::ElecFeeCrate*>(&efc);
            std::map<int, JM::ElecFeeChannel> *feeChannels = &m_crate->channelData();
            std::map<int, JM::ElecFeeChannel>::iterator it;
            //only process fired pmt
            nPMT = 0;
            for(it = feeChannels->begin(); it!=feeChannels->end(); ++it){
                JM::ElecFeeChannel &channel = (it->second);
                if(0==channel.adc().size()) continue;
                if(n_pmt<=it->first) continue;
                ++nPMT;
            }
            nFiredPMT.push_back(nPMT);
            nPMT_hist[s]->Fill(nPMT);
            // nPulse_hist->Fill(nPulse);
        }

        name = sname[s] + "_gaus";
        fit_g[s] = new TF1(name.c_str(),"gaus");
        fit_g[s]->SetParameters(nPMT_hist[s]->GetBinContent(nPMT_hist[s]->GetMaximumBin()),nPMT_hist[s]->GetMean(),nPMT_hist[s]->GetRMS());
        nPMT_hist[s]->Fit(name.c_str());
        fit_g[s]->GetParameters(&fit_result[s][0]);
        cout << name << " results, mean " << fit_result[s][1] << " sigma: " << fit_result[s][2] << endl;

        for(i=0;i<tElec->GetEntries(); ++i){
            if(i%100==0) cout << "Processing " << i << endl;
            tElec->GetEntry(i);
            const JM::ElecFeeCrate &efc = ee->elecFeeCrate();
            JM::ElecFeeCrate *m_crate = const_cast<JM::ElecFeeCrate*>(&efc);
            std::map<int, JM::ElecFeeChannel> *feeChannels = &m_crate->channelData();
            std::map<int, JM::ElecFeeChannel>::iterator it;
            if(nFiredPMT.at(i)>fit_result[s][1]-2.5*fit_result[s][2]&&nFiredPMT.at(i)<fit_result[s][1]+2.5*fit_result[s][2]){
                for(it = feeChannels->begin(); it!=feeChannels->end(); ++it){
                    JM::ElecFeeChannel &channel = (it->second);
                    if(0==channel.adc().size()) continue;
                    if(n_pmt<=it->first) continue;
                    ++pmt_dn[s][it->first];
                }
                ++dn_evt[s];
            }
        }
		cout << sname[s] + " dn_evt: " << dn_evt[s] << endl;
        for(i=0; i<10; ++i){
            cout << "pmtid: " << i << " counts: " << pmt_dn[s][i] << " DNR: " << pmt_dn[s][i]*1.0/(1250.e-9*dn_evt[s]) << endl;
        }

        nFiredPMT.clear();
        tElec->~TChain();
    }
    
    TCanvas *c = new TCanvas("c","c",1920,1080);
    // c->Divide(2,2);
    for(int s=0;s<n_source;++s){
        nPMT_hist[s]->Scale(1./nPMT_hist[s]->Integral());
        nPMT_hist[s]->Draw("SAME HIST");
    }
    // c->cd(1);
    // nPMT_hist[i]->Draw("SAME");
    // c->cd(2);
    // nPulse_hist->Draw();
}
