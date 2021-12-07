// #include "/junofs/users/jiangw/include/myincludes.h"
void ck_nPE_eplus_E_3()
{
	TH1::AddDirectory(kFALSE);
    const int num_pmt    = 17612;
    const int num_energy = 10;
    const int num_file   = 3;
    std::string path[num_file] = {
        "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/00/e+/e+_Uniform",
        "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/11/e+/e+_Uniform",
		"/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/e+/e+_Uniform"
    };

    int tmp_totPE;
    float edep;
    double totPE[num_file+1][CalibPosNum];
    double totPEres[num_file][CalibPosNum];

    for(int t=0;t<num_file;++t) for(int e=0;e<num_energy;++e) for(int i=0;i<CalibPosNum;i++) {
        TString simFilePath = Form("%s/%dMeV/detsim/user-root/user-detsim-*", path[t].c_str(), e);
        TChain *tc = new TChain("evt");
        tc->Add(simFilePath);
        tc->SetBranchStatus("*",0);
        tc->SetBranchStatus("totalPE",1);
        tc->SetBranchStatus("edep",1);
        TH1D *nPE_pdf = new TH1D("npe","npe",400,800,2400);
        // int sumPE = 0;
        tc->SetBranchAddress("totalPE",&tmp_totPE);
        tc->SetBranchAddress("edep",&edep);
        for(int en=0;en<tc->GetEntries();++en){
            tc->GetEntry(en);
            if(edep<1.0215||edep>1.0225) continue;
            nPE_pdf->Fill(tmp_totPE);
            // sumPE+=tmp_totPE;
        }
        // totPE[t][i] = sumPE/tc->GetEntries();
        nPE_pdf->Fit("gaus","Q");
        TF1* func_temp = (TF1*)nPE_pdf->GetFunction("gaus");
        double mean = func_temp->GetParameter(1);
        totPE[t][i] = mean;
        totPEres[t][i] = 100*func_temp->GetParameter(2)/mean;
        cout << i <<"\tavg_nPE: "<< totPE[t][i] << "  \tmean: " << mean << "\tres: " << totPEres[t][i] << "\tentries: " << tc->GetEntries() <<endl;
        tc->~TChain();
    }
    for(int i=0;i<CalibPosNum;i++){
        totPE[num_file][i] = totPE[2][i] - totPE[1][i];
        cout << i << ":\t" << totPE[1][i] << "\tup:\t" << 100.*totPE[2][i]/totPE[1][i] << "\%" << endl;
    }

    TCanvas *c0 = new TCanvas("c0","c0",1600,900);
    TGraph *nPE_r0 = new TGraph(CalibPosNum,positionR3[1],totPE[0]);
    TGraph *nPE_r1 = new TGraph(CalibPosNum,positionR3[1],totPE[1]);
    TGraph *nPE_r2 = new TGraph(CalibPosNum,positionR3[1],totPE[2]);
    TGraph *nPE_r3 = new TGraph(CalibPosNum,positionR3[1],totPE[3]);
    // TH1D *nPE_r0= new TH1D("nPE_r0","nPE_r0",20,0,20);
    // TH1D *nPE_r1= new TH1D("nPE_r1","nPE_r1",20,0,20);
    // for(int i=0;i<CalibPosNum;++i){
    //     int bin = int(positionR[2][i]);
    //     nPE_r0->SetBinContent(bin+1,totPE[0][i]);
    //     nPE_r1->SetBinContent(bin+1,totPE[1][i]);
    // }
    c0->cd();
    nPE_r2->SetLineColor(kRed);
    nPE_r1->SetLineColor(kBlue);
    nPE_r2->GetYaxis()->SetRangeUser(1100,1800);
    nPE_r2->Draw("AC*");
    nPE_r1->Draw("SAME C*");
    nPE_r0->Draw("SAME C*");

    TCanvas *c1 = new TCanvas("c1","c1",1600,900);
    TGraph *nPE_r0res = new TGraph(CalibPosNum,positionR3[1],totPEres[0]);
    TGraph *nPE_r1res = new TGraph(CalibPosNum,positionR3[1],totPEres[1]);
    TGraph *nPE_r2res = new TGraph(CalibPosNum,positionR3[1],totPEres[2]);
    // TH1D *nPE_r30= new TH1D("nPE_r30","nPE_r30",20,0,6000);
    // TH1D *nPE_r31= new TH1D("nPE_r31","nPE_r31",20,0,6000);
    // for(int i=0;i<CalibPosNum;++i){
    //     int bin = int(positionR3[2][i]/300);
    //     nPE_r30->SetBinContent(bin+1,totPE[0][i]);
    //     nPE_r31->SetBinContent(bin+1,totPE[1][i]);
    // }
    c1->cd();
    nPE_r2res->SetLineColor(kRed);
    nPE_r1res->SetLineColor(kBlue);
    // nPE_r1res->GetYaxis()->SetRangeUser(1250,1600);
    nPE_r2res->Draw("AC*");
    nPE_r1res->Draw("SAME C*");
    nPE_r0res->Draw("SAME C*");

    TCanvas *c2 = new TCanvas("c2","c2",1600,900);
    c2->cd();
    nPE_r3->GetYaxis()->SetRangeUser(0,200);
    nPE_r3->Draw("AC*");
}
