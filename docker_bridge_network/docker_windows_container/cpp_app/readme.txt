






--------------------------------------------------------------------------------------------

g++ main.cpp -o main.exe -static -lws2_32

exe dosyasını çıkarırken bu şekilde çıkarmalıyız, böylece tüm .dll ler de exe içinde olur ve başka bilgisayara geçtiğimizde sorun yaşamayız.

docker-compose up --build -d
