Bu senaryoda linux container kullanılmıştır.
Cpp socketleri linux da çalışabilecek şekilde düzenlenmiştir.
Csharp .net core ile çalıştığından hali hazırda zaten linux tarafından desteklenmektedir.

------------------------------------------------------------------------------------------

Ama sorun şu; Linux container'a uyarlamak için cpp içindeki socketleri linuxa çevirdik ama bu seferde bu kod windowsda çalışmaz hale geldi. Çünkü include'ları değiştirdik tamamen linux'a uyarlı.
Windows’ta karşılıkları şunlardır:
Linux (POSIX)		Windows (Winsock)
sys/socket.h		winsock2.h
unistd.h (close())	closesocket()
socklen_t		int
bind, recvfrom vs.	Winsock fonksiyonları

Senin bu kodun Windows’ta derlenebilmesi için en baştan tekrar winsock2.h ile yazılması gerekir. Ama bu sefer de Linux desteğini kaybedersin.

------------------------------------------------------------------------------------------
docker-compose up --build -d
