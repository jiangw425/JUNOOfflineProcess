// ---------------------------------------------------
// NIMmodel.h
//
// Used to fit adc distributions.
//
// Author: J. Pedro Ochoa, adapted from Qing He's Calibration/RollingGain
// package
//
// March 19 2011
// ---------------------------------------------------

#include <iostream>
#include <math.h>
#include <TSystem.h>
#include <TMath.h>
int nmin = 1;
int nmax = 3;

double poisson(double u, int n)
{
  return pow(u, n) * exp(-u) / TMath::Gamma(n + 1);
}

double gn(double x, double n, double q0, double sigma0, double q1, double sigma1)
{
  double sigman = sqrt(sigma0 * sigma0 + n * sigma1 * sigma1);
  double xn = x - q0 - n * q1;
  double tmp = pow(xn, 2) / (2 * sigman * sigman);
  return exp(-tmp) / (sigman * sqrt(2 * TMath::Pi()));
}

double NIMmodel(double* xpar, double* par)
{
  double x = xpar[0];
  double norm = par[0];
  double q0 = par[1];
  double sigma0 = par[2];
  double q1 = par[3];
  double sigma1 = par[4];
  double w = par[5];
  double a = par[6];
  double u = par[7];

  double total = 0;
  for (int i = nmin; i < nmax; i++) {
    int n = i;
    double tmp = poisson(u, n);
    double tmp2 = gn(x, n, q0, sigma0, q1, sigma1);
    double qn = q0 + n * q1;
    double sigman = sqrt(sigma0 * sigma0 + n * sigma1 * sigma1);

    double tmp3 = exp(-a * (x - qn - a * sigman * sigman / 2.)) * a / 2.;
    double tmp4 = TMath::Erf(fabs(q0 - qn - a * sigman * sigman) / (sigman * sqrt(2.)));
    double sign = 1.0;
    if ((x - qn - sigman * sigman * a) < 0) sign = -1.0;
    tmp4 += sign * TMath::Erf(fabs(x - qn - sigman * sigman * a) / (sigman * sqrt(2.)));
    tmp3 *= tmp4;

    double result = tmp * ((1 - w) * tmp2 + w * tmp3);
    total += result;
  }
  return total * norm;
}
