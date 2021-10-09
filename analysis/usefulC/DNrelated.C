// #include "/junofs/users/jiangw/include/myincludes.h"
void DNrelated(){
    gROOT->ProcessLine("#include <vector>");
    const int n_pmt = 17612;

    int nPMT, nPulse;
    vector<int> *PMTID = new vector<int>;
    // tmp available: Co60 Cs137 Laser0.1 Laser0.05
    string sname[4] = {"Cs137","Co60","Laser0.1","Laser0.05"};
    TH1D *nPMT_hist[4];
    TF1 *fit_g[4];
    int lineColor[4] = {2,4,6,8};
    int pmt_dn[4][n_pmt];
    int dn_evt[4];

    double fit_result[4][3];
    int s;
    TH1D *all_nPMT_hist = new TH1D("all_nPMT_hist","all_nPMT_hist",200,450,1050);
    for(s=0; s<4; ++s){ 
        memset(pmt_dn[s], 0, n_pmt*sizeof(int));
        dn_evt[s] = 0;
        TChain *t = new TChain("CDLPMTSIMEVT");
        t->Add(Form("root://junoeos01.ihep.ac.cn//eos/juno/user/jiangw/J21v1r0-Pre0/ForceTrigger/%s/elecsim/user-root/user-elecsim-*.root",sname[s].c_str()));
        t->SetBranchStatus("*",0);
        t->SetBranchStatus("nPMT",1);
        // t->SetBranchStatus("nPulse",1);
        t->SetBranchAddress("nPMT",&nPMT);
        // t->SetBranchAddress("PMTID",&PMTID);
        // t->SetBranchAddress("nPulse",&nPulse);
        string name = sname[s] + "_nPMT_hist";
        nPMT_hist[s] = new TH1D(name.c_str(),name.c_str(),200,450,1050);
        nPMT_hist[s]->SetLineColor(lineColor[s]);
        // TH1D *nPulse_hist = new TH1D("nPulse_hist","nPulse_hist",200,450,1050);

        cout << endl << sname[s] << " Total event: " << t->GetEntries() << endl;
        int i;
        for(i=0; i<t->GetEntries(); ++i){
            // PMTID->clear();
            t->GetEntry(i);
            nPMT_hist[s]->Fill(nPMT);
            all_nPMT_hist->Fill(nPMT);
            // nPulse_hist->Fill(nPulse);
        }

        name = sname[s] + "_gaus";
        fit_g[s] = new TF1(name.c_str(),"gaus");
        fit_g[s]->SetParameters(nPMT_hist[s]->GetBinContent(nPMT_hist[s]->GetMaximumBin()),nPMT_hist[s]->GetMean(),nPMT_hist[s]->GetRMS());
        nPMT_hist[s]->Fit(name.c_str());
        fit_g[s]->GetParameters(&fit_result[s][0]);
        cout << name << " results, mean " << fit_result[s][1] << " sigma: " << fit_result[s][2] << endl;

        t->SetBranchStatus("PMTID",1);
        t->SetBranchAddress("PMTID",&PMTID);
        for(i=0;i<t->GetEntries(); ++i){
            PMTID->clear();
            t->GetEntry(i);
            // cout << "nPMT: " << nPMT << ", PMTID size: " << PMTID->size() << endl;
            if(nPMT>fit_result[s][1]-2.5*fit_result[s][2]&&nPMT<fit_result[s][1]+2.5*fit_result[s][2]){
                for(int j=0; j<PMTID->size(); ++j){
                    ++pmt_dn[s][PMTID->at(j)];
                }
                ++dn_evt[s];
            }
        }
        cout << sname[s] + " dn_evt: " << dn_evt[s] << endl;
		// for(i=0; i<10; ++i){
        //     cout << "pmtid: " << i << " counts: " << pmt_dn[s][i] << " DNR: " << pmt_dn[s][i]*1.0/(1250.e-9*dn_evt[s]) << endl;
        // }

        t->~TChain();
    }
    
    TCanvas *c = new TCanvas("c","c",1920,1080);
    // c->Divide(2,2);
    all_nPMT_hist->Draw();
    for(int s=0;s<4;++s){
        // nPMT_hist[s]->Scale(1./nPMT_hist[s]->Integral());
        nPMT_hist[s]->Draw("SAME HIST");
    }
    // c->cd(1);
    // nPMT_hist[i]->Draw("SAME");
    // c->cd(2);
    // nPulse_hist->Draw();
    double pmt_DN[n_pmt];
    for(int i=0; i<n_pmt; ++i){
        pmt_DN[i] = 0.25*(double(pmt_dn[0][i])/double(dn_evt[0])+double(pmt_dn[1][i])/double(dn_evt[1])+double(pmt_dn[2][i])/double(dn_evt[2])+double(pmt_dn[3][i])/double(dn_evt[3]))/1250.e-9;
        // pmt_DN[i] = (pmt_dn[0][i]+pmt_dn[1][i]+pmt_dn[2][i]+pmt_dn[3][i])*1.0/(dn_evt[0]+dn_evt[1]+dn_evt[2]+dn_evt[3])/1250.e-9;
        if(false){
            // vector<double> dn;
            // vector<double> dn_err;
            // for(int j=0;j<4;++j){
            //     dn.push_back()
            // }
        }
        if(pmt_DN[i]>100e3) cout << "High DN? pmtid: " << i << ", pmtdn: " << pmt_dn[0][i] << " " << pmt_dn[1][i] << " " << pmt_dn[2][i] << " " << pmt_dn[3][i] << endl;
    }
    ofstream ojw("DNR.txt");
    for(int i=0; i<n_pmt; ++i) ojw << pmt_DN[i] << endl;
    ojw.close();
}
