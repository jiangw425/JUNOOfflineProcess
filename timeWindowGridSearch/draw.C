void draw(){
    const int num_time = 16;
    const int len_step = 20;
    const int start_time=200;
    const int num_energy= 6;

    int energies[num_energy] = {0,2,3,4,5,8};
    int color[num_energy] = {1,3,4,2,5,6};

    double energyscales[num_time];
    double time_windows[num_time];
    double tmps[num_time] = {0};
    double rec[2][num_energy][num_time];
    double res[2][num_energy][num_time];
    double reserr[num_energy][num_time];
    double reserr_relative[num_energy][num_time];
    double jw[2][num_energy][num_time];
    for(int t=0;t<num_time;t++){
        int time_window = len_step*t + start_time;
        time_windows[t] = time_window;
        
        std::ifstream ijw(std::to_string(time_window)+"/Ge68_energyscale.txt");
        ijw >> energyscales[t];
        cout << "Time Window Length: " << time_window << ", Ge68 energy scale: " << energyscales[t] << endl;
        ijw.close();
        ijw.open(std::to_string(time_window)+"/e+/kRes.txt");
        for(int e=0;e<num_energy;e++){
            ijw >> rec[0][e][t] >> rec[1][e][t] >> res[0][e][t] >> res[1][e][t];
            std::cout << " " << rec[0][e][t] << " " << rec[1][e][t] << " " << res[0][e][t] << " " << res[1][e][t] << std::endl;
            rec[0][e][t] *= energyscales[t];
            rec[1][e][t] *= energyscales[t];
        }
        ijw.close();
    }
    const int rela_n = 15;
    for(int t=0;t<num_time;t++) for(int e=0;e<num_energy;e++){
        jw[0][e][t] = 100. * rec[0][e][t] / rec[0][e][rela_n] - 100.;
        jw[1][e][t] = 100. * res[0][e][t] / res[0][e][rela_n] - 100.;
        // jw[0][e][t] = rec[0][e][t];
        // jw[1][e][t] = res[0][e][t];
        // reserr[e][t] = jw[1][e][t] * TMath::Sqrt(res[1][e][t]*res[1][e][t]/res[0][e][t]/res[0][e][t] + res[1][rela_n][t]*res[1][rela_n][t]/res[0][rela_n][t]/res[0][rela_n][t]);
    }
    // for(int t=0;t<num_time;t++) for(int e=0;e<num_energy;e++){
    //     reserr_relative[e][t] = TMath::Sqrt(reserr[e][t]*reserr[e][t]/res[0][e][t]/res[0][e][t] + reserr[num_time-1][t]*reserr[num_time-1][t]/res[0][num_time-1][t]/res[0][num_time-1][t]);
    // }
    // TH1D *h[2][num_energy];
    // for(int e=0;e<num_energy;e++){
    //     TString name = Form("rec1D_%dMeV",energies[e]);
    //     h[0][e] = new TH1D(name,name,num_time,time_windows[0],time_windows[num_time-1]);
    //     name = Form("res1D_%dMeV",energies[e]);
    //     h[1][e] = new TH1D(name,name,num_time,time_windows[0],time_windows[num_time-1]);
    //     for(int t=0;t<num_time;++t){
    //         h[0][e]->SetBinContent(t,rec[0][e][t]);
    //         h[1][e]->SetBinContent(t,res[0][e][t]);
    //     }
    //     h[0][e]->SetLineColor(color[e]);
    //     h[1][e]->SetLineColor(color[e]);
    // }

    TGraphErrors *tgraph[2][num_energy];
    TMultiGraph *mg[2];
    mg[0] = new TMultiGraph("rec","rec");
    mg[1] = new TMultiGraph("res","res");
    mg[0]->GetXaxis()->SetTitle("Signal window length[ns]");
    mg[1]->GetXaxis()->SetTitle("Signal window length[ns]");
    mg[0]->GetYaxis()->SetTitle("E_{rec} variation[%]");
    mg[1]->GetYaxis()->SetTitle("Res. variation[%]");

    for(int e=0;e<num_energy;e++){
        tgraph[0][e] = new TGraphErrors(num_time, time_windows, jw[0][e], tmps, tmps);
        // tgraph[1][e] = new TGraphErrors(num_time, time_windows, jw[1][e], tmps, reserr[e]);
        tgraph[1][e] = new TGraphErrors(num_time, time_windows, jw[1][e], tmps, tmps);
        tgraph[0][e]->SetLineColor(color[e]);
        tgraph[1][e]->SetLineColor(color[e]);
        tgraph[0][e]->SetLineWidth(2);
        tgraph[1][e]->SetLineWidth(2);
        tgraph[0][e]->SetName(Form("Ek=%dMeV",energies[e]));
        tgraph[1][e]->SetName(Form("Ek=%dMeV",energies[e]));
        tgraph[0][e]->SetTitle(Form("Ek=%dMeV",energies[e]));
        tgraph[1][e]->SetTitle(Form("Ek=%dMeV",energies[e]));
        // tgraph[0][e]->GetXaxis()->SetTitle("Signal window length[ns]");
        // tgraph[1][e]->GetXaxis()->SetTitle("Signal window length[ns]");
        // tgraph[0][e]->GetYaxis()->SetTitle("E_{rec} variation[%]");
        // tgraph[1][e]->GetYaxis()->SetTitle("Res. variation[%]");
    }
    // TGraph *g00 = new TGraph(num_time, time_windows, res[0][0]);
    // TGraph *g01 = new TGraph(num_time, time_windows, res[0][1]);
    // TGraph *g02 = new TGraph(num_time, time_windows, res[0][2]);
    // TGraph *g03 = new TGraph(num_time, time_windows, res[0][3]);

    TCanvas *c = new TCanvas("c","c",1600,900);
    c->Divide(2);
    for(int i=0;i<2;i++){
        c->cd(i+1);
        for(int e=0;e<num_energy;e++){
            mg[i]->Add(tgraph[i][e]);
        }
        mg[i]->Draw("AC*");
        c->cd(i+1)->BuildLegend();
        // for(int e=0;e<num_energy;e++){
        //     h[i][e]->Draw("SAME");
        // }
        // tgraph[i][0]->Draw("AC*");
        // tgraph[i][1]->Draw("C* SAME");
        // tgraph[i][2]->Draw("C* SAME");
        // tgraph[i][3]->Draw("C* SAME");
    }

    TCanvas *c0= new TCanvas("c0","c0",1600,900);
    c0->cd();
    TGraph *es = new TGraph(num_time,time_windows,energyscales);
    es->GetXaxis()->SetTitle("Signal window length[ns]");
    es->GetYaxis()->SetTitle("Ge68 energy scale");
    es->SetTitle("1.022/(#mu_{Erec} of Ge68 at center)");
    es->Draw("AC*");

    // TH1D *rec1D = new TH1D("rec1D","rec1D",34,180,520);
    // for(int t=0;t<num_time;t++){
    //     int time_window = len_step*t + start_time;
    //     for(int e=0;e<num_energy;e++){
    //         rec1D->SetBinContent(2*t, rec[0][0][t]);
    //     }
    // }

    // rec1D->Draw();
}