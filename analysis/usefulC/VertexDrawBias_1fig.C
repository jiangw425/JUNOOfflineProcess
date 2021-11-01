#include<vector>
const int eNum = 5;
const int cNum = 2;
const int CheckE = 2;
const int E[5] = {0, 1, 3, 5, 8};
//const int E[5] = {0, 2, 4, 6, 8};
const int EColor[5] = {1, 2, 4, 7, 8};
//const int EColor[5] = {1, 8, 4, 7, 417};

#define TMLE
void VertexDrawBias_1fig() {
    double lm = 0.13;
    double rangeLow = -100;
    double rangeUp = 100;
    //TCanvas* cR = new TCanvas("cR", "cR",200,10,700,500);
    //cR->SetRightMargin(lm);


    //auto  legend = new TLegend(0.16, 0.17, 0.45, 0.47);
    auto  legend = new TLegend(0.22, 0.58, 0.52, 0.88);
    legend->SetFillStyle(0);
    legend->SetTextSize(0.05);
    legend->SetFillColor(0);
    
    double m_leftm = 0.16;
    double stW = 1./(1./(1. - m_leftm)  + 1. + 1./(1. - m_leftm));
    double topMargin = 0.0;

    TCanvas* cR = new TCanvas("cR","cR",1500, 500);
    TPad* c1 = new TPad("c1", "c1", 0.0, 0.01, stW/(1. - m_leftm), 0.99);
    TPad* c2 = new TPad("c2", "c2", stW/(1. - m_leftm), 0.01, stW/(1. - m_leftm) + stW, 0.99);
    TPad* c3 = new TPad("c3", "c3", stW/(1. - m_leftm) + stW, 0.01, 1., 0.99);

    c1->SetLeftMargin(m_leftm);
    c1->SetRightMargin(0.);
    c2->SetLeftMargin(0.);
    c2->SetRightMargin(0.);
    c3->SetLeftMargin(0.);
    c3->SetRightMargin(m_leftm);

    c1->Draw();
    c2->Draw();
    c3->Draw();

  
    for(int eid =0; eid<5; eid++) {
        // std::string filename = "Assemb.root";
        // TFile* sfile = NULL;
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsLaser_noaln_TTS_TDN_3pe/Assemb.root", E[eid]),"READ"); 
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_106R_corr2DN_pro/Assemb.root", E[eid]),"READ"); 
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_35R_corr2DN_pro_ContV2_remove/Assemb.root", E[eid]),"READ"); 
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_paln_more_QTTSDN_6peSimp_cal_35R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20/Assemb.root", E[eid]),"READ"); 
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_35R_corr2_pro_ContV2_2/%s", E[eid], filename.c_str()),"READ"); 
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_35R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_1peAln/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_106R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_1peAln_fit/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_106R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_1peAln_fit_corr_smooth/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_106R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_1peLAln_fit_corr_smooth_check_3pecut/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_106R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_1peAln_fit_corr_smooth/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_106R_corr2DN_pro_fitGeL/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_106R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_1peLAln_fitEdge_corr_smooth_check_test/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_106R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_fitEdge_corr_checkInit/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_106R_corr2DN_pro_fitGeL_checkInit/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_35R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_fitEdge_corr_checkInit/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_35R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_fitEdge_corr_checkInit_DynOnly/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_106R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_fitEdge_corr_checkInit/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_106R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_fitEdge_corr_checkInit_1peGeE_smearV/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_35R_corr2DN_pro_fitGeL_checkInit_tune_corrV/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_35R_corr2DN_pro_fitGeL_checkInit_tune_corrV/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_35R_corr2DN_pro_fitGeL_checkInit_tune_corrV/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_35R_corr2DN_pro_fitGeL_checkInit_tune_corrV_fitGeL/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_106R_corr2DN_pro_fitGeL_checkInit_tune_corrV_fitGeL/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_35R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_fitEdge_corr_checkInit_1peGeE_corrV_check/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_35R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_fitEdge_corr_checkInit_1peGeE_recV/%s", E[eid], filename.c_str()),"READ");
        // sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_106R_corr2DN_pro_fitGeL_checkInit_tune_corrV/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_35R_corr2DN_pro_fitGeL_checkInit_tune_Ge_corr/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_106R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_fitEdge_corr_checkInit_1peGeE_corrV/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_TTSDN_6peSimp_cal_35R_corr2DN_pro_fitGeL_checkInit_tune/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime1.5nsGeL_paln_more_QTTSDN_6peSimp_cal_35R_corr2DN_pro_fitGeL_checkInit_tune/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_106R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_1peLAln_fit_corr_smooth_check/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_106R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_1peAln_fit_corr_smooth_Ge/%s", E[eid], filename.c_str()),"READ");
        //sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_paln_more_QTTSDN_8peSimp_cal_35R_corr2_pro_ContV2_2_checkGeL6pe_cmps_J20_fit/%s", E[eid], filename.c_str()),"READ");
        //TFile* sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_35R_corr2_pro_ContV2_2/Assemb.root", E[eid]),"READ"); 
        //TFile* sfile = TFile::Open(Form("/junofs/users/huanggh/EnergyRec/CalibData_J20/ZXT/UnifPositron_Pro_More/%dMeV/FreeV_e+MapCorrE_L_MuRT2Dh_0-8_CubicRComb_TruthPE_ContV2CalibPos_WR1.63_Tr3_50_Real_FineTime0.5nsGeL_galn_more_6peSimp_cal_35R_corr2_pro_ContV2_2/Assemb.root", E[eid]),"READ"); 
        TFile *sfile = TFile::Open(Form("%dMeV/Assemb.root",E[eid]));
        cout<< sfile->GetName()<<endl;
        TH2D* hQ_RBias = new TH2D(Form("hQ_RBias_%d", eid), Form("hQ_RBias_%d", eid), 100, 0, 6000, 100, -1000, 1000);
        hQ_RBias->SetTitle("");
        hQ_RBias->GetXaxis()->SetTitle("r_{true}^{3} [m^{3}]");
        hQ_RBias->GetYaxis()->SetTitle("r_{rec} - r_{true} [mm]");
        hQ_RBias->GetXaxis()->SetRangeUser(0, 5939);
        hQ_RBias->GetXaxis()->SetLabelSize(0.05);
        
        TH2D* hQ_ThetaBias = new TH2D("hQ_ThetaBias", "hQ_ThetaBias", 100, 0, 6000, 100, -0.5, 0.5);
        hQ_ThetaBias->SetTitle("");
        hQ_ThetaBias->GetXaxis()->SetTitle("r_{true}^{3} [m^{3}]");
        hQ_ThetaBias->GetYaxis()->SetTitle("#theta_{rec} - #theta_{true} [rad]");

        TH2D* hQ_PhiBias = new TH2D("hQ_PhiBias", "hQ_PhiBias", 100, 0, 6000, 100,  -0.5,  0.5);
        hQ_PhiBias->SetTitle("");
        hQ_PhiBias->GetXaxis()->SetTitle("r_{true}^{3} [m^{3}]");
        hQ_PhiBias->GetYaxis()->SetTitle("#phi_{rec} - #phi_{true} [rad]");

        //------------------------------
        TH2D* hT_RBias = new TH2D(Form("hT_RBias_%d", eid), Form("hT_RBias_%d", eid), 100, 0, 6000, 100, -1000, 1000);
        hT_RBias->SetTitle("");
        hT_RBias->GetXaxis()->SetTitle("r_{true}^{3} [m^{3}]");
        hT_RBias->GetYaxis()->SetTitle("r_{rec} - r_{true} [mm]");
        hT_RBias->GetXaxis()->SetRangeUser(0, 5939);
        hT_RBias->GetXaxis()->SetLabelSize(1.15*0.05);
        hT_RBias->GetXaxis()->SetTitleSize(1.15*0.06);

        TH2D* hT_ThetaBias = new TH2D("hT_ThetaBias", "hT_ThetaBias", 100, 0, 6000, 100, -0.5, 0.5);
        hT_ThetaBias->SetTitle("");
        hT_ThetaBias->GetXaxis()->SetTitle("r_{true}^{3} [m^{3}]");
        hT_ThetaBias->GetYaxis()->SetTitle("#theta_{rec} - #theta_{true} [rad]");

        TH2D* hT_PhiBias = new TH2D("hT_PhiBias", "hT_PhiBias", 100, 0, 6000, 100,  -0.5,  0.5);
        hT_PhiBias->SetTitle("");
        hT_PhiBias->GetXaxis()->SetTitle("r_{true}^{3} [m^{3}]");
        hT_PhiBias->GetYaxis()->SetTitle("#phi_{rec} - #phi_{true} [rad]");

        //--------------------------
        TH2D* hQT_RBias = new TH2D(Form("hQT_RBias_%d", eid), Form("hQT_RBias_%d", eid), 100, 0, 6000, 100, -1000, 1000);
        hQT_RBias->SetTitle("");
        hQT_RBias->GetXaxis()->SetTitle("r_{true}^{3} [m^{3}]");
        hQT_RBias->GetYaxis()->SetTitle("r_{rec} - r_{true} [mm]");
        hQT_RBias->GetXaxis()->SetLabelSize(0.05);

        TH2D* hQT_ThetaBias = new TH2D("hQT_ThetaBias", "hQT_ThetaBias", 100, 0, 6000, 100, -0.5, 0.5);
        hQT_ThetaBias->SetTitle("");
        hQT_ThetaBias->GetXaxis()->SetTitle("r_{true}^{3} [m^{3}]");
        hQT_ThetaBias->GetYaxis()->SetTitle("#theta_{rec} - #theta_{true} [rad]");

        TH2D* hQT_PhiBias = new TH2D("hQT_PhiBias", "hQT_PhiBias", 100, 0, 6000, 100,  -0.5,  0.5);
        hQT_PhiBias->SetTitle("");
        hQT_PhiBias->GetXaxis()->SetTitle("r_{true}^{3} [m^{3}]");
        hQT_PhiBias->GetYaxis()->SetTitle("#phi_{rec} - #phi_{true} [rad]");



        TTree* stree = (TTree*)sfile->Get("evt");
        Float_t rec_x, rec_y, rec_z, Qrec_x, Qrec_y, Qrec_z, True_x, True_y, True_z, TR, QR, recTR, QTR, RecR, QEn, QTEn, chi2;
        
        stree->SetBranchAddress("TrueR",&TR); 
        
        stree->SetBranchAddress("m_QR",&QR);
        stree->SetBranchAddress("m_TR",&recTR);
        //stree->SetBranchAddress("recTR",&recTR);
        stree->SetBranchAddress("m_QTR",&QTR);
        stree->SetBranchAddress("m_QEn",&QEn);
        stree->SetBranchAddress("m_QTEn",&QTEn);
        stree->SetBranchAddress("recx",&rec_x);
        stree->SetBranchAddress("recy",&rec_y);
        stree->SetBranchAddress("recz",&rec_z);
        stree->SetBranchAddress("recQx",&Qrec_x);
        stree->SetBranchAddress("recQy",&Qrec_y);
        stree->SetBranchAddress("recQz",&Qrec_z);
        stree->SetBranchAddress("Truex",&True_x);
        stree->SetBranchAddress("Truey",&True_y);
        stree->SetBranchAddress("Truez",&True_z);

        int Nentries = stree->GetEntries();
        for(int i=0;i<Nentries;i++) {
            stree->GetEntry(i);
            if(TR>17399.) continue;
            hQ_RBias->Fill(TR*TR*TR/1.e9, QR - TR);
            hQ_ThetaBias->Fill(TR*TR*TR/1.e9, TMath::ACos(Qrec_z/QR) - TMath::ACos(True_z/TR));
            hQ_PhiBias->Fill(TR*TR*TR/1.e9, TMath::ATan(Qrec_y/Qrec_x) - TMath::ATan(True_y/True_x));
             
            if(TR>17399.) continue;
            hT_RBias->Fill(TR*TR*TR/1.e9, recTR - TR);
            hT_ThetaBias->Fill(TR*TR*TR/1.e9, TMath::ACos(rec_z/QTR) - TMath::ACos(True_z/TR));
            hT_PhiBias->Fill(TR*TR*TR/1.e9, TMath::ATan(rec_y/rec_x) - TMath::ATan(True_y/True_x));

            if(TR>17399.) continue;
            hQT_RBias->Fill(TR*TR*TR/1.e9, QTR - TR);
            hQT_ThetaBias->Fill(TR*TR*TR/1.e9, TMath::ACos(rec_z/QTR) - TMath::ACos(True_z/TR));
            hQT_PhiBias->Fill(TR*TR*TR/1.e9, TMath::ATan(rec_y/rec_x) - TMath::ATan(True_y/True_x));
        }
        
        //hQ_RBias->Draw("COLZ");
        
        c1->cd();
        //TH1D* rprof_Q = hQ_RBias->ProfileX("rprof_Q");
        TH1D* rprof_Q = new TH1D(Form("rprof_Q%d", eid), "",100, 0, 6000);
        for(int i=1;i<=100;i++) {
            TH1D* htemp = hQ_RBias->ProjectionY(Form("proj%d", i), i,i);
            if(htemp->GetSumOfWeights()<2) continue;
            htemp->Fit("gaus", "QR0", "", -1000, 1000);
            TF1* func = htemp->GetFunction("gaus");
            double mean = func->GetParameter(1);
            double sigm = func->GetParError(1);
            rprof_Q->SetBinContent(i, mean);
            rprof_Q->SetBinError(i, sigm);
            delete func;
            delete htemp;
        }
        rprof_Q->SetName(Form("rprof_%d", eid));
        rprof_Q->SetMarkerStyle(20);
        rprof_Q->SetMarkerSize(0.5);
        rprof_Q->SetMarkerColor(EColor[eid]);
        rprof_Q->SetLineColor(EColor[eid]);
        if(eid==0) {
            rprof_Q->Draw();
            rprof_Q->GetYaxis()->SetRangeUser(rangeLow, rangeUp);
            rprof_Q->GetYaxis()->SetTitle("Radial bias [mm]");
            rprof_Q->GetYaxis()->SetTitleOffset(1.2);
        }
        else rprof_Q->Draw("SAME E");

        c2->cd();
        //TProfile* rprof_T = hT_RBias->ProfileX("rprof_T");
        TH1D* rprof_T = new TH1D(Form("rprof_T%d", eid), "",100, 0, 6000);
        for(int i=1;i<=100;i++) {
            TH1D* htemp = hT_RBias->ProjectionY(Form("proj%d", i), i,i);
            if(htemp->GetSumOfWeights()<2) continue;
            htemp->Fit("gaus", "QR0", "", -1000, 1000);
            TF1* func = htemp->GetFunction("gaus");
            double mean = func->GetParameter(1);
            double sigm = func->GetParError(1);
            rprof_T->SetBinContent(i, mean);
            rprof_T->SetBinError(i, sigm);
            delete func;
            delete htemp;
        }
        rprof_T->SetName(Form("rprof_%d", eid));
        rprof_T->SetMarkerStyle(20);
        rprof_T->SetMarkerSize(0.5);
        rprof_T->SetMarkerColor(EColor[eid]);
        rprof_T->SetLineColor(EColor[eid]);
        if(eid==0) {
            rprof_T->Draw();
            rprof_T->GetYaxis()->SetRangeUser(rangeLow, rangeUp);
            rprof_T->GetYaxis()->SetTitle("Radial bias [mm]");
            rprof_T->GetXaxis()->SetLabelSize(0.062);
        }
        else rprof_T->Draw("SAME E");
    
        c3->cd();
        //TProfile* rprof_QT = hQT_RBias->ProfileX("rprof_QT");
        TH1D* rprof_QT = new TH1D(Form("rprof_QT%d", eid), "",100, 0, 6000);
        for(int i=1;i<=100;i++) {
            TH1D* htemp = hQT_RBias->ProjectionY(Form("proj%d", i), i,i);
            if(htemp->GetSumOfWeights()<2) continue;
            htemp->Fit("gaus", "QR0", "", -1000, 1000);
            TF1* func = htemp->GetFunction("gaus");
            double mean = func->GetParameter(1);
            double sigm = func->GetParError(1);
            rprof_QT->SetBinContent(i, mean);
            rprof_QT->SetBinError(i, sigm);
            delete func;
            delete htemp;
        }
        rprof_QT->SetName(Form("rprof_%d", eid));
        rprof_QT->SetMarkerStyle(20);
        rprof_QT->SetMarkerSize(0.5);
        rprof_QT->SetMarkerColor(EColor[eid]);
        rprof_QT->SetLineColor(EColor[eid]);
        if(eid==0) {
            rprof_QT->Draw();
            rprof_QT->GetYaxis()->SetRangeUser(rangeLow, rangeUp);
            rprof_QT->GetYaxis()->SetTitle("Radial bias [mm]");
            rprof_QT->GetXaxis()->SetTitle("r^{3} [m^{3}]");
        }
        else rprof_QT->Draw("SAME E");
 
        
        TLegendEntry* l1 = legend->AddEntry(rprof_Q, Form("E_{k}= %d MeV", E[eid]), "lp");
        l1->SetTextColor(EColor[eid]);     

        
        /*
        TCanvas* cT = new TCanvas("cT", "cT",200,10,700,500);
        cT->SetRightMargin(lm);
        //hQ_ThetaBias->Draw("COLZ");
        TProfile* tprofx = hQ_ThetaBias->ProfileX("tprofx");
        tprofx->SetMarkerStyle(20);
        tprofx->SetMarkerSize(0.5);
        tprofx->Draw("SAME");

        TCanvas* cP = new TCanvas("cP", "cP",200,10,700,500);
        cP->SetRightMargin(lm);
        //hQ_PhiBias->Draw("COLZ");
        TProfile* pprofx = hQ_PhiBias->ProfileX("pprofx");
        pprofx->SetMarkerStyle(20);
        pprofx->SetMarkerSize(0.5);
        pprofx->Draw("SAME");
        */
    }
    c1->cd();
    legend->Draw();
    TBox* boxres= new TBox(17.2*17.2*17.2, rangeLow, 17.7*17.7*17.7, rangeUp);
    boxres->SetFillStyle(3003);
    boxres->SetFillColor(kPink);
    boxres->Draw("SAME");
    
    TText* text = new TText(4000, 80, "QMLE");
    text->SetTextSize(0.06);
    text->Draw("SAME");
 
    c2->cd();
    TText* text_T = new TText(4000, 80, "TMLE");
    text_T->SetTextSize(0.06*1.15);
    text_T->Draw("SAME");
    boxres->Draw("SAME");

    c3->cd();
    TText* text_QT = new TText(4000, 80, "QTMLE");
    text_QT->SetTextSize(0.06);
    text_QT->Draw("SAME");
    boxres->Draw("SAME");
}
