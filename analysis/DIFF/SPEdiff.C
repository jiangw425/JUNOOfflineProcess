void SPEdiff(){
    //This cpp is going to print PMT SPE spectra pmt by pmt to a pdf file.
    const int num_PMT = 17612;
    const int m_length= 1250;

    std::string ver[2] = {"pre","new"};
    std::string filename[2] = {
        "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2/data/Reconstruction/Deconvolution/share/SPE_v20.root",
        // "/junofs/users/liujiaxi16/calib/Ge68/step02/user-root/SPE.root"
        "/scratchfs/juno/jiangw/Pre2/Ge68/step02/user-root/SPE.root"
    };
    int color[2] = {4,2};
    TH1D *h[2];
    TFile *f[2];
    f[0]=TFile::Open(filename[0].c_str());
    f[1]=TFile::Open(filename[1].c_str());
    TCanvas *c = new TCanvas("c", "c", 1920, 1080);
    c->Divide(2,1);
    c->Print("SPE_spec.pdf[");
    for(int i=0; i<20; ++i){
        for(int j=0;j<2;++j){
            h[j] = (TH1D*)f[j]->Get(Form("PMTID_%d_MeanSpec",i));
            // cout << h[j]->GetMean() << "\t";
            h[j]->SetLineColor(color[j]);
            // h[j]->Scale(1./h[j]->Integral());
            // h[j]->GetXaxis()->SetRangeUser(0,5);
        }
        // TH1D *diff = new TH1D("PMTID_%d_MeanSpec_diff","PMTID_%d_MeanSpec",m_length,0,m_length);
        // for(int j=0;j<m_length;++j){
        //     diff->SetBinContent(j+1,h[1]->GetBinContent(j+1)-h[0]->GetBinContent(j+1));
        // }
        // cout << endl;
        // c->cd()->SetLogy();
        c->cd(1);
        h[1]->DrawCopy("HIST");
        h[0]->Draw("HIST SAME");
        c->cd(2);
        h[1]->Add(h[0],-1);
        h[1]->Draw("HIST");
        c->Print("SPE_spec.pdf");
    }
    c->Print("SPE_spec.pdf]");
}