// #include "/junofs/users/jiangw/include/myincludes.h"
void ck_nPE_r()
{
	TH1::AddDirectory(kFALSE);
    const int num_pmt = 17612;
    // bool* hmmtpmt = pmtflag();
    int CalibPosNum = 35;
    std::string path[2] = {
        "/scratchfs/juno/jiangw/J21_angle/Production/00",
        "/scratchfs/juno/jiangw/J21_angle/Production/11"
    };

    int positionZ[CalibPosNum];
    double positionR[CalibPosNum];
    double positionR3[CalibPosNum];

    for(int i=0;i<CalibPosNum;i++) {
        positionZ[i] = (i-17)*1000;
        positionR[i] = positionZ[i]/1e3;
        positionR3[i]=positionR[i]*positionR[i]*positionR[i];
    }

    int tmp_totPE;
    double totPE[3][CalibPosNum];
    for(int t=0;t<2;++t) for(int i=0;i<CalibPosNum;i++) {
        TString simFilePath = Form("%s/e+_0_0_%d/0MeV/detsim/user-root/user-detsim-0.root", path[t].c_str(), positionZ[i]);
        TChain *tc = new TChain("evt");
        tc->Add(simFilePath);
        tc->SetBranchStatus("*",0);
        tc->SetBranchStatus("totalPE",1);
        int sumPE = 0;
        tc->SetBranchAddress("totalPE",&tmp_totPE);
        for(int en=0;en<tc->GetEntries();++en){
            tc->GetEntry(en);
            sumPE+=tmp_totPE;
        }
        totPE[t][i] = sumPE/tc->GetEntries();
        cout << i <<"\t"<< totPE[t][i] << "\t" << tc->GetEntries() << endl;
        tc->~TChain();
    }
    for(int i=0;i<CalibPosNum;i++) totPE[2][i] = totPE[1][i] - totPE[0][i];

    TCanvas *c0 = new TCanvas("c0","c0",1600,900);
    TGraph *nPE_r0 = new TGraph(CalibPosNum,positionR,totPE[0]);
    TGraph *nPE_r1 = new TGraph(CalibPosNum,positionR,totPE[1]);
    TGraph *nPE_r2 = new TGraph(CalibPosNum,positionR,totPE[2]);
    // TH1D *nPE_r0= new TH1D("nPE_r0","nPE_r0",37,-18.5,18.5);
    // TH1D *nPE_r1= new TH1D("nPE_r1","nPE_r1",37,-18.5,18.5);
    // for(int i=0;i<CalibPosNum;++i){
    //     int bin = positionR[i] + 19;
    //     nPE_r0->SetBinContent(bin,totPE[0][i]);
    //     nPE_r1->SetBinContent(bin,totPE[1][i]);
    // }
    c0->cd();
    nPE_r1->SetLineColor(kRed);
    nPE_r1->GetYaxis()->SetRangeUser(1250,1600);
    nPE_r1->Draw("AC*");
    nPE_r0->Draw("SAME");

    TCanvas *c1 = new TCanvas("c1","c1",1600,900);
    // TGraph *nPE_r30 = new TGraph(CalibPosNum,positionR3,totPE[0]);
    // TGraph *nPE_r31 = new TGraph(CalibPosNum,positionR3,totPE[1]);
    // TH1D *nPE_r30= new TH1D("nPE_r30","nPE_r30",20,0,6000);
    // TH1D *nPE_r31= new TH1D("nPE_r31","nPE_r31",20,0,6000);
    // for(int i=0;i<CalibPosNum;++i){
    //     int bin = int(positionR3[i]/300);
    //     nPE_r30->SetBinContent(bin+1,totPE[0][i]);
    //     nPE_r31->SetBinContent(bin+1,totPE[1][i]);
    // }
    c1->cd();
    nPE_r2->GetYaxis()->SetRangeUser(0,120);
    nPE_r2->Draw("AC*");
    // nPE_r31->SetLineColor(kRed);
    // nPE_r31->Draw("AC*");
    // nPE_r30->Draw("SAME");
}
