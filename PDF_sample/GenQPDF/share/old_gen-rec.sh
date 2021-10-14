#!/bin/bash
path0=`pwd`
mytop=MYTOP
source $mytop/bashrc

function rec_script() {
    echo Genqpdf.py
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

recLOG_NAME=$(gen_name qpdflog $1 txt)
recOUTPUT=$(gen_name qpdf $1 root)
recUSER_OUTPUT=$(gen_name user-qpdf $1 root)

if [ -f $SH_NAME ]; then
    rm $SH_NAME
fi

cat>$SH_NAME <<EOF
#!/bin/bash
cd ${path0}
source $mytop/bashrc
(time python $(rec_script) --JOBA $2 --JOBB $[$2+20] --enableUserSPEs --runmode 2 --peRange 100 --userSPEFile ${mytop}/data/Calibration/PMTCalibSvc/data/output_deconv.root  --outdir ./ ) >&  $recLOG_NAME
EOF

chmod +x $SH_NAME
}


TYPE=${1:-condor}; shift
Momentum=${1:-0}; shift
SEED_START=0
N=881
seed=0

for ((i=0; i< $N; i++))
do
    create_gen $i $[$SEED_START+$seed]
    SH_NAME=$(gen_name run $i sh)
    LOG_NAME=$(gen_name run $i log)
    ERR_NAME=$(gen_name run $i err)
    #hep_sub $SH_NAME -g juno -o $LOG_NAME -e $ERR_NAME
    ((seed += 20))
done
hep_sub run-"%{ProcId}".sh -n 881 -e /dev/null -o /dev/null
#hep_sub run_parent.sh -argu %{ProcId} -n 881
