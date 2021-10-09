#include "/junofs/users/jiangw/include/pmtflag.h"
void drawGain()
{
    bool *hmmt = pmtflag();
    double tmp;
    int n = 17612;
    double deconvGain[n], integral[n];

    ifstream ijw("PmtPrtData_deconv.txt");
    for(int i=0; i<n; ++i) ijw>>tmp>>tmp>>deconvGain[i]>>tmp>>tmp>>tmp;
    ijw.close();
    // cout << deconvGain[0] << endl;
    // cout << deconvGain[1] << endl;
    // cout << deconvGain[2] << endl;

    ijw.open("parameterForGain.txt");
    for(int i=0; i<n; ++i) ijw>>tmp>>integral[i];
    ijw.close();
    // cout << integral[0] << endl;

    TH2D *Gain = new TH2D("Gain","Deconv adc Gain of hmmt",n,0,n,100,7e6,12e6);
    double rate = 75 / 0.55 / 50 / 1.602 * 1e4;
    for(int i=0; i<n; ++i) if(!hmmt[i]) Gain->Fill(i,deconvGain[i]*integral[i]*rate);
    Gain->Draw();
}
