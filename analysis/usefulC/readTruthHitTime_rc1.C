#include "Event/ElecHeader.h"
#include "Event/ElecTruthHeader.h"
#include "Context/TimeStamp.h"
void readTruthHitTime_rc1(){
	const int length = 1250;
	string sname[4] = {"AmC","Ge68","Laser0.1","Laser0.05"};
	TH1D *hitTime[4];
	int lineColor[4] = {2,4,6,8};//红紫绿蓝
	TH1D *all_hitTime_hist = new TH1D("all_hitTime_hist","all_hitTime_hist",length+100,-50,length+50);

	for(int s=0; s<4; ++s){
		string name = sname[s] + "_hitTime";
        hitTime[s] = new TH1D(name.c_str(),name.c_str(),length+100,-50,length+50);
        hitTime[s]->SetLineColor(lineColor[s]);

		TChain *tTruth = new TChain("Event/Sim/Truth/LpmtElecTruthEvent");
		TChain *tElec = new TChain("Event/Elec/ElecEvent");
		tElec->Add(Form("root://junoeos01.ihep.ac.cn//eos/juno/valprod/valprod0/J22.1.0-rc0/ACU-CLS/%s/%s_0_0_0/elecsim/root/elecsim-*.root",sname[s].c_str(),sname[s].c_str()));
		tTruth->Add(Form("root://junoeos01.ihep.ac.cn//eos/juno/valprod/valprod0/J22.1.0-rc0/ACU-CLS/%s/%s_0_0_0/elecsim/root/elecsim-*.root",sname[s].c_str(),sname[s].c_str()));
		JM::ElecEvent* ee = new JM::ElecEvent();
		JM::LpmtElecTruthEvent* et = new JM::LpmtElecTruthEvent();
		tElec->SetBranchAddress("ElecEvent", &ee);
		tTruth->SetBranchAddress("LpmtElecTruthEvent", &et);
		cout << "Total Elec Events: " << tElec->GetEntries() << endl;
		cout << "Total ElecTruth Events: " << tTruth->GetEntries() << endl;
		if (tElec->GetEntries() != tTruth->GetEntries()) cout << "Wrong input data, please check!" << endl;

		for (int i = 0; i < tTruth->GetEntries()/100; i++) {
			if(i%100==0) cout<<"Processing "<< i << endl;
			tElec->GetEntry(i);
			tTruth->GetEntry(i);

			const JM::ElecFeeCrate& efc = ee->elecFeeCrate();
			JM::ElecFeeCrate* m_crate = const_cast<JM::ElecFeeCrate*>(&efc);
			const std::vector<JM::LpmtElecTruth>& let = et->truths();
			std::vector<JM::LpmtElecTruth>* m_truths;
			m_truths = const_cast<std::vector<JM::LpmtElecTruth>*>(&let);
			TimeStamp triggerTime = m_crate->TriggerTime();
			// TimeStamp evtTime     = m_crate->EvtTimeStamp();
			double m_trigTime = triggerTime.GetNanoSec();
			// double m_evtTime  = evtTime.GetNanoSec();
			// cout << m_truths->size() << endl;
			for (int j = 0; j < m_truths->size(); j++) {
				double m_pulseHitTime = m_truths->at(j).pulseHitTime().GetNanoSec(); 
		 		double m_waveformTime = m_pulseHitTime - m_trigTime + 100;
				// double m_hitTime = m_truths->at(j).hitTime();
				hitTime[s]->Fill(m_waveformTime);
				all_hitTime_hist->Fill(m_waveformTime);
			}
		}

		tTruth->~TChain();
		tElec->~TChain();
	}
	TCanvas *c = new TCanvas("c","c",1920,1080);
    // c->Divide(2,2);
	c->cd();
    all_hitTime_hist->Draw();
    for(int s=0;s<4;++s){
        // nPMT_hist[s]->Scale(1./nPMT_hist[s]->Integral());
        hitTime[s]->Draw("SAME HIST");
    }
	c->BuildLegend();

	// tElec->SetBranchAddress("ElecEvent", &ee);
	// tTruth->SetBranchAddress("LpmtElecTruthEvent", &et);
	// cout << "Total Elec Events: " << tElec->GetEntries() << endl;
	// cout << "Total ElecTruth Events: " << tTruth->GetEntries() << endl;
	// if (tElec->GetEntries() != tTruth->GetEntries()) cout << "Wrong input data, please check!" << endl;

	// for (int i = 0; i < 1; i++) {
	// 	cout << "evtID=" << i << endl;
	// 	tElec->GetEntry(i);
	// 	tTruth->GetEntry(i);

	// 	const JM::ElecFeeCrate& efc = ee->elecFeeCrate();
	// 	JM::ElecFeeCrate* m_crate = const_cast<JM::ElecFeeCrate*>(&efc);
	// 	const std::vector<JM::LpmtElecTruth>& let = et->truths();
	// 	std::vector<JM::LpmtElecTruth>* m_truths;
	// 	m_truths = const_cast<std::vector<JM::LpmtElecTruth>*>(&let);
	// 	TimeStamp triggerTime = m_crate->TriggerTime();
	// 	TimeStamp evtTime     = m_crate->EvtTimeStamp();
	// 	double m_trigTime = triggerTime.GetNanoSec();
	// 	double m_evtTime  = evtTime.GetNanoSec();
	// 	cout << m_truths->size() << endl;
	// 	for (int j = 0; j < m_truths->size(); j++) {
	// 		std::string m_pulsetype = m_truths->at(j).pulsetype();     
	// 		int m_pmtId = m_truths->at(j).pmtId();                     
	// 		int m_npe = m_truths->at(j).npe();                         
	// 		double m_hitTime = m_truths->at(j).hitTime();              
	// 		double m_amplitude = m_truths->at(j).amplitude();          
	// 		double m_TTS = m_truths->at(j).tts();                      
	// 		double m_timeoffset = m_truths->at(j).timeoffset();        
	// 		double m_pulseHitTime = m_truths->at(j).pulseHitTime().GetNanoSec(); 
	// 		double m_waveformTime = m_pulseHitTime - m_trigTime + 100;
	// 	}
	// }
}
