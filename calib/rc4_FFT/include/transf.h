class wavedec {

  public:
  bool set_input(TH1D* h1, TH1D* h2, TH1D* h3, TH1D* h4);
  bool set_rawh(TH1D* h);
  bool set_filter(TH1D* h);
  bool set_spere(TH1D* h);
  bool set_speim(TH1D* h);

  public:
  bool transform();
  TH1D* get_rawfre();
  TH1D* get_rawfim();
  TH1D* get_rawfmag();
  TH1D* get_filteredfre();
  TH1D* get_filteredfim();
  TH1D* get_filteredfmag();
  TH1D* get_filteredt();
  TH1D* get_dividedfre();
  TH1D* get_dividedfim();
  TH1D* get_dividedfmag();
  TH1D* get_dividedt();

  public:
  TH1D* f2t(double* re_full, double* im_full);

  private:
  TH1D* rawh;
  TH1D* filter;
  TH1D* spere;
  TH1D* speim;

  private:
  TH1D* rawfre = new TH1D("rawfre", "rawfre", 600, 0, 600);
  TH1D* rawfim = new TH1D("rawfim", "rawfim", 600, 0, 600);
  TH1D* rawfmag = new TH1D("rawfmag", "rawfmag", 600, 0, 600);
  TH1D* filteredfre = new TH1D("filteredfre", "filteredfre", 600, 0, 600);
  TH1D* filteredfim = new TH1D("filteredfim", "filteredfim", 600, 0, 600);
  TH1D* filteredfmag = new TH1D("filteredfmag", "filteredfmag", 600, 0, 600);
  TH1D* filteredt;
  TH1D* dividedfre = new TH1D("dividedfre", "dividedfre", 600, 0, 600);
  TH1D* dividedfim = new TH1D("dividedfim", "dividedfim", 600, 0, 600);
  TH1D* dividedfmag = new TH1D("dividedfmag", "dividedfmag", 600, 0, 600);
  TH1D* dividedt;

  private:
  int f_length;
};

bool wavedec::transform()
{
  if (!rawh) {
    cout << "No raw input! Please check!" << endl;
    return true;
  }
  f_length = rawh->GetNbinsX();
  // cout << "Total length: " << f_length << endl;
  TH1* f_tempH;
  std::vector<double> re_full_vec(f_length);
  std::vector<double> im_full_vec(f_length);
  double* re_full = &re_full_vec.front();
  double* im_full = &im_full_vec.front();
  delete TVirtualFFT::GetCurrentTransform();
  TVirtualFFT::SetTransform(0);
  f_tempH = rawh->FFT(NULL, "MAG");
  for (int i = 0; i < f_length; i++) {
    re_full[i] = 0;
    im_full[i] = 0;
  }
  TVirtualFFT* fft = TVirtualFFT::GetCurrentTransform();
  fft->GetPointsComplex(re_full, im_full);
  f_tempH->Delete();
  f_tempH = NULL;
  for (int i = 0; i < 600 && i < f_length; i++) {
    rawfre->SetBinContent(i + 1, re_full[i]);
    rawfim->SetBinContent(i + 1, im_full[i]);
    rawfmag->SetBinContent(i + 1, sqrt(re_full[i] * re_full[i] + im_full[i] * im_full[i]));
  }

  if (!filter) {
    cout << "No filter input! Set a default \"1\" filter!" << endl;
  }
  for (int i = 0; i < 600 && i < f_length; i++) {
    if (filter) {
      re_full[i] *= filter->GetBinContent(i + 1);
      im_full[i] *= filter->GetBinContent(i + 1);
    }
    filteredfre->SetBinContent(i + 1, re_full[i]);
    filteredfim->SetBinContent(i + 1, im_full[i]);
    filteredfmag->SetBinContent(i + 1, sqrt(re_full[i] * re_full[i] + im_full[i] * im_full[i]));
  }
  if (filteredt) {
    filteredt->Delete();
    filteredt = NULL;
  }
  filteredt = f2t(re_full, im_full);
  filteredt->SetNameTitle("filteredt", "filteredt");

  if (!spere || !speim) {
    cout << "No SPE input! Will be no final output!" << endl;
    return true;
  }
  for (int i = 0; i < spere->GetNbinsX(); i++) {
    if (spere->GetBinContent(i + 1) != 0 || speim->GetBinContent(i + 1) != 0) {
      double a11 = re_full[i], b11 = im_full[i], c11 = spere->GetBinContent(i + 1), d11 = speim->GetBinContent(i + 1);
      double f11 = c11 * c11 + d11 * d11;
      re_full[i] = (a11 * c11 + b11 * d11) / f11;
      im_full[i] = (b11 * c11 - a11 * d11) / f11;
    } else {
      re_full[i] = 0;
      im_full[i] = 0;
    }
  }
  for (int i = spere->GetNbinsX(); i < f_length; i++) {
    re_full[i] = 0;
    im_full[i] = 0;
  }
  for (int i = 0; i < 600 && i < f_length; i++) {
    dividedfre->SetBinContent(i + 1, re_full[i]);
    dividedfim->SetBinContent(i + 1, im_full[i]);
    dividedfmag->SetBinContent(i + 1, sqrt(re_full[i] * re_full[i] + im_full[i] * im_full[i]));
  }
  if (dividedt) {
    dividedt->Delete();
    dividedt = NULL;
  }
  dividedt = f2t(re_full, im_full);
  dividedt->SetNameTitle("dividedt", "dividedt");
  return true;
}

TH1D* wavedec::f2t(double* re_full, double* im_full)
{
  TH1* tmph = NULL;
  double re_tmp[f_length];
  double im_tmp[f_length];
  for (int i = 0; i < f_length; i++) {
    re_tmp[i] = re_full[i];
    im_tmp[i] = im_full[i];
  }
  re_tmp[0] = 0;
  im_tmp[0] = 0;
  TVirtualFFT* fft_back = TVirtualFFT::FFT(1, &f_length, "C2R M K");
  fft_back->SetPointsComplex(re_tmp, im_tmp);
  fft_back->Transform();
  tmph = TH1::TransformHisto(fft_back, tmph, "Re");
  tmph->Scale(1. / f_length);
  fft_back->Delete();
  fft_back = NULL;
  return (TH1D*)tmph;
}

bool wavedec::set_input(TH1D* h1, TH1D* h2, TH1D* h3, TH1D* h4)
{
  rawh = h1;
  filter = h2;
  spere = h3;
  speim = h4;
  return true;
}

bool wavedec::set_rawh(TH1D* h)
{
  rawh = h;
  return true;
}
bool wavedec::set_filter(TH1D* h)
{
  filter = h;
  return true;
}
bool wavedec::set_spere(TH1D* h)
{
  spere = h;
  return true;
}
bool wavedec::set_speim(TH1D* h)
{
  speim = h;
  return true;
}

TH1D* wavedec::get_rawfre()
{
  return rawfre;
}
TH1D* wavedec::get_rawfim()
{
  return rawfim;
}
TH1D* wavedec::get_rawfmag()
{
  return rawfmag;
}
TH1D* wavedec::get_filteredfre()
{
  return filteredfre;
}
TH1D* wavedec::get_filteredfim()
{
  return filteredfim;
}
TH1D* wavedec::get_filteredfmag()
{
  return filteredfmag;
}
TH1D* wavedec::get_filteredt()
{
  return filteredt;
}
TH1D* wavedec::get_dividedfre()
{
  return dividedfre;
}
TH1D* wavedec::get_dividedfim()
{
  return dividedfim;
}
TH1D* wavedec::get_dividedfmag()
{
  return dividedfmag;
}
TH1D* wavedec::get_dividedt()
{
  return dividedt;
}
