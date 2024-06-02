connect:
	sudo mount 192.168.70.202:/home/nodo/operativos /home/nodo/operativos
build:
	mpic++ -o /home/nodo/operativos/freq_count freq_check.cpp
run:
	mpirun --host geo -np 3 --oversubscribe /home/nodo/operativos/freq_count EncryptedData.txt 
conn:
	nmcli dev wifi connect "Diana's Pixel"
back:
	nmcli dev wifi connect "Diani y Rei"