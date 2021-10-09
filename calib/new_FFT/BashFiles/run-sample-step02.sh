#!/bin/bash
source LOCALENV
pushd MAIND

(time python $WORKTOP/offline/Calibration/PMTCalib/share/run.py --evtmax -1 FORCETRIGGEROPT --input INPUTFILE --step 2 --calibfile Ge68/step01/user-root/Integral.txt --output PARA/step02/root/SPE_step02_RUN.root) >& PARA/step02/log/SPE-step02-log-RUN.txt

popd
