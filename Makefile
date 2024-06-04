connect:
	sudo mount 192.168.70.202:/home/nodo/operativos /home/nodo/operativos
build:
	g++ -c Biblioteca.cpp
	ar rvs Biblioteca.a Biblioteca.o
	mpic++ freq_check.cpp -o /home/nodo/operativos/freq_check
	g++ freq_plotter.cpp -o /home/nodo/operativos/plotter
run:
	mpirun  --host localhost:1,geo:1,abner:1 -np 3 /home/nodo/operativos/freq_check EncryptedData.txt
conn:
	nmcli dev wifi connect "Diana's Pixel"
back:
	nmcli dev wifi connect "Diani y Rei"
