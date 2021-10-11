void merge()
{
	TH1D* h = new TH1D("h","DN",50,0,0.2);
  int i;
  double mu[17612];
  double gain[17612];
  double dr[17612];
  double tmp;
  double tfs[17612];
  ifstream file;
  // dr
  file.open("CalibPars_DN.txt");
  while (file >> i >> dr[i])
    if (i == 17612) break;
  file.close();
  // mu
  file.open("CalibPars_dn.txt");
  while (file >> i >> mu[i] >> tmp >> tmp)
    if (i == 17612) break;
  file.close();
  // gain
  file.open("CalibPars.txt");
  while (file >> i >> tmp >> gain[i] >> tfs[i] >> tmp >> tmp)
    if (i == 17612) break;
  file.close();
  // mean
  TFile* f = new TFile("../SCSSN/step12/user-root/user_calibCorr.root", "read");
  TH1F* spe;
  double spem[17612];
  for (int i = 0; i < 17612; i++) {
    TString name = Form("ch%d_charge_spec", i);
    spe = (TH1F*)f->Get(name);
    spem[i] = spe->GetMean();
  }

  ofstream ofile;
  ofile.open("all_CalibPars.txt");
  for (int i = 0; i < 17612; i++) {
    ofile << i << "\t" << mu[i] * 0.1 << "\t" << gain[i] << "\t" << tfs[i] << "\t" << dr[i] << "\t" << spem[i] << endl;
  }
  ofile.close();

	for(int i=0; i<17612; ++i) h->Fill(mu[i]*0.1);
	h->Draw();
}
