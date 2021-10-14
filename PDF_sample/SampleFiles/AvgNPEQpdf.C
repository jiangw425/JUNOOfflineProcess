const int TotalPMT = 17612;
const int length = 4000;
const int peRange = 100;

void AvgNPEQpdf()
{
    std::string base = getenv("JUNOTOP");
    std::ifstream datain;
    datain.open(base + "/data/Simulation/ElecSim/Hamamatsu_pmtID.txt", std::ios::in);

    bool* IsDyn = new bool[TotalPMT];
    for(int i=0;i<TotalPMT;++i) IsDyn[i] = false;
    int dTemp = 0;
    while(datain>>dTemp) {
        IsDyn[dTemp] = true;
    }
    datain.close();
 
    double prob[length];
    double q[length];
    int k = 0;
    TFile* AssembFile = new TFile("AvgNPEQpdf.root", "RECREATE");
    TH1F* McpAvgQPdf[peRange];
    TH1F* DynAvgQPdf[peRange];
    for(int i=0;i<peRange;i++) {
        McpAvgQPdf[i] = new TH1F(Form("nPE%d_mcp_qpdf", i+1), Form("nPE%d_mcp_qpdf", i+1), 4000, 0, 200);
        DynAvgQPdf[i] = new TH1F(Form("nPE%d_dyn_qpdf", i+1), Form("nPE%d_dyn_qpdf", i+1), 4000, 0, 200);
    }
    for(int i=0;i<881;i++) {
        std::cout<<i<<std::endl;
        TFile* FileTemp = new TFile(Form("../runscript/NPEPDF/SPEsQPDF_%d.root", k), "READ");
        for(int c=k;c<k+20&&c<TotalPMT;c++) {
            for(int nPE = 0; nPE < peRange; nPE ++) {
                TH1F* hTemp = (TH1F*)FileTemp->Get(Form("channel%d_nPE%d_qpdf", c, nPE+1));
                if(!IsDyn[c]) (McpAvgQPdf[nPE])->Add(hTemp);
                else (DynAvgQPdf[nPE])->Add(hTemp);
                delete hTemp;
            }
        }  
        k += 20;
        delete FileTemp;
    }
    AssembFile->cd();
    for(int i=0;i<peRange;i++) {
        McpAvgQPdf[i]->Scale(1./McpAvgQPdf[i]->GetSumOfWeights());
        McpAvgQPdf[i]->Write();
        DynAvgQPdf[i]->Scale(1./DynAvgQPdf[i]->GetSumOfWeights());
        DynAvgQPdf[i]->Write();
    }
}
