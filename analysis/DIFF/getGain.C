#include "/junofs/users/jiangw/include/pmtflag.h"
void getGain()
{
	TFile *f = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre0/data/Simulation/ElecSim/PmtData_Lpmt.root");
	TTree *t = (TTree*)f->Get("PmtData_Lpmt");
	double gain, pde;
	t->SetBranchAddress("Gain",&gain);
	t->SetBranchAddress("PDE",&pde);
	bool *hmmtpmt = pmtflag();
	TH2D *th = new TH2D("th","MCP PMT gain truth vs pmtid",17612,0,17612,100,9*1e6,12*1e6);
	TH1D *pde1d=new TH1D("pde1d","pde",200,0,0.2);
	for(int i=0; i<17613;++i)
	{
		t->GetEntry(i);
		if(!hmmtpmt[i])
		th->Fill(i,gain);
		pde1d->Fill(pde/29.23*0.1121);
	}
	th->Draw();
	//pde1d->Draw();
}
