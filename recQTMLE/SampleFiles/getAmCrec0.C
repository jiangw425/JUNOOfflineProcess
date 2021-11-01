void getAmCrec0(){
    TFile *f = TFile::Open("AssembFiles/AmC/Assemb_QTMLE_0.root");
    TH1D *rec1d = (TH1D*)f->Get("hSignal_QTEn");
    rec1d->Draw();
    rec1d->Fit("gaus","","",2.2,2.7);
    // f->Close();
}