// calculate statistic weighted mean
pair<double, double> weightedMean(vector<double> x, vector<double> e)
{
  // check if it is a good input
  if (x.size() != e.size()) {
    cout << "Error: Wrong input!" << endl;
    return make_pair(0, 0);
  }
  // check if it is 0 error
  for (int i = 0; i < e.size(); i++) {
    if (e.at(i) == 0) {
      cout << "Error: Errors 0!" << endl;
      return make_pair(0, 0);
    }
  }

  int n = x.size();
  double b = 0;
  double c = 0;
  for (int i = 0; i < n; i++) {
    b += x.at(i) / (e.at(i) * e.at(i));
    c += 1. / (e.at(i) * e.at(i));
  }

  return make_pair(b / c, sqrt(1. / c));
}

double plusError(double x1, double x2, double e1, double e2, int plus = 1, double co = 0.)
{
  return sqrt(e1 * e1 + e2 * e2 + 2. * e1 * e2 * co * plus);
}

double multiError(double x1, double x2, double e1, double e2, int multi = 1, double co = 0.)
{
  return sqrt(x1 * x2 * x1 * x2 * (e1 * e1 / x1 / x1 + e2 * e2 / x2 / x2 + 2. * co / x1 / x2 * multi));
}
