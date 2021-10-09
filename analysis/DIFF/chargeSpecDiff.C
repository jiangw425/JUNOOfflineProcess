void chargeSpecDiff(){
    const int num_PMT = 17612;

    std::string ver[2] = {"pre","new"};
	std::string filename[2] = {
		// "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/data/Calibration/PMTCalibSvc/data/output_deconv.root",//old
        // "/junofs/users/jiangw/J21_calib/FFT/C14/step12/user-root/user_calibCorr.root",
        // "/junofs/users/jiangw/J21_calib/FFT/C14/step12/user-root/600-1000.root",
        //"/junofs/users/jiangw/J21_calib/FFT/C14/step12/user-root/725-975.root",
        //"/junofs/users/jiangw/give2lxj/J21v1r0-Pre0/output_deconv.root"

        // "/junofs/users/jiangw/JUNOofflineProcess/FFT_v2/Laser0.1_output.root",
        // "/scratchfs/juno/jiangw/J21_calib/FFT/Laser0.1/step12/user-root/user_calibCorr.root"
		"/junofs/users/liujiaxi16/calib/C14/step12/user-root/user_calibCorr.root",
		"/scratchfs/juno/jiangw/Pre2/C14/step12/user-root/user_calibCorr.root"
    };
    int color[2] = {4,2};
    TH1F *h[2];
    TFile *f[2];
    f[0]=TFile::Open(filename[0].c_str());
    f[1]=TFile::Open(filename[1].c_str());
    TCanvas *c = new TCanvas("c", "c", 1920, 1080);
    c->Print("SPE_charge_spec.pdf[");
    for(int i=0; i<20; ++i){
        for(int j=0;j<2;++j){
            h[j] = (TH1F*)f[j]->Get(Form("ch%d_charge_spec",i));
            // cout << h[j]->GetMean() << "\t";
            h[j]->SetLineColor(color[j]);
            // h[j]->Scale(1./h[j]->Integral());
            // h[j]->GetXaxis()->SetRangeUser(0,5);
        }
        // cout << endl;
        c->cd()->SetLogy();
        h[1]->Draw("HIST");
        h[0]->Draw("HIST SAME");
        c->Print("SPE_charge_spec.pdf");
    }
    c->Print("SPE_charge_spec.pdf]");
}
