#!/bin/bash
source LOCALENV
pushd MAIND
hostname
(time python $WORKTOP/offline/Calibration/PMTCalibAlg/share/run.py --evtmax -1 --SPE 0 FORCETRIGGEROPT --input INPUTFILE --output PARA/step12/root/calibCorr_RUN.root --user-output PARA/step12/user-root/user_calibCorr_RUN.root) >& PARA/step12/log/calibCorr-log-RUN.txt
popd
