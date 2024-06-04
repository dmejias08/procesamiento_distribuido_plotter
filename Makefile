connect:
	sudo mount 192.168.70.202:/home/nodo/operativos /home/nodo/operativos
build:
	mpic++ freq_check.cpp Biblioteca.a -o /home/nodo/operativos/freq_check
	g++ freq_plotter.cpp -o plotter
run:
	mpirun  --host geo,abner -np 3 --oversubscribe /home/nodo/operativos/freq_check EncryptedData.txt
conn:
	nmcli dev wifi connect "Diana's Pixel"
back:
	nmcli dev wifi connect "Diani y Rei"
