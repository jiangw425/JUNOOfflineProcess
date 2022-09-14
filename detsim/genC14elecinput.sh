#!/bin/bash

eosTop="root://junoeos01.ihep.ac.cn/"
eosPath="/eos/juno/valprod/valprod0/J22.1.0-rc4/C14/C14_Uniform/detsim/root/detsim-"
jobnum=40
ratio=40


for((n=0;n<$jobnum;n++))
do
    filename="C14_${n}.txt"
    for((i=0;i<$ratio;i++))
    do
        echo "${eosTop}/${eosPath}$((ratio*n+i)).root" >> $filename
    done
done