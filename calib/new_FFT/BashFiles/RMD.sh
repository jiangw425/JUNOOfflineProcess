sname=(AmC Ge68 Co60 Cs137 Laser0.1 Laser0.05 C14)
for d in ${sname[@]}
do
    rm -rf $d
done
rm -rf tmpworkout