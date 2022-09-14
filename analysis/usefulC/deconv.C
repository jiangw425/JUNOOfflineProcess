void deconv(){
    int m_length = 1000;
    const int N = m_length;
    const double m_threshold = 0.25;
    const int m_widthThreshold = 0;
    const int inteW = 1;
    const double lowAC = 0.05;
    const int entries = 2;

    TFile *SPEtf = TFile::Open("Mean_RE_IM.root");
    TH1D *reh = (TH1D*)SPEtf->Get("PMTID_0_SPERE");
    TH1D *imh = (TH1D*)SPEtf->Get("PMTID_0_SPEIM");
    int nx = reh->GetNbinsX();
    cout << "nx : " << nx << endl;
    double re[nx], im[nx];
    for(int i=0;i<nx;++i){
        re[i] = reh->GetBinContent(i+1);
        im[i] = imh->GetBinContent(i+1);
    }
    reh->~TH1D();
    imh->~TH1D();

    TVirtualFFT* fft_forward = TVirtualFFT::FFT(1, &m_length, "R2C EX K");
	TVirtualFFT* fft_back = TVirtualFFT::FFT(1, &m_length, "C2R EX K");

    TFile *wf_f = TFile::Open("user_Wave.root");
    TFile *ojw = TFile::Open(Form("user_calib_%d.root",m_length),"recreate");
    TH1D *input_wf = 0;
    TH1D *input_bsl_wf = 0;
    TH1D *fft_wf = 0;
    TH1D *fft_bsl_wf = 0;
    TH1D *fft_acc_wf = 0;
    TH1D *hit_begin = 0;
    for(int n=0;n<entries;++n){
        cout << "Processing " << n << endl;
        vector<double> charge;
        vector<double> time;
        vector<int> width;
        vector<int> peak;

        TH1D *tmp = (TH1D*)wf_f->Get(Form("wf_%d",n));
        vector<double> adc;
        for(int j=0;j<m_length;++j){
            adc.push_back(tmp->GetBinContent(j+1));
        }

        input_wf = new TH1D(Form("%d_input_wf",n),Form("%d_input_wf",n),m_length,0,m_length);
        for(int i=0;i<m_length;++i) input_wf->SetBinContent(i+1,adc[i]);
        ojw->cd();
        input_wf->Write();
// cout << "Checkpoint 1" << endl;
        // raw baseline
        double baseline = 0;
        for(int j=0;j<50;++j) baseline += adc[j];
        baseline /= 50;

        double input_rawh[m_length];
        for(int j=0;j<m_length;++j) input_rawh[j] = adc[j] - baseline;
       
        // input_bsl_wf = new TH1D(Form("%d_input_bsl_wf",n),Form("%d_input_bsl_wf",n),m_length,0,m_length);
        // for(int i=0;i<m_length;++i) input_bsl_wf->SetBinContent(i+1,input_rawh[i]);
        // ojw->cd();
        // input_bsl_wf->Write();

        std::vector<double> re_full_vec(N);
        std::vector<double> im_full_vec(N);
        double *re_full = &re_full_vec.front();
        double *im_full = &im_full_vec.front();
        for (int i = 0; i < N; i++) {
            re_full[i] = 0;
            im_full[i] = 0;
        }

        // fft
        fft_forward->SetPoints(input_rawh);
        fft_forward->Transform();
        fft_forward->GetPointsComplex(re_full, im_full);
    
        int ui = (nx<(N/2)) ? nx : int(N/2);
        for(int i=0; i<ui; ++i){
            // if(i>=N) break;
            if(re[i]!=0 || im[i]!=0){
                double a = re_full[i], b = im_full[i], c = re[i], d = im[i];
                double f = c*c + d*d;
                re_full[i] = (a*c+b*d)/f;
                im_full[i] = (b*c-a*d)/f;
            }else{
                re_full[i] = 0;
                im_full[i] = 0;
            }
        }for(int i=ui;i<N;++i){
            re_full[i] = 0;
            im_full[i] = 0;
        }
        re_full[0] = 0;
        im_full[0] = 0;

        // deconv
        fft_back->SetPointsComplex(re_full, im_full);
        fft_back->Transform();
        TH1 *Back = 0;
        Back = TH1::TransformHisto(fft_back, Back, "Re");
        Back->Scale(1. / m_length);

        std::vector<double> AC(N);
        for(int i=0; i<m_length;++i) AC[i] = Back->GetBinContent((i+N-87)%N+1);


        fft_wf = new TH1D(Form("%d_fft_wf",n),Form("%d_fft_wf",n),m_length,0,m_length);
        for(int i=0;i<m_length;++i) fft_wf->SetBinContent(i+1,AC[i]);
        ojw->cd();
        fft_wf->Write();

        double max, min;
        Back->GetMinimumAndMaximum(min,max);
        for(int i=0;i<N;++i) Back->SetBinContent(i+1,0);

        // rough bsl
        double tmp_scale = 1e3;
        if((max-min)>1e2){
            max = 0;
            tmp_scale = 1e5/(max-min);
        }
        int imax = int(max * tmp_scale);
        int imin = int(min * tmp_scale);
        baseline = 0;
        const int tmpsize = imax - imin + 1;
        vector<int> tmpv(tmpsize);
        for(int i=0;i<N;++i){
            int tmpidx = int(AC[i]*tmp_scale);
            if(tmpidx>=imax) continue;
            if (tmpidx >= tmpsize || tmpidx < imin) cout << "Fatal error!" << endl;
            tmpv.at(tmpidx-imin)++;
        }
        baseline = max_element(tmpv.begin(), tmpv.end()) - tmpv.begin() + imin;
        baseline /= tmp_scale;
        for(int i=0;i<N;++i) AC[i] -= baseline;

        fft_bsl_wf = new TH1D(Form("%d_fft_bsl_wf",n),Form("%d_fft_bsl_wf",n),m_length,0,m_length);
        for(int i=0;i<m_length;++i) fft_bsl_wf->SetBinContent(i+1,AC[i]);
        ojw->cd();
        fft_bsl_wf->Write();

        // waveform reconstruction
        std::vector<int> oneHit;
        std::vector<std::vector<int>> hit;
        std::vector<std::vector<int>> singleHit;
        std::vector<int> singleHitPeak;
        std::vector<int> hitPeakCounter;
        std::vector<int> hitWidth;

        // rough hits
        oneHit.clear();
        hit.clear();
        int Pass = 0;
        for (int i = 0; i < N; i++) {
            if (Pass == 0 && AC[i] > m_threshold) {
                for (int j = i; j >= 0; j--) {
                    oneHit.insert(oneHit.begin(), j);
                    if (AC[j] <= lowAC) break;
                }
                Pass = 1;
                continue;
            }
            if (Pass == 1) {
                oneHit.push_back(i);
                if (AC[i] <= lowAC || i == N - 1) {
                    if (oneHit.size() > m_widthThreshold) hit.push_back(oneHit);  // 12 ns width for J19
                    Pass = 0;
                    oneHit.clear();
                }
            }
        }

        // accurate baseline
        int flag;
        int counter = 0;
        baseline = 0;
        for (int i = 0; i < N; i++) {
            flag = 1;
            for (int j = 0; j < hit.size(); j++) {
                if (i >= hit.at(j).front() - inteW && i <= hit.at(j).back() + inteW) {
                    flag = 0;
                    break;
                }
            }
            if (flag != 0) {
                baseline += AC[i];
                counter++;
            }
        }
        if (counter < 40) {
            cout << "No enough points for baseline calculation, baseline will set to be the default one." << std::endl;  // make sure to have enough points to calculate baseline.
        } else if (counter != 0) {
            baseline /= (counter + 0.);
            for (int i = 0; i < N; i++) AC[i] -= baseline;
        }

        fft_acc_wf = new TH1D(Form("%d_fft_acc_wf",n),Form("%d_fft_acc_wf",n),m_length,0,m_length);
        for(int i=0;i<m_length;++i) fft_acc_wf->SetBinContent(i+1,AC[i]);
        ojw->cd();
        fft_acc_wf->Write();

        // accurate hits
        hit.clear();
        oneHit.clear();  // only when "if (hit.size() > 990) break;" happens, it will be used.
        Pass = 0;
        for (int i = 0; i < N; i++) {
            if (Pass == 0 && AC[i] > m_threshold) {
                for (int j = i; j >= 0; j--) {
                    oneHit.insert(oneHit.begin(), j);
                    if (AC[j] <= lowAC) break;
                }
                // if(n==0&&i>300) cout << "find a overthreshold at i="<< i << endl;
                Pass = 1;
                continue;
            }
            if (Pass == 1) {
                oneHit.push_back(i);
                if (AC[i] <= lowAC || i == N - 1) {
                    // if(n==0&&i>330){
                    //     cout << "the hit ns contain: ";
                    //     for(int t=0;t<oneHit.size();++t) cout << oneHit.at(t) << " ";
                    //     cout << endl;
                    // }
                    if (oneHit.size() > m_widthThreshold) hit.push_back(oneHit);  // 12 ns width for J19
                    Pass = 0;
                    oneHit.clear();
                }
            }
        }

        // if(n==0){
        //     int tmptmp = 2;
        //     for(int i=0;i<hit.at(hit.size()-tmptmp).size();++i) cout << hit.at(hit.size()-tmptmp).at(i) << endl;
        // }

        // find singal hit
        singleHit.clear();
        singleHitPeak.clear();
        hitPeakCounter.clear();
        int Rise = 1;
        counter = 0;
        int HitCount = hit.size();
        int HitBTime[1000] = {0};
        int HitETime[1000] = {0};
        hitWidth.clear();
        for (int i = 0; i < hit.size(); i++) {
            Rise = 1;
            int sT = -1;
            int eT;
            hitPeakCounter.push_back(0);
            // record hit starting time and ending time
            HitBTime[i] = hit.at(i).front();
            HitETime[i] = hit.at(i).back();
            for (int j = 0; j < hit.at(i).size(); j++) {
                // find vally
                if (Rise == 0) {
                    if (AC[hit.at(i).at(j)] > AC[hit.at(i).at(j - 1)]) {
                        singleHit.push_back(oneHit);
                        oneHit.clear();
                        counter++;
                        Rise = 1;
                    }
                }
                oneHit.push_back(hit.at(i).at(j));
                // find peak
                if (Rise == 1 && j > 0) {
                    if (AC[hit.at(i).at(j)] < AC[hit.at(i).at(j - 1)]) {
                        singleHitPeak.push_back(hit.at(i).at(j - 1));
                        if (AC[hit.at(i).at(j - 1)] >= m_threshold) hitPeakCounter.at(i)++;
                        Rise = 0;
                    }
                }
                // find overthreshold starting and ending point and their width
                if (AC[hit.at(i).at(j)] >= m_threshold) {
                    eT = j;
                    if (sT == -1) sT = j;
                }
            }
            // finish imcompleted hits, most are half peak at beginning or last of waveform
            if (oneHit.size() != 0) {
                if (Rise == 1) {
                    singleHitPeak.push_back(hit.at(i).back());
                    if (AC[hit.at(i).back()] >= m_threshold) hitPeakCounter.at(i)++;
                }
                singleHit.push_back(oneHit);
                oneHit.clear();
                counter++;
            }
            hitWidth.push_back(eT - sT + 1);
        }
        // exclude fake peaks which are underthreshold
        for (int i = 0; i < singleHitPeak.size(); i++) {
            if (AC[singleHitPeak.at(i)] < m_threshold) {
                singleHitPeak.erase(singleHitPeak.begin() + i);
                i--;
                counter--;
            }
        }
        // peak number test, robust check
        if (counter != singleHitPeak.size()) {
            cout << "Error! Wrong hit peak counting!" << std::endl;
        }
        int tempN = 0;
        for (int i = 0; i < hitPeakCounter.size(); i++) tempN += hitPeakCounter.at(i);
        if (tempN != singleHitPeak.size() || hitPeakCounter.size() != hit.size()) {
            cout << "Error2! Wrong hit peak counting!" << std::endl;
        }
        // for (int i = 0; i < hitWidth.size(); i++) {
        //     if (hitWidth.at(i) > 100 && debug == 1) {
        //     for (int i = 0; i < N; i++) debugWave->SetBinContent(i + 1, AC2[i]);
        //     TString debugTitle = Form("Warning: Too long width, width = %lu", hitWidth.at(i));
        //     debugWave->SetTitle(debugTitle);
        //     m_debug->Fill();
        //     }
        // }

        if (HitCount == 0) continue;
        for (int i = 0; i < HitCount; i++) {
            time.push_back(HitBTime[i]);
            int inte_begin = 0, inte_end = 0;
            int singleHit = 0;
            if (HitCount == 1) {
            inte_begin = HitBTime[i] - inteW + 1;
            if (inte_begin < 0) inte_begin = 0;
            inte_end = HitETime[i] + inteW;
            if (inte_end > N) inte_end = N;
            singleHit = 1;
            } else {
            if (i == 0) {
                inte_begin = HitBTime[i] - inteW + 1;
                if (inte_begin < 0) inte_begin = 0;
                int interval = HitBTime[1] - HitETime[0];
                if (interval > 2 * inteW) {
                inte_end = HitETime[0] + inteW;
                singleHit = 1;
                } else
                inte_end = int(HitETime[0] + interval / 2.);
            } else if (i == HitCount - 1) {
                inte_end = HitETime[i] + inteW;
                if (inte_end > N) inte_end = N;
                int interval = HitBTime[i] - HitETime[i - 1];
                if (interval > 2 * inteW) {
                inte_begin = HitBTime[i] - inteW + 1;
                singleHit = 1;
                } else
                inte_begin = int(HitBTime[i] - interval / 2.);
            } else {
                int single1 = 0, single2 = 0;
                int interval = HitBTime[i + 1] - HitETime[i];
                if (interval > inteW * 2) {
                inte_end = HitETime[i] + inteW;
                single1 = 1;
                } else
                inte_end = int(HitETime[i] + interval / 2.);
                interval = HitBTime[i] - HitETime[i - 1];
                if (interval > 2 * inteW) {
                inte_begin = HitBTime[i] - inteW + 1;
                single2 = 1;
                } else
                inte_begin = int(HitBTime[i] - interval / 2.);
                if (single1 == 1 && single2 == 1) singleHit = 1;
            }
            }
            double tempC = 0;
            for (int j = inte_begin; j < inte_end; j++) tempC += AC[j];
            // if (m_hc) {  // the hit counting method is opened
            // if (singleHit) {
            //     if (tempC < 1.55) tempC = 1;  // A simple hit counting, to remove the PMT amplitude fluctuations.
            //                                 // else if(tempC<2.5) tempC = 2;
            //                                 // else if(tempC<3.5) tempC = 3;
            // }
            // }
            charge.push_back(tempC);
        }
        width = hitWidth;
        peak = hitPeakCounter;

        hit_begin = new TH1D(Form("%d_hit_begin",n),Form("%d_hit_begin",n),m_length,0,m_length);
        for(int i=0;i<time.size();++i) hit_begin->SetBinContent(time.at(i)+1,charge.at(i));
        ojw->cd();
        hit_begin->Write();
    }

    wf_f->Close();
    ojw->Close();
    SPEtf->Close();
}