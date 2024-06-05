connect:
	sudo mount 192.168.70.202:/home/nodo/operativos /home/nodo/operativos
build:
	mkdir -p build/
	g++ src/Cliente.cpp -o build/cliente
	g++ -c src/Biblioteca.cpp -o build/Biblioteca.o
	ar rvs build/Biblioteca.a build/Biblioteca.o
	mpic++ src/freq_check.cpp build/Biblioteca.a -o /home/nodo/operativos/freq_check
run:
	mpirun  --host localhost:1,geo:1,abner:1 -np 3 /home/nodo/operativos/freq_check output/EncryptedData.txt
conn:
	nmcli dev wifi connect "Diana's Pixel"
back:
	nmcli dev wifi connect "Diani y Rei"
clean:
	rm -rf build
