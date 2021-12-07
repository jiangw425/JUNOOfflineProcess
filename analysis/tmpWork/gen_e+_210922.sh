#!/bin/bash
posfile=xyz.txt
path0=`pwd`
en=(0)
evt1pos=1
evt1file=1
filenum=$((evt1pos/evt1file))

write_num=-1
posnum=-1
while read lines
do
    psx=`echo $lines | cut -d' ' -f1`
	psy=`echo $lines | cut -d' ' -f2`
	psz=`echo $lines | cut -d' ' -f3`
    pos=${psx}_${psy}_${psz}
    echo "$pos $posnum"
    let ++posnum

    if [[ $posnum%200 -eq 0 ]];then
        let ++write_num
        fname="run/e+_0MeV_fwx_${write_num}.sh"
        echo "#!/bin/bash" > $fname
        echo "source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre2/setup.sh" >> $fname
        echo "cd $path0" >> $fname
        chmod +x $fname
    fi  

    echo "time python \$TUTORIALROOT/share/tut_detsim.py --evtmax $evt1file --seed $posnum --output root/detsim-${pos}.root --user-output user-root/user-detsim-${pos}.root --no-gdml gun --particles e+ --position ${psx} ${psy} ${psz} --momentums 0 --momentums-interp KineticEnergy" >> $fname
done < $posfile

cd run
bash ~jiangw/hepNsub.sh -1 -1
