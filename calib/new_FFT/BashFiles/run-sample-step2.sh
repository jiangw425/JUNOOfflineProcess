#!/bin/bash
source LOCALENV
pushd MAIND
hostname
(time python $WORKTOP/offline/Calibration/PMTCalibAlg/share/run.py --evtmax -1 --SPE 1 FORCETRIGGEROPT --input INPUTFILE --output PARA/step2/root/calibCorr_RUN.root --user-output PARA/step2/user-root/user_calibCorr_RUN.root) >& PARA/step2/log/calibCorr-log-RUN.txt
popd
