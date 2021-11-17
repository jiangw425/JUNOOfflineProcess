step=(01 02 03 1 2 11 12)
echo 'Making directories...'
for i in ${step[@]}
do
    if [[ ${i:0:1} -eq 0 ]];then
        sname=(Ge68)
    elif [[ $i -lt 10 ]];then
        sname=(AmC Ge68 Co60 Cs137 Laser0.1 Laser0.05)
    else
        sname=(C14 Laser0.1 Laser0.05)
    fi
    mkdir -p tmpworkout/step$i
    for n in ${sname[@]}
    do
        mkdir -p $n/step$i/run
        mkdir -p $n/step$i/log
        mkdir -p $n/step$i/root
        mkdir -p $n/step$i/user-root
    done
done
mkdir -p Parameters
mkdir -p CheckStation
echo 'Directories created.'
