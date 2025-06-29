# Windows tabanlı container kullanılıyor
FROM mcr.microsoft.com/windows/servercore:ltsc2022

# Uygulama klasörü
WORKDIR /app

# Derlenmiş .exe dosyasını kopyala
COPY main.exe .

# Uygulamayı başlat
ENTRYPOINT ["main.exe"]
