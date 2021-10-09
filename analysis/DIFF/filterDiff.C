void filterDiff()
{
    TFile *nf = TFile::Open("/scratchfs/juno/jiangw/Pre2/Ge68/step02/user-root/filter.root");
    TH1D *n_h = (TH1D*)nf->Get("fh0");
    TH1D *n_n = (TH1D*)nf->Get("fn0");

    //TFile *of = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre0/data/Reconstruction/Deconvolution/share/filter3_m.root");
    TFile *of = TFile::Open("/junofs/users/liujiaxi16/calib/Ge68/step02/user-root/filter.root");
    TH1D *o_h = (TH1D*)of->Get("fh0");
    TH1D *o_n = (TH1D*)of->Get("fn0");

    o_n->SetLineColor(kBlue);
    o_h->SetLineColor(kBlue);
    n_n->SetLineColor(kRed);
    n_h->SetLineColor(kRed);
    n_n->GetXaxis()->SetRangeUser(0,250);
    n_h->GetXaxis()->SetRangeUser(0,250);
    n_n->GetYaxis()->SetRangeUser(-0.1,1);
    n_h->GetYaxis()->SetRangeUser(-0.1,1);
    TCanvas *c = new TCanvas("c","c",1600,900);
    c->Divide(2,1);
    c->cd(1);
    n_n->Draw();
	o_n->Draw("SAME");
    c->cd(2);
    n_h->Draw();
    o_h->Draw("SAME");
}
