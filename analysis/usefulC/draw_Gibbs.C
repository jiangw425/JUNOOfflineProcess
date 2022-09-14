void draw_Gibbs(){
    int m_length = 1250;
    const int N = m_length;
    // const int nf = 300;
    TVirtualFFT* fft_forward=TVirtualFFT::FFT(1, &m_length, "R2C EX K");
    TVirtualFFT* fft_back = TVirtualFFT::FFT(1, &m_length, "C2R EX K");

    TH1D *w_tmp = new TH1D("h","h",m_length,0,m_length);
    TH1D *filter = new TH1D("f","f",600,0,600);
    TH1D *input_wf = 0;
    TH1D *input_bsl_wf = 0;
    TH1D *fft_wf = 0;
    TH1D *fft_bsl_wf = 0;
    TH1D *fft_acc_wf = 0;
    TH1D *hit_begin = 0;
    for(int i=0;i<m_length;i++) w_tmp->SetBinContent(i+1,0);
    for(int i=450;i<500;i++) w_tmp->SetBinContent(i+1,0.5);
    for(int i=0;i<180;i++) filter->SetBinContent(i+1,1);

    double input_rawh[m_length];
    for(int j=0;j<m_length;++j) input_rawh[j] = w_tmp->GetBinContent(j+1);
    std::vector<double> re_full_vec(N);
    std::vector<double> im_full_vec(N);
    double *re_full = &re_full_vec.front();
    double *im_full = &im_full_vec.front();
    for (int i = 0; i < N; i++) {
        re_full[i] = 0;
        im_full[i] = 0;
    }
    fft_forward->SetPoints(input_rawh);
    fft_forward->Transform();
    fft_forward->GetPointsComplex(re_full, im_full);
    for (int i = 180; i < N; i++) {
        re_full[i] = 0;
        im_full[i] = 0;
    }
    fft_back->SetPointsComplex(re_full, im_full);
    fft_back->Transform();
    TH1 *Back = 0;
    Back = TH1::TransformHisto(fft_back, Back, "Re");
    Back->Scale(1. / m_length);
    fft_wf = new TH1D(Form("_fft_wf"),Form("_fft_wf"),m_length,0,m_length);
    for(int i=0;i<m_length;++i) fft_wf->SetBinContent(i+1,Back->GetBinContent(i+1));
    fft_wf->SetLineColor(kRed);
    fft_wf->GetXaxis()->SetRangeUser(400,550);
    fft_wf->Draw();
    w_tmp->Draw("SAME");
    // fft_wf->Draw("SAME");
    
}