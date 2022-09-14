// TCanvas* tc = new TCanvas("tc", "tc", 800, 600);
// compare(tc->cd(), chi2ndfth, 3, fname, 1);
void compare(TVirtualPad* c, TList* list, int length, TString lname[], int logy)
{

  TH1D* hist[length];
  for (int i = 0; i < length; i++) {
    hist[i] = (TH1D*)list->At(i);
  }
  double max = -1e10;
  double min = 1e10;
  //int color[8] = {13, 4, 2, 6, 8, 41, 28, 7};
  int color[8] = {3, 1, 4, 2, 8, 41, 28, 7};

  TLegend* l = new TLegend(0.6, 0.6, 0.9, 0.9);
  if (length >= 6) l->SetNColumns(2);
  l->SetFillStyle(0);
  for (int i = 0; i < length; i++) {
    max = max > hist[i]->GetMaximum() ? max : hist[i]->GetMaximum();
    min = min > hist[i]->GetMinimum() ? hist[i]->GetMinimum() : min;
  }

  // find min but not 0
  double min0 = max;
  if (logy && min >= 0) {
    for (int i = 0; i < length; i++) {
      for (int j = 0; j < hist[i]->GetNbinsX(); j++) {
        double tmp = hist[i]->GetBinContent(j + 1);
        min0 = (tmp < min0 && tmp > 0) ? tmp : min0;
      }
    }
  }

  double maxedge = max + (max - min) / 10.;
  double minedge = min - (max - min) / 10.;
  minedge = (min >= 0 && minedge < 0) ? 0 : minedge;

  if (logy && min >= 0) {
    c->SetLogy();
  }
  for (int i = 0; i < length; i++) {
    // c->cd();
    if (logy && min >= 0)
      hist[i]->GetYaxis()->SetRangeUser(min0 * 0.5, max * 2);
    else
      hist[i]->GetYaxis()->SetRangeUser(minedge, maxedge);
    hist[i]->SetLineColor(color[i]);
	hist[i]->SetLineWidth(2);
    hist[i]->SetMarkerColor(color[i]);
    if (i == 0)
      hist[i]->Draw("hist");
    else
      hist[i]->Draw("same hist");
    l->AddEntry(hist[i], lname[i], "ple");
  }
  l->Draw();
}

void compare(TVirtualPad* c, TList* list, int length, TString lname[], int logy, TString opt)
{

  TH1D* hist[length];
  for (int i = 0; i < length; i++) {
    hist[i] = (TH1D*)list->At(i);
  }
  double max = -1e10;
  double min = 1e10;
  int color[8] = {13, 2, 4, 6, 8, 41, 28, 7};

  TLegend* l = new TLegend(0.6, 0.6, 0.9, 0.9);
  l->SetFillStyle(0);
  for (int i = 0; i < length; i++) {
    max = max > hist[i]->GetMaximum() ? max : hist[i]->GetMaximum();
    min = min > hist[i]->GetMinimum() ? hist[i]->GetMinimum() : min;
  }

  // find min but not 0
  double min0 = max;
  if (logy && min >= 0) {
    for (int i = 0; i < length; i++) {
      for (int j = 0; j < hist[i]->GetNbinsX(); j++) {
        double tmp = hist[i]->GetBinContent(j + 1);
        min0 = (tmp < min0 && tmp > 0) ? tmp : min0;
      }
    }
  }

  double maxedge = max + (max - min) / 10.;
  double minedge = min - (max - min) / 10.;
  minedge = (min >= 0 && minedge < 0) ? 0 : minedge;

  if (logy && min >= 0) {
    c->SetLogy();
  }
  for (int i = 0; i < length; i++) {
    // c->cd();
    if (logy && min >= 0)
      hist[i]->GetYaxis()->SetRangeUser(min0 * 0.5, max * 2);
    else
      hist[i]->GetYaxis()->SetRangeUser(minedge, maxedge);
    hist[i]->SetLineColor(color[i]);
    hist[i]->SetMarkerColor(color[i]);
    if (i == 0)
      hist[i]->Draw(opt);
    else
      hist[i]->Draw("same" + opt);
    l->AddEntry(hist[i], lname[i], "ple");
  }
  l->Draw();
}
