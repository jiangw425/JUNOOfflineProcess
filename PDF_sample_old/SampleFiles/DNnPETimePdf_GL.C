
const int NPE = 8;
const int tBinN = 400;
const int dBinN = 200;
const int TrBinN = 35;
const double tRange = 600.;
const double dRange = 40.;

const double tbinW = tRange/double(tBinN);
const double FullADCBinN =  1250./tbinW;
static const double DNProb  = 1./FullADCBinN;

void DNnPETimePdf_GL()
{
    std::string GeDynTimePdfFileName = "TimePdfFile_Ge68_dyn.root";
    std::string GeMcpTimePdfFileName = "TimePdfFile_Ge68_mcp.root";
    TFile* GeDynTimePdfFile = TFile::Open((GeDynTimePdfFileName).c_str(), "READ");
    TFile* GeMcpTimePdfFile = TFile::Open((GeMcpTimePdfFileName).c_str(), "READ");

    std::string LDynTimePdfFileName = "TimePdfFile_Laser0.05_dyn.root";
    std::string LMcpTimePdfFileName = "TimePdfFile_Laser0.05_mcp.root";
    TFile* LDynTimePdfFile = TFile::Open((LDynTimePdfFileName).c_str(), "READ");
    TFile* LMcpTimePdfFile = TFile::Open((LMcpTimePdfFileName).c_str(), "READ");
 
    TFile* TimePdfFile = TFile::Open("TimePdfFile.root", "RECREATE");

    //////////////
    // Ge TimeInfo
    //1pe
    TH2F* GeDynProb[TrBinN];
    TH2F* GeMcpProb[TrBinN];

    TH2F* hGeDynIProb[TrBinN];
    TH2F* hGeMcpIProb[TrBinN];

    //2pe
    TH1F*   GeDyn_I_ft_Ft[TrBinN];
    TH1F*  GeDyn_I_ft_Fdt[TrBinN];
    TH1F*  GeDyn_I_fdt_Ft[TrBinN];
    TH1F* GeDyn_I_fdt_Fdt[TrBinN];

    TH1F*   GeMcp_I_ft_Ft[TrBinN];
    TH1F*  GeMcp_I_ft_Fdt[TrBinN];
    TH1F*  GeMcp_I_fdt_Ft[TrBinN];
    TH1F* GeMcp_I_fdt_Fdt[TrBinN];

    //////////////////
    // Laser TimeInfo
    //1pe
    TH2F* LDynProb[TrBinN];
    TH2F* LMcpProb[TrBinN];

    TH2F* hLDynIProb[TrBinN];
    TH2F* hLMcpIProb[TrBinN];

    //2pe
    TH1F*   LDyn_I_ft_Ft[TrBinN];
    TH1F*  LDyn_I_ft_Fdt[TrBinN];
    TH1F*  LDyn_I_fdt_Ft[TrBinN];
    TH1F* LDyn_I_fdt_Fdt[TrBinN];

    TH1F*   LMcp_I_ft_Ft[TrBinN];
    TH1F*  LMcp_I_ft_Fdt[TrBinN];
    TH1F*  LMcp_I_fdt_Ft[TrBinN];
    TH1F* LMcp_I_fdt_Fdt[TrBinN];


    ///////////////
    // GeL TimeInfo
    TH1F*  Dyn_I_Geft_LFt[TrBinN];
    TH1F*  Dyn_I_Lft_GeFt[TrBinN];

    TH1F*  Mcp_I_Geft_LFt[TrBinN];
    TH1F*  Mcp_I_Lft_GeFt[TrBinN];

    int NonZeroI = 0;
    for(int rk=0;rk<TrBinN;rk++) {
        int Rk=rk;

        TH2D* GeDynProb_test = (TH2D*)GeDynTimePdfFile->Get(Form("RsdTimeFuncRec_%d_0", rk));
        if(GeDynProb_test->GetSumOfWeights()>1.) NonZeroI = rk;
        if(GeDynProb_test->GetSumOfWeights()<1.) Rk=NonZeroI;

        TH2D* GeDynProb_0 = (TH2D*)GeDynTimePdfFile->Get(Form("RsdTimeFuncRec_%d_0", Rk));
        TH2D* GeMcpProb_0 = (TH2D*)GeMcpTimePdfFile->Get(Form("RsdTimeFuncRec_%d_0", Rk));
        TH2D* LDynProb_0 = (TH2D*)LDynTimePdfFile->Get(Form("RsdTimeFuncRec_%d_0", Rk));
        TH2D* LMcpProb_0 = (TH2D*)LMcpTimePdfFile->Get(Form("RsdTimeFuncRec_%d_0", Rk));

        GeDynProb_0->Smooth();
        GeMcpProb_0->Smooth();
        LDynProb_0->Smooth();
        LMcpProb_0->Smooth();

        int GeBeginBin = 0;
        int GeEndBin = 0;
        bool foundBeginBin = false;
        bool foundEndBin = false;
        for(int checkBin = 1; checkBin <= 200; checkBin++) {
            TH1D* GeDynPdf1pe = GeDynProb_0->ProjectionY("GeDynPdf1pe", checkBin, checkBin);
            TH1D* GeMcpPdf1pe = GeMcpProb_0->ProjectionY("GeMcpPdf1pe", checkBin, checkBin);
            double GeDynProbSum = GeDynPdf1pe->GetSumOfWeights();
            double MapProbSum = GeMcpPdf1pe->GetSumOfWeights();
            if(GeDynProbSum>0.5 && !foundBeginBin) {GeBeginBin = checkBin; foundBeginBin=true; break;}
        }
        for(int checkBin = 200; checkBin >= 1; checkBin--) {
            TH1D* GeDynPdf1pe = GeDynProb_0->ProjectionY("GeDynPdf1pe", checkBin, checkBin);
            TH1D* GeMcpPdf1pe = GeMcpProb_0->ProjectionY("GeMcpPdf1pe", checkBin, checkBin);
            double GeDynProbSum = GeDynPdf1pe->GetSumOfWeights();
            double MapProbSum = GeMcpPdf1pe->GetSumOfWeights();
            if(GeDynProbSum>0.5 && !foundEndBin) {GeEndBin = checkBin; foundEndBin=true; break;}
        }
        cout<< rk << '\t' << GeBeginBin << '\t' << GeEndBin << endl;

        int LBeginBin = 0;
        int LEndBin = 0;
        foundBeginBin = false;
        foundEndBin = false;
        for(int checkBin = 1; checkBin <= 200; checkBin++) {
            TH1D* LDynPdf1pe = LDynProb_0->ProjectionY("LDynPdf1pe", checkBin, checkBin);
            TH1D* LMcpPdf1pe = LMcpProb_0->ProjectionY("LMcpPdf1pe", checkBin, checkBin);
            double LDynProbSum = LDynPdf1pe->GetSumOfWeights();
            double MapProbSum = LMcpPdf1pe->GetSumOfWeights();
            if(LDynProbSum>0.5 && !foundBeginBin) {LBeginBin = checkBin; foundBeginBin=true; break;}
        }
        for(int checkBin = 200; checkBin >= 1; checkBin--) {
            TH1D* LDynPdf1pe = LDynProb_0->ProjectionY("LDynPdf1pe", checkBin, checkBin);
            TH1D* LMcpPdf1pe = LMcpProb_0->ProjectionY("LMcpPdf1pe", checkBin, checkBin);
            double LDynProbSum = LDynPdf1pe->GetSumOfWeights();
            double MapProbSum = LMcpPdf1pe->GetSumOfWeights();
            if(LDynProbSum>0.5 && !foundEndBin) {LEndBin = checkBin; foundEndBin=true; break;}
        }
        cout<< rk << '\t' << LBeginBin << '\t' << LEndBin << endl;



        GeDynProb[rk] = new TH2F(Form("hGeDynProb_%d", rk), Form("hGeDynProb_%d", rk), dBinN, 0, dRange, tBinN, 0, tRange);
        GeMcpProb[rk] = new TH2F(Form("hGeMcpProb_%d", rk), Form("hGeMcpProb_%d", rk), dBinN, 0, dRange, tBinN, 0, tRange);

        hGeDynIProb[rk] = new TH2F(Form("hGeDynIProb_%d", rk), Form("hGeDynIProb_%d", rk), dBinN, 0, dRange, tBinN, 0, tRange);
        hGeMcpIProb[rk] = new TH2F(Form("hGeMcpIProb_%d", rk), Form("hGeMcpIProb_%d", rk), dBinN, 0, dRange, tBinN, 0, tRange);

        //2pe
        GeDyn_I_ft_Ft[rk] = new TH1F(Form("GeDyn_I_ft_Ft_%d", rk), Form("GeDyn_I_ft_Ft_%d", rk), dBinN, 0, dRange);
        GeDyn_I_ft_Fdt[rk] = new TH1F(Form("GeDyn_I_ft_Fdt_%d", rk), Form("GeDyn_I_ft_Fdt_%d", rk), dBinN, 0, dRange);
        GeDyn_I_fdt_Ft[rk] = new TH1F(Form("GeDyn_I_fdt_Ft_%d", rk), Form("GeDyn_I_fdt_Ft_%d", rk), dBinN, 0, dRange);
        GeDyn_I_fdt_Fdt[rk] = new TH1F(Form("GeDyn_I_fdt_Fdt_%d", rk), Form("GeDyn_I_fdt_Fdt_%d", rk), dBinN, 0, dRange);

        GeMcp_I_ft_Ft[rk] = new TH1F(Form("GeMcp_I_ft_Ft_%d", rk), Form("GeMcp_I_ft_Ft_%d", rk), dBinN, 0, dRange);
        GeMcp_I_ft_Fdt[rk] = new TH1F(Form("GeMcp_I_ft_Fdt_%d", rk), Form("GeMcp_I_ft_Fdt_%d", rk), dBinN, 0, dRange);
        GeMcp_I_fdt_Ft[rk] = new TH1F(Form("GeMcp_I_fdt_Ft_%d", rk), Form("GeMcp_I_fdt_Ft_%d", rk), dBinN, 0, dRange);
        GeMcp_I_fdt_Fdt[rk] = new TH1F(Form("GeMcp_I_fdt_Fdt_%d", rk), Form("GeMcp_I_fdt_Fdt_%d", rk), dBinN, 0, dRange);

        TH1D* hDNGeDyn = GeDynProb_0->ProjectionX(Form("GeDynDN1pe_%d", rk), 1, 40);
        TH1D* hDNGeMcp = GeMcpProb_0->ProjectionX(Form("GeMcpDN1pe_%d", rk), 1, 40);
        hDNGeDyn->Scale(1./40.);
        hDNGeMcp->Scale(1./40.);

        //---------- Laser ----------
        LDynProb[rk] = new TH2F(Form("hLDynProb_%d", rk), Form("hLDynProb_%d", rk), dBinN, 0, dRange, tBinN, 0, tRange);
        LMcpProb[rk] = new TH2F(Form("hLMcpProb_%d", rk), Form("hLMcpProb_%d", rk), dBinN, 0, dRange, tBinN, 0, tRange);

        hLDynIProb[rk] = new TH2F(Form("hLDynIProb_%d", rk), Form("hLDynIProb_%d", rk), dBinN, 0, dRange, tBinN, 0, tRange);
        hLMcpIProb[rk] = new TH2F(Form("hLMcpIProb_%d", rk), Form("hLMcpIProb_%d", rk), dBinN, 0, dRange, tBinN, 0, tRange);

        //2pe
        LDyn_I_ft_Ft[rk] = new TH1F(Form("LDyn_I_ft_Ft_%d", rk), Form("LDyn_I_ft_Ft_%d", rk), dBinN, 0, dRange);
        LDyn_I_ft_Fdt[rk] = new TH1F(Form("LDyn_I_ft_Fdt_%d", rk), Form("LDyn_I_ft_Fdt_%d", rk), dBinN, 0, dRange);
        LDyn_I_fdt_Ft[rk] = new TH1F(Form("LDyn_I_fdt_Ft_%d", rk), Form("LDyn_I_fdt_Ft_%d", rk), dBinN, 0, dRange);
        LDyn_I_fdt_Fdt[rk] = new TH1F(Form("LDyn_I_fdt_Fdt_%d", rk), Form("LDyn_I_fdt_Fdt_%d", rk), dBinN, 0, dRange);

        LMcp_I_ft_Ft[rk] = new TH1F(Form("LMcp_I_ft_Ft_%d", rk), Form("LMcp_I_ft_Ft_%d", rk), dBinN, 0, dRange);
        LMcp_I_ft_Fdt[rk] = new TH1F(Form("LMcp_I_ft_Fdt_%d", rk), Form("LMcp_I_ft_Fdt_%d", rk), dBinN, 0, dRange);
        LMcp_I_fdt_Ft[rk] = new TH1F(Form("LMcp_I_fdt_Ft_%d", rk), Form("LMcp_I_fdt_Ft_%d", rk), dBinN, 0, dRange);
        LMcp_I_fdt_Fdt[rk] = new TH1F(Form("LMcp_I_fdt_Fdt_%d", rk), Form("LMcp_I_fdt_Fdt_%d", rk), dBinN, 0, dRange);

        TH1D* hDNLDyn = LDynProb_0->ProjectionX(Form("LDynDN1pe_%d", rk), 1, 40);
        TH1D* hDNLMcp = LMcpProb_0->ProjectionX(Form("LMcpDN1pe_%d", rk), 1, 40);
        hDNLDyn->Scale(1./40.);
        hDNLMcp->Scale(1./40.);

        //---------------------- GeL
        Dyn_I_Geft_LFt[rk] = new TH1F(Form("Dyn_I_Geft_LFt_%d", rk), Form("Dyn_I_Geft_LFt_%d", rk), dBinN, 0, dRange);
        Dyn_I_Lft_GeFt[rk] = new TH1F(Form("Dyn_I_Lft_GeFt_%d", rk), Form("Dyn_I_Lft_GeFt_%d", rk), dBinN, 0, dRange);

        Mcp_I_Geft_LFt[rk] = new TH1F(Form("Mcp_I_Geft_LFt_%d", rk), Form("Mcp_I_Geft_LFt_%d", rk), dBinN, 0, dRange);
        Mcp_I_Lft_GeFt[rk] = new TH1F(Form("Mcp_I_Lft_GeFt_%d", rk), Form("Mcp_I_Lft_GeFt_%d", rk), dBinN, 0, dRange);


        for(int checkBin = 1; checkBin <= dBinN; checkBin++) {
            int CheckBin = checkBin;
            if(checkBin<GeBeginBin) CheckBin=GeBeginBin;
            if(checkBin>GeEndBin) CheckBin=GeEndBin;
            
            //-----------------------------------Ge68
            TH1D* GeDynPdf1pe = GeDynProb_0->ProjectionY("GeDynPdf1pe", CheckBin, CheckBin);
            TH1D* GeMcpPdf1pe = GeMcpProb_0->ProjectionY("GeMcpPdf1pe", CheckBin, CheckBin);
            
            hDNGeDyn->SetBinContent(checkBin, hDNGeDyn->GetBinContent(CheckBin));
            hDNGeMcp->SetBinContent(checkBin, hDNGeMcp->GetBinContent(CheckBin));

            
            int ktemp = 1;
            while(GeDynPdf1pe->GetMaximum()<1e-3) {
                std::cout << "Empty bin <<<<<<<<<<<<<<<<< "<< rk <<'\t' << checkBin  << std::endl;
                GeDynPdf1pe = GeDynProb_0->ProjectionY("GeDynPdf1pe", CheckBin+ktemp, CheckBin+ktemp);
                GeMcpPdf1pe = GeMcpProb_0->ProjectionY("GeMcpPdf1pe", CheckBin+ktemp, CheckBin+ktemp);

                hDNGeDyn->SetBinContent(checkBin, hDNGeDyn->GetBinContent(CheckBin + ktemp));
                hDNGeMcp->SetBinContent(checkBin, hDNGeMcp->GetBinContent(CheckBin + ktemp));

                ktemp++;
                if(ktemp>dBinN) break;
            }
            if(ktemp > dBinN) continue;

            for(int i=1;i<=tBinN;i++) {
                double GeDynP1pe = GeDynPdf1pe->GetBinContent(i) - hDNGeDyn->GetBinContent(checkBin);
                if(GeDynP1pe<0.) GeDynP1pe = 0.;
                GeDynPdf1pe->SetBinContent(i, GeDynP1pe);

                double GeMcpP1pe = GeMcpPdf1pe->GetBinContent(i) - hDNGeMcp->GetBinContent(checkBin);
                if(GeMcpP1pe<0.) GeMcpP1pe = 0.;
                GeMcpPdf1pe->SetBinContent(i, GeMcpP1pe);

            }
            GeDynPdf1pe->Smooth();
            GeMcpPdf1pe->Smooth();

            GeDynPdf1pe->Scale(1./GeDynPdf1pe->GetSumOfWeights());
            GeMcpPdf1pe->Scale(1./GeMcpPdf1pe->GetSumOfWeights());
    
            for(int i=1;i<=tBinN;i++) {
                double IProbGeDyn = 0.;
                double IProbGeMcp = 0.;
                for(int j=i;j<=tBinN;j++) {
                    double GeDynProb1pe = GeDynPdf1pe->GetBinContent(j);
                    double GeMcpProb1pe = GeMcpPdf1pe->GetBinContent(j);
                    IProbGeDyn += GeDynProb1pe;
                    IProbGeMcp += GeMcpProb1pe;
                }
                hGeDynIProb[rk]->SetBinContent(checkBin, i, IProbGeDyn);
                hGeMcpIProb[rk]->SetBinContent(checkBin, i, IProbGeMcp);

                GeDynProb[rk]->SetBinContent(checkBin, i, GeDynPdf1pe->GetBinContent(i));
                GeMcpProb[rk]->SetBinContent(checkBin, i, GeMcpPdf1pe->GetBinContent(i));

            }

            //-------------------------------------- Laser
            CheckBin = checkBin;
            if(checkBin<LBeginBin) CheckBin=LBeginBin;
            if(checkBin>LEndBin) CheckBin=LEndBin;

            TH1D* LDynPdf1pe = LDynProb_0->ProjectionY("LDynPdf1pe", CheckBin, CheckBin);
            TH1D* LMcpPdf1pe = LMcpProb_0->ProjectionY("LMcpPdf1pe", CheckBin, CheckBin);

            hDNLDyn->SetBinContent(checkBin, hDNLDyn->GetBinContent(CheckBin));
            hDNLMcp->SetBinContent(checkBin, hDNLMcp->GetBinContent(CheckBin));

            ktemp = 1;
            while(LDynPdf1pe->GetMaximum()<1e-3) {
                std::cout << "Empty bin <<<<<<<<<<<<<<<<< "<< rk <<'\t' << checkBin  << std::endl;
                LDynPdf1pe = LDynProb_0->ProjectionY("LDynPdf1pe", CheckBin+ktemp, CheckBin+ktemp);
                LMcpPdf1pe = LMcpProb_0->ProjectionY("LMcpPdf1pe", CheckBin+ktemp, CheckBin+ktemp);

                hDNLDyn->SetBinContent(checkBin, hDNLDyn->GetBinContent(CheckBin + ktemp));
                hDNLMcp->SetBinContent(checkBin, hDNLMcp->GetBinContent(CheckBin + ktemp));

                ktemp++;
            }
            for(int i=1;i<=tBinN;i++) {
                double LDynP1pe = LDynPdf1pe->GetBinContent(i) - hDNLDyn->GetBinContent(checkBin);
                if(LDynP1pe<0.) LDynP1pe = 0.;
                LDynPdf1pe->SetBinContent(i, LDynP1pe);

                double LMcpP1pe = LMcpPdf1pe->GetBinContent(i) - hDNLMcp->GetBinContent(checkBin);
                if(LMcpP1pe<0.) LMcpP1pe = 0.;
                LMcpPdf1pe->SetBinContent(i, LMcpP1pe);

            }
            LDynPdf1pe->Smooth();
            LMcpPdf1pe->Smooth();

            LDynPdf1pe->Scale(1./LDynPdf1pe->GetSumOfWeights());
            LMcpPdf1pe->Scale(1./LMcpPdf1pe->GetSumOfWeights());
            
            for(int i=1;i<=tBinN;i++) {
                double IProbLDyn = 0.;
                double IProbLMcp = 0.;
                for(int j=i;j<=tBinN;j++) {
                    double LDynProb1pe = LDynPdf1pe->GetBinContent(j);
                    double LMcpProb1pe = LMcpPdf1pe->GetBinContent(j);
                    IProbLDyn += LDynProb1pe;
                    IProbLMcp += LMcpProb1pe;
                }
                hLDynIProb[rk]->SetBinContent(checkBin, i, IProbLDyn);
                hLMcpIProb[rk]->SetBinContent(checkBin, i, IProbLMcp);

                LDynProb[rk]->SetBinContent(checkBin, i, LDynPdf1pe->GetBinContent(i));
                LMcpProb[rk]->SetBinContent(checkBin, i, LMcpPdf1pe->GetBinContent(i));

            }

        
    
            //2pe
            double   GeDyn_I_ft_Ft_temp=0.;
            double  GeDyn_I_ft_Fdt_temp=0.;
            double  GeDyn_I_fdt_Ft_temp=0.;
            double GeDyn_I_fdt_Fdt_temp=0.;

            double   GeMcp_I_ft_Ft_temp=0.;
            double  GeMcp_I_ft_Fdt_temp=0.;
            double  GeMcp_I_fdt_Ft_temp=0.;
            double GeMcp_I_fdt_Fdt_temp=0.;

            //2pe
            double   LDyn_I_ft_Ft_temp=0.;
            double  LDyn_I_ft_Fdt_temp=0.;
            double  LDyn_I_fdt_Ft_temp=0.;
            double LDyn_I_fdt_Fdt_temp=0.;

            double   LMcp_I_ft_Ft_temp=0.;
            double  LMcp_I_ft_Fdt_temp=0.;
            double  LMcp_I_fdt_Ft_temp=0.;
            double LMcp_I_fdt_Fdt_temp=0.;
 
                
            double Dyn_I_Geft_LFt_temp=0.;
            double Dyn_I_Lft_GeFt_temp=0.;
            double Mcp_I_Geft_LFt_temp=0.;
            double Mcp_I_Lft_GeFt_temp=0.;

            for(int i=1;i<=tBinN;i++) {
                double GeDyn1peProb = GeDynPdf1pe->GetBinContent(i);
                double GeMcp1peProb = GeMcpPdf1pe->GetBinContent(i);

                double IGeDyn1peProb = hGeDynIProb[rk]->GetBinContent(checkBin, i);
                double IGeMcp1peProb = hGeMcpIProb[rk]->GetBinContent(checkBin, i);
                //2pe
                  GeDyn_I_ft_Ft_temp += GeDyn1peProb*IGeDyn1peProb;
                  GeMcp_I_ft_Ft_temp += GeMcp1peProb*IGeMcp1peProb;

                 GeDyn_I_ft_Fdt_temp += GeDyn1peProb*(double(tBinN - i)*DNProb);
                 GeMcp_I_ft_Fdt_temp += GeMcp1peProb*(double(tBinN - i)*DNProb);

                 GeDyn_I_fdt_Ft_temp += DNProb*IGeDyn1peProb;
                 GeMcp_I_fdt_Ft_temp += DNProb*IGeMcp1peProb;

                GeDyn_I_fdt_Fdt_temp += DNProb*(double(tBinN - i)*DNProb);
                GeMcp_I_fdt_Fdt_temp += DNProb*(double(tBinN - i)*DNProb);

                //------------------------------------ Laser 
                double LDyn1peProb = LDynPdf1pe->GetBinContent(i);
                double LMcp1peProb = LMcpPdf1pe->GetBinContent(i);

                double ILDyn1peProb = hLDynIProb[rk]->GetBinContent(checkBin, i);
                double ILMcp1peProb = hLMcpIProb[rk]->GetBinContent(checkBin, i);
                //2pe
                  LDyn_I_ft_Ft_temp += LDyn1peProb*ILDyn1peProb;
                  LMcp_I_ft_Ft_temp += LMcp1peProb*ILMcp1peProb;

                 LDyn_I_ft_Fdt_temp += LDyn1peProb*(double(tBinN - i)*DNProb);
                 LMcp_I_ft_Fdt_temp += LMcp1peProb*(double(tBinN - i)*DNProb);

                 LDyn_I_fdt_Ft_temp += DNProb*ILDyn1peProb;
                 LMcp_I_fdt_Ft_temp += DNProb*ILMcp1peProb;

                LDyn_I_fdt_Fdt_temp += DNProb*(double(tBinN - i)*DNProb);
                LMcp_I_fdt_Fdt_temp += DNProb*(double(tBinN - i)*DNProb);

                //------------------------------------ GeL
                Dyn_I_Geft_LFt_temp += GeDyn1peProb*ILDyn1peProb;
                Dyn_I_Lft_GeFt_temp += LDyn1peProb*IGeDyn1peProb;

                Mcp_I_Geft_LFt_temp += GeMcp1peProb*ILMcp1peProb;
                Mcp_I_Lft_GeFt_temp += LMcp1peProb*IGeMcp1peProb;
            }

            //2pe
            GeDyn_I_ft_Ft[rk]->SetBinContent(checkBin, GeDyn_I_ft_Ft_temp);
            GeDyn_I_ft_Fdt[rk]->SetBinContent(checkBin, GeDyn_I_ft_Fdt_temp);
            GeDyn_I_fdt_Ft[rk]->SetBinContent(checkBin, GeDyn_I_fdt_Ft_temp);
            GeDyn_I_fdt_Fdt[rk]->SetBinContent(checkBin, GeDyn_I_fdt_Fdt_temp);

            GeMcp_I_ft_Ft[rk]->SetBinContent(checkBin, GeMcp_I_ft_Ft_temp);
            GeMcp_I_ft_Fdt[rk]->SetBinContent(checkBin, GeMcp_I_ft_Fdt_temp);
            GeMcp_I_fdt_Ft[rk]->SetBinContent(checkBin, GeMcp_I_fdt_Ft_temp);
            GeMcp_I_fdt_Fdt[rk]->SetBinContent(checkBin, GeMcp_I_fdt_Fdt_temp);

            //----------------------- Laser
            LDyn_I_ft_Ft[rk]->SetBinContent(checkBin, LDyn_I_ft_Ft_temp);
            LDyn_I_ft_Fdt[rk]->SetBinContent(checkBin, LDyn_I_ft_Fdt_temp);
            LDyn_I_fdt_Ft[rk]->SetBinContent(checkBin, LDyn_I_fdt_Ft_temp);
            LDyn_I_fdt_Fdt[rk]->SetBinContent(checkBin, LDyn_I_fdt_Fdt_temp);

            LMcp_I_ft_Ft[rk]->SetBinContent(checkBin, LMcp_I_ft_Ft_temp);
            LMcp_I_ft_Fdt[rk]->SetBinContent(checkBin, LMcp_I_ft_Fdt_temp);
            LMcp_I_fdt_Ft[rk]->SetBinContent(checkBin, LMcp_I_fdt_Ft_temp);
            LMcp_I_fdt_Fdt[rk]->SetBinContent(checkBin, LMcp_I_fdt_Fdt_temp);

            //----------------------- GeL
            Dyn_I_Geft_LFt[rk]->SetBinContent(checkBin, Dyn_I_Geft_LFt_temp);
            Dyn_I_Lft_GeFt[rk]->SetBinContent(checkBin, Dyn_I_Lft_GeFt_temp);
            Mcp_I_Geft_LFt[rk]->SetBinContent(checkBin, Mcp_I_Geft_LFt_temp);
            Mcp_I_Lft_GeFt[rk]->SetBinContent(checkBin, Mcp_I_Lft_GeFt_temp);

            
            delete GeMcpPdf1pe;
            delete GeDynPdf1pe;
            delete LMcpPdf1pe;
            delete LDynPdf1pe;
        }

        TimePdfFile->cd();
        hDNGeDyn->Write();
        hDNGeMcp->Write();

        GeDynProb[rk]->Write();
        GeMcpProb[rk]->Write();

        hGeDynIProb[rk]->Write();
        hGeMcpIProb[rk]->Write();

        //2pe
          GeDyn_I_ft_Ft[rk]->Write();
         GeDyn_I_ft_Fdt[rk]->Write();
         GeDyn_I_fdt_Ft[rk]->Write();
        GeDyn_I_fdt_Fdt[rk]->Write();

          GeMcp_I_ft_Ft[rk]->Write();
         GeMcp_I_ft_Fdt[rk]->Write();
         GeMcp_I_fdt_Ft[rk]->Write();
        GeMcp_I_fdt_Fdt[rk]->Write();

        //------------------------
        hDNLDyn->Write();
        hDNLMcp->Write();

        LDynProb[rk]->Write();
        LMcpProb[rk]->Write();

        hLDynIProb[rk]->Write();
        hLMcpIProb[rk]->Write();

        //2pe
          LDyn_I_ft_Ft[rk]->Write();
         LDyn_I_ft_Fdt[rk]->Write();
         LDyn_I_fdt_Ft[rk]->Write();
        LDyn_I_fdt_Fdt[rk]->Write();

          LMcp_I_ft_Ft[rk]->Write();
         LMcp_I_ft_Fdt[rk]->Write();
         LMcp_I_fdt_Ft[rk]->Write();
        LMcp_I_fdt_Fdt[rk]->Write();


        //------------------
        Dyn_I_Geft_LFt[rk]->Write();
        Dyn_I_Lft_GeFt[rk]->Write();
        Mcp_I_Geft_LFt[rk]->Write();
        Mcp_I_Lft_GeFt[rk]->Write(); 


    }
}



                

