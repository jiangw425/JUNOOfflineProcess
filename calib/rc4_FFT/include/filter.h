TH1D* filter(double par1, double par2, double par3)
{
  std::vector<double> f_filter;
  double f_para1 = par1;
  double f_para2 = par2;
  double f_para3 = par3;
  int f_length = 1250;
  TH1D* f_filterh = new TH1D("f_filterh", "f_filterh", f_length, 0, f_length);
  f_filter.clear();
  for (int i = 0; i < f_length; i++) {
    if (i < f_para1)
      f_filter.push_back(1);
    else if (i < f_para2)
      f_filter.push_back(exp(-0.5 * ((i - f_para1) / f_para3) * ((i - f_para1) / f_para3)));
    else
      f_filter.push_back(0);
    f_filterh->SetBinContent(i + 1, f_filter.at(i));
  }
  return f_filterh;
}
