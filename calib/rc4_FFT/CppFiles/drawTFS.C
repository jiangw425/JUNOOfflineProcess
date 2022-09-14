#include "INCLUDE"
void drawTFS(){
	bool *hmmt = pmtflag();
	const int N =17612;
	float tfs[N] = {0};
	float tmp;

	ifstream ijw("timeOffset_roofit.txt");
	//ifstream ijw("timeOffset_linear.txt");
	for(int i=0;i<N;++i){
		ijw >> tmp >> tfs[i] >> tmp >> tmp >> tmp >> tmp;
	}
	ijw.close();

	float sum=0;
	for(int i=0;i<N;++i){
		sum+=tfs[i];
	}
	sum/=N*1.0;
	for(int i=0;i<N;++i){
		tfs[i]-=sum;
	}
	cout<<"mean:"<<sum<<endl;	
	
	ofstream ojw("timeOffset_sub_roofit.txt");
	//ofstream ojw("timeOffset_linear_sub.txt");
	for(int i=0;i<N;++i){
		// ojw <<fixed<<setprecision(1)<< tfs[i] << endl;
		ojw << tfs[i] << endl;
	}

	TH1D *t1D[2];
	t1D[0] = new TH1D("nnvt","nnvt",100,-50,50);
	t1D[1] = new TH1D("hmmt","hmmt",100,-50,50);

	for(int i=0;i<N;++i){
		t1D[int(hmmt[i])]->Fill(tfs[i]);
		//if(tfs[i]<367) cout<< i<<endl;
	}
	t1D[1]->SetLineColor(kRed);

	TCanvas *cjw = new TCanvas("cjw","cjw",1600,900);
	t1D[0]->Draw();
	t1D[1]->Draw("SAME");
	cjw->Print("TimeOffset.png");
}
