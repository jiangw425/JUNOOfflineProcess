void tt_angular_MCP()
{
    double tt_angle[9] = {0, 14, 30, 42.5, 55, 67, 77.5, 85, 90};
  double tt_ratio[9] = {0., 0., 0.2142, 4.5757, 6.2484, 9.1953, 8.9885, 7.87506618, 7.1328014};
  TGraph *gTT_MCP = new TGraph(9, tt_angle, tt_ratio);
  gTT_MCP->Draw();
}