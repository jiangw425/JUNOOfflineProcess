#!/bin/bash
path0=`pwd`
for e in {0..10}
do
    cd ${e}MeV/calib/run
    for n in {0..99}
    do
        sed -e "s#ENERGY#${e}#g" -e "s#NUM#${n}#g" $path0/sample_calib.sh > e+_${e}MeV_calib_${n}.sh
    done
    chmod +x *.sh
    cd $path0
done