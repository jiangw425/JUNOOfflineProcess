#!/bin/bash
echo "In order to save storage space, root and user-root files are removed."
echo "Please make sure you have checked there is nothing wrong, or you need to generate them again."
case $# in 
    0)
        echo "You have not input type number. Input from: ${types[@]}"
        echo -e "0: step0*\t2: step1*\t1:other"
        exit 1
    ;;
    1)
        echo -e "Input \"remove\" as argument 2 to confirm this operation."
        exit 2
    ;;
    2)
        sleep 1
    ;;
    *)
        echo "Wrong inputs!"
        exit 3
    ;;
esac
types=(0 1 2)
if [[ ! ${types[@]} =~ $1 ]];then
    echo "Wrong input type number. Input from: ${types[@]}"
    echo -e "0: step0*\t2: step1*\t1:other"
    exit 1
fi
if [[ $2 != "remove" ]];then
    echo -e "Input \"remove\" as argument 2 to confirm this operation."
    exit 2
fi
case $1 in 
    0)
        sname=(Ge68)
        echo -e "Actually step0* don't take up much space.\nNothing happened."
    ;;
    1)
        sname=(AmC Ge68 Co60 Cs137 Laser0.1 Laser0.05)
        for s in ${sname[@]}
        do
            rm -rf $s/step1/root/*.root
            rm -rf $s/step1/user-root/*.root
            rm -rf $s/step2/root/*.root
        done
    ;;
    2)
        sname=(C14 Laser0.1 Laser0.05)
        for s in ${sname[@]}
        do
            rm -rf $s/step11/root/*.root
            rm -rf $s/step11/user-root/*.root
            rm -rf $s/step12/root/*.root
        done
    ;;
esac

