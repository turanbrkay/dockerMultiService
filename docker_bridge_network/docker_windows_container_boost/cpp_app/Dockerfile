FROM gcc:12

# Boost bağımlılıkları için
RUN apt-get update && apt-get install -y build-essential python3 bzip2

# Boost klasörünü konteynıra al
WORKDIR /app
COPY . .

# Boost'u derle (sadece system modülü yeterli)
RUN cd boost_1_84_0 && ./bootstrap.sh --with-libraries=system && ./b2

# main.cpp'i derle (boost lib'leri ./boost_1_84_0/stage/lib içindedir)
RUN g++ -std=c++17 -I./boost_1_84_0 -L./boost_1_84_0/stage/lib -o main main.cpp -lboost_system -lpthread

# Çalıştır
CMD ["./main"]
