#include "Event/ElecHeader.h"
#include "Event/ElecTruthHeader.h"
#include "Context/TimeStamp.h"
void readTruthHitTime(){
	// std::string path = "root://junoeos01.ihep.ac.cn//eos/juno/user/jiangw/J21v1r0-Pre0/ForceTrigger/Cs137/elecsim/root/elecsim-*.root";
	std::string path = "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/11/C14/C14_Uniform/elecsim/root";
	// std::string path = "/scratchfs/juno/jiangw/Production/11";
	// std::string path = "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/ACU-CLS/Ge68/Ge68_0_0_0/elecsim/root";
	// std::string path = "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/ACU-CLS/AmC/AmC_0_0_0/elecsim/root";
	// std::string path = "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/ACU-CLS/Laser0.1/Laser0.1_0_0_0/elecsim/root";
	// std::string path = "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/ACU-CLS/Laser0.05/Laser0.05_0_0_0/elecsim/root";
	// std::string path = "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/ACU/Co60/Co60_0_0_0/elecsim/root";
	// std::string path = "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/ACU/Cs137/Cs137_0_0_0/elecsim/root";
	// std::string path = "/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/11/e+/e+_Uniform/0MeV/elecsim/root";
	const int length = 1250;
	string sname[4] = {"Cs137","Co60","Laser0.1","Laser0.05"};
	TH1D *hitTime[4];
	int lineColor[4] = {2,4,6,8};//红紫绿蓝
	TH1D *all_hitTime_hist = new TH1D("all_hitTime_hist","all_hitTime_hist",length+100,-50,length+50);

	for(int s=0; s<1; ++s){
		string name = sname[s] + "_hitTime";
        hitTime[s] = new TH1D(name.c_str(),name.c_str(),length+100,-50,length+50);
        hitTime[s]->SetLineColor(lineColor[s]);

		TChain *tTruth = new TChain("Event/Sim/Truth/LpmtElecTruthEvent");
		TChain *tElec = new TChain("Event/Elec/ElecEvent");
		//TString spath = Form("%s/elecsim-*.root",path.c_str());
		TString spath = Form("%s/elecsim-0.root",path.c_str());
		tElec->Add(spath);
		tTruth->Add(spath);
		JM::ElecEvent* ee = new JM::ElecEvent();
		JM::LpmtElecTruthEvent* et = new JM::LpmtElecTruthEvent();
		tElec->SetBranchAddress("ElecEvent", &ee);
		tTruth->SetBranchAddress("LpmtElecTruthEvent", &et);
		cout << "Total Elec Events: " << tElec->GetEntries() << endl;
		cout << "Total ElecTruth Events: " << tTruth->GetEntries() << endl;
		if (tElec->GetEntries() != tTruth->GetEntries()) cout << "Wrong input data, please check!" << endl;

		// for (int i = 0; i < tTruth->GetEntries(); i++) {
		for (int i = 0; i < 1000; i++) {
			if(i%1000==0) cout<<"Processing "<< i << endl;
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
				// hitTime[s]->Fill(m_waveformTime);
				all_hitTime_hist->Fill(m_waveformTime);
			}
		}

		tTruth->~TChain();
		tElec->~TChain();
	}
	TCanvas *c = new TCanvas("c","c",1600,900);
    // c->Divide(2,2);
    all_hitTime_hist->Draw();
	c->Print("tmp.png");
    // for(int s=0;s<1;++s){
    //     // nPMT_hist[s]->Scale(1./nPMT_hist[s]->Integral());
    //     hitTime[s]->Draw("SAME HIST");
    // }


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
