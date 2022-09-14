void getAmC_ES(){
    TFile *f = TFile::Open("AmC/Assemb.root");
    TH1D *rec1d = (TH1D*)f->Get("hSignal_QTEn");
    rec1d->Draw();
    rec1d->Fit("gaus","","",2.2,2.7);
	TF1* func_temp = (TF1*)rec1d->GetFunction("gaus");
	double mean = func_temp->GetParameter(1);
	double energyscale = 2.223/mean;
    // f->Close();
	ofstream ojw("AmC_ES.txt");
	ojw << energyscale << endl;
	ojw << mean << '\t' << func_temp->GetParError(1) << '\t' << func_temp->GetParameter(2)/mean*100. << '\t' << func_temp->GetParError(2)/mean*100.<< endl;
	ojw.close();
}
