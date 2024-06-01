connect:
	sudo mount 192.168.70.202:/home/nodo/operativos /home/nodo/operativos
build:
	mpic++ -o /home/nodo/operativos/freq_count freq_count.cpp
run:
	mpirun --host abner,isaac,geo -np 3 --oversubscribe ./home/nodo/operativos/freq_count /home/nodo/operativos/Quijote.txt 
