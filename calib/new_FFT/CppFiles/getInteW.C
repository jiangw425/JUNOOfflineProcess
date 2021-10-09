void getInteW(){
	TFile *f = TFile::Open("res_tmp.root");
    assert(f);
    const int num_PMT = 17612;
    TH1D *inte[2];
    int inteW[2] = {0,0};
    inte[0] = (TH1D*)f->Get("myn");
    inte[1] = (TH1D*)f->Get("myh");
    for(int i=0; i<2; ++i)  inteW[i] = inte[i]->GetMinimumBin();
    f->Close();

    ofstream ojw("inteW.txt");//nnvt, hmmt
    ojw << inteW[0] << endl << inteW[1] << endl;
    ojw.close();

    // for(int i=0; i<2; ++i)
    // if(inteW[i]<7 || inteW[i]>9){
    //     cout << "Warning!!! Please check the inteW info in CheckStation, detail in Ge68/step02/user-root/res_tmp.root" << endl;
    //     // return false;
    // }

    // return true;
}