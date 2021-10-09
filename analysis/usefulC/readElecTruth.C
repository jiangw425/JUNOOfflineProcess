#include "Event/ElecHeader.h"
#include "Event/ElecTruthHeader.h"
#include "Context/TimeStamp.h"

void temp(){
	
	TFile* f = new TFile("/junofs/users/huangx/production/J20v1r0-Pre0/e+/uniform/e+_4/elecsim/elecsim-20400.root", "read");
	TTree* tElec = (TTree*)f->Get("Event/Elec/ElecEvent");
	TTree* tTruth = (TTree*)f->Get("Event/Sim/Truth/LpmtElecTruthEvent");
	JM::ElecEvent* ee = new JM::ElecEvent();
	JM::LpmtElecTruthEvent* et = new JM::LpmtElecTruthEvent();

	tElec->SetBranchAddress("ElecEvent", &ee);
	tTruth->SetBranchAddress("LpmtElecTruthEvent", &et);
	cout << "Total Elec Events: " << tElec->GetEntries() << endl;
	cout << "Total ElecTruth Events: " << tTruth->GetEntries() << endl;
	if (tElec->GetEntries() != tTruth->GetEntries()) cout << "Wrong input data, please check!" << endl;

	for (int i = 0; i < 1; i++) {
		cout << "evtID=" << i << endl;
		tElec->GetEntry(i);
		tTruth->GetEntry(i);

		const JM::ElecFeeCrate& efc = ee->elecFeeCrate();
		JM::ElecFeeCrate* m_crate = const_cast<JM::ElecFeeCrate*>(&efc);
		const std::vector<JM::LpmtElecTruth>& let = et->truths();
		std::vector<JM::LpmtElecTruth>* m_truths;
		m_truths = const_cast<std::vector<JM::LpmtElecTruth>*>(&let);
		TimeStamp triggerTime = m_crate->TriggerTime();
		TimeStamp evtTime     = m_crate->EvtTimeStamp();
		double m_trigTime = triggerTime.GetNanoSec();
		double m_evtTime  = evtTime.GetNanoSec();
		cout << m_truths->size() << endl;
		for (int j = 0; j < m_truths->size(); j++) {
			std::string m_pulsetype = m_truths->at(j).pulsetype();     
			int m_pmtId = m_truths->at(j).pmtId();                     
			int m_npe = m_truths->at(j).npe();                         
			double m_hitTime = m_truths->at(j).hitTime();              
			double m_amplitude = m_truths->at(j).amplitude();          
			double m_TTS = m_truths->at(j).tts();                      
			double m_timeoffset = m_truths->at(j).timeoffset();        
			double m_pulseHitTime = m_truths->at(j).pulseHitTime().GetNanoSec(); 
			double m_waveformTime = m_pulseHitTime - m_trigTime + 100;
		}
	}
}
