#!/bin/bash
mkdir recMap
cd recMap
mkdir CalibPMTPara  ChargeSpec  TimePdf  nPEMap
cp ../share/Ge68/GridMu_RealAdd/LnPEMapFile.root ./nPEMap/
cp ../share/Laser0.05/GridMu_RealAdd/LnPEMapFile_Ek.root ./nPEMap/
cp ../share/timePDF/TimePdfFile.root ./TimePdf
cp ../GenQPDF/share/J20v2r0-Pre2_deconv/GenGrPDF/AvgNPEQpdf.root ./ChargeSpec

