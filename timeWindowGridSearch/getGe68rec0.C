void getGe68rec0(){
    TFile *f = TFile::Open("Ge68/Assemb.root");
    TH1D *rec1d = (TH1D*)f->Get("hSignal_QTEn");
    rec1d->GetXaxis()->SetRangeUser(0.7,1.3);
    rec1d->Draw();
    rec1d->Fit("gaus","","",0.8,1.2);
    // f->Close();

    ofstream ojw("Ge68_energyscale.txt");
    TF1* func_temp = (TF1*)rec1d->GetFunction("gaus");
    double rec = func_temp->GetParameter(1);
    double recerr = func_temp->GetParError(1);
    double res = 100*func_temp->GetParameter(2)/rec;
    double reserr = 100*func_temp->GetParError(2)/rec;
    double energyscale = 1.022/rec;

    ojw << energyscale << endl;
    ojw << rec*energyscale << '\t' << recerr*energyscale << '\t' << res << '\t' << reserr << endl;
    ojw.close();
}