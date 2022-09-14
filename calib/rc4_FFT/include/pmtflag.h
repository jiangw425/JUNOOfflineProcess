// usage:
// bool* hmmtpmt = pmtflag();
// int* hmmtpmt = highqeflag();
bool* pmtflag()
{
  int const f_Npmt = 17612;
  static bool hmmtpmt[f_Npmt];
  // get hmmt pmt id
  std::string s;
  assert(("No JUNO environment!", getenv("JUNOTOP")));
   s = getenv("JUNOTOP");
  // //  s += "/data/Simulation/ElecSim/Hamamatsu_pmtID.txt";
    s += "/data/Detector/Geometry/PMTType_CD_LPMT.csv";
    cout << s << endl;
//s = "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v2r0-Pre0/data/Detector/Geometry/PMTType_CD_LPMT.csv";
  // s="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc0/data/Detector/Geometry/PMTType_CD_LPMT.csv";
  ifstream sfile;
  sfile.open(s.c_str(), ios::in);
  for (int i = 0; i < f_Npmt; i++) hmmtpmt[i] = false;
  int tmpid;
  std::string tmpstring;
  for(int i=0;i<f_Npmt;i++){
      sfile >> tmpid >> tmpstring;
      if(tmpstring=="Hamamatsu") hmmtpmt[i] = true;
  }
  // int tmpflag;
  // while (sfile >> tmpflag) hmmtpmt[tmpflag] = true;
  sfile.close();
  return hmmtpmt;
}

int* highqeflag()
{
  int const f_Npmt = 17612;
  static int highqepmt[f_Npmt];
  std::string s;
  if (getenv("JUNOTOP")) {
    s = getenv("JUNOTOP");
    s += "/data/Simulation/ElecSim/pmtdata.txt";
  } else {
    cout << "No JUNO environment!" << endl;
  }
  ifstream sfile;
  sfile.open(s.c_str(), ios::in);
  for (int i = 0; i < f_Npmt; i++) highqepmt[i] = 0;
  int tmpflag;
  double tmp;
  TString name;
  while (sfile >> tmpflag >> name >> tmp)
    if (name == "HighQENNVT")
      highqepmt[tmpflag] = 2;
    else if (name == "NNVT")
      highqepmt[tmpflag] = 1;
  sfile.close();
  return highqepmt;
}
