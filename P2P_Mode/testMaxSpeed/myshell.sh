for ((i=0;i<=11;i++))
do
	a=`expr 10 \* $i`;
	sudo ./myP2Psend.cpp_exe 250 $a 0 100 sf7 500
done

















