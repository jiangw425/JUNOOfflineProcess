#!/bin/bash
mytop=MYTOP
function detsim_script() {
    echo $mytop/offline/Examples/Tutorial/share/tut_detsim.py
}
function calib_script() {
    echo $mytop/offline/Examples/Tutorial/share/tut_det2calib.py
}
function gen_job_name() {
    local prefix=$1; shift
    local suffix=$1; shift
    echo ${prefix}.${suffix}
}

function gen_name() {
    local prefix=$1; shift
    local seed=$1; shift
    local suffix=$1; shift
    echo ${prefix}-${seed}.${suffix}
}

function create_job(){
GEN_NAME=$(gen_name sub $1 $2 condor)
SH_NAME=$(gen_name run $1 $2 sh)
if [ -f $GEN_NAME ]; then
    rm $GEN_NAME
fi

cat > $GEN_NAME <<EOF
Universe = vanilla
Executable = $SH_NAME
Accounting_Group = juno
getenv = True
Queue
EOF
}

function create_gen(){
SH_NAME=$(gen_name run $1 sh)
simLOG_NAME=$(gen_name simlog $1 txt)
simOUTPUT=$(gen_name sim $1 root)
simUSER_OUTPUT=$(gen_name user-sim $1 root)

calibLOG_NAME=$(gen_name caliblog $1 txt)
calibOUTPUT=$(gen_name calib $1 root)
calibUSER_OUTPUT=$(gen_name user-calib $1 root)

#data_source_dir=root://junoeos01.ihep.ac.cn//eos/juno/valprod/valprod0/J21v1r0-Pre0/ACU-CLS/
data_source_dir=ACUCLSPATH

if [ -f $SH_NAME ]; then
    rm $SH_NAME
fi

cat>$SH_NAME <<EOF
#!/bin/bash
cd $(pwd)
source $mytop/bashrc
export EOS_MGM_URL="root://junoeos01.ihep.ac.cn"
export EOS_HOME="/eos/juno/valprod/valprod0/"

(time python ../tut_calib2rec.py --JOBA $1 --JOBB $[$1+1] --PMTtype 1 --SignalWindow 280 --RfrIndxLS 1.54 --RfrIndxWR 1.355 --calibdir ${data_source_dir} --usercalibdir ${data_source_dir} --paradir PARADIR --calibsource NAMESOURCE --startseed 0 --filenum FILENUM) >& $calibLOG_NAME
EOF

chmod +x $SH_NAME
}

TYPE=${1:-condor}; shift
Momentum=${1:-0}; shift
JOB_START=0
N=293
seed=0

for ((i=0; i< $N; i++))
do
    create_gen  $i 
    if [ ! -d pos$i ]; then
        mkdir -p pos$i
    fi
    SH_NAME=$(gen_name run $i sh)
    LOG_NAME=$(gen_name run $i log)
    ERR_NAME=$(gen_name run $i err)
    #hep_sub $SH_NAME -g juno -o $LOG_NAME -e $ERR_NAME
    #nohup ./$SH_NAME & 
done
hep_sub run-"%{ProcId}".sh -n 293 -e /dev/null -o /dev/null

