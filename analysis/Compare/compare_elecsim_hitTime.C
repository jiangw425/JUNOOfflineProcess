#include "Event/ElecHeader.h"
#include "Event/ElecTruthHeader.h"
#include "Context/TimeStamp.h"
void compare_elecsim_hitTime(){
	const int num_file = 2;
    const std::string sname = "SNAME";
    std::string lable[num_file] = {"old","new"};
    std::string path[num_file] = {
        "OLD",
        "NEW",
    };

	const int length = 1250;
	int color[num_file] = {1,2};
	TH1D *hitTime[num_file];

	for(int fn=0;fn<num_file;++fn){
		TChain *tTruth = new TChain("Event/Sim/Truth/LpmtElecTruthEvent");
		TChain *tElec = new TChain("Event/Elec/ElecEvent");
		TString spath = Form("%s/*.root",path[fn].c_str());
		tElec->Add(spath);
		tTruth->Add(spath);
		JM::ElecEvent* ee = new JM::ElecEvent();
		JM::LpmtElecTruthEvent* et = new JM::LpmtElecTruthEvent();
		tElec->SetBranchAddress("ElecEvent", &ee);
		tTruth->SetBranchAddress("LpmtElecTruthEvent", &et);
		int totalEntries = tElec->GetEntries();
		cout << "Total Elec Events: " << tElec->GetEntries() << endl;
		cout << "Total ElecTruth Events: " << tTruth->GetEntries() << endl;
		if (tElec->GetEntries() != tTruth->GetEntries()) cout << "Wrong input data, please check!" << endl;

		TString tmpname = Form("%s_%s_elecsim_hitTime",lable[fn].c_str(),sname.c_str());
        hitTime[fn] = new TH1D(tmpname,tmpname,length+100,-50,length+50);
        hitTime[fn]->SetLineColor(color[fn]);

		int tmpentries = 2000 < totalEntries ? 2000 : totalEntries;
		// for (int i = 0; i < tTruth->GetEntries(); i++) {
		for (int i = 0; i < tmpentries; i++) {
            if(i%100==0) cout << "Processing " << i << "/" << tmpentries << endl;
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
				hitTime[fn]->Fill(m_waveformTime);
				// all_hitTime_hist->Fill(m_waveformTime);
			}
		}
		tTruth->~TChain();
		tElec->~TChain();
	}

	TCanvas *cc = new TCanvas("c","c",1600,900);
    cc->cd();
    for(int fn=0;fn<num_file;fn++) hitTime[fn]->Draw("SAME");
    cc->Print(Form("%s_elecsim_hitTime_diff.png",sname.c_str()));

	// TCanvas *c = new TCanvas("c","c",1920,1080);
    // c->Divide(2,2);
    // all_hitTime_hist->Draw();
	// c->Print("tmp.png");
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
