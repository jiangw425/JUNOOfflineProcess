#!/bin/bash
source LOCALENV
pushd MAIND

(time python $WORKTOP/offline/Calibration/PMTCalib/share/run.py --evtmax -1 FORCETRIGGEROPT --input INPUTFILE --step 1 --output PARA/step01/root/SPE_step01_RUN.root) >& PARA/step01/log/SPE-log-step01-RUN.txt

popd
