Bu senaryoda linux container kullanılmıştır.
Csharp .net core ile çalıştığından hali hazırda zaten linux tarafından desteklenmektedir.
Cpp socketleri linux da çalışabilecek şekilde boost kullanılarak düzenlenmiştir. Böylece hem windows hem Linux da sorun yaşamaycağız ve kodumuzu sürekli değiştirmek zorunda kalmayacağız.

------------------------------------------------------------------------------------------

Windows'ta boost ile derlemek için tasks.json dosyanı güncelle:
json
Copy
Edit
"args": [
    "-fdiagnostics-color=always",
    "-g",
    "${file}",
    "-o",
    "${fileDirname}\\${fileBasenameNoExtension}.exe",
    "-lboost_system",
    "-lws2_32"
]
Not: Boost kurulumu ve boost_system .lib/.dll dosyalarının PATH veya LIB ortam değişkenlerinde görünmesi gerekir.

------------------------------------------------------------------------------------------
.env dosyasına aşağıdakini ekledik
LD_LIBRARY_PATH=/app/boost_1_84_0/stage/lib
------------------------------------------------------------------------------------------


Şimdi burada yaptığımız şey ile container yine çalıştı tek fark cpp de windows'a özgü soketler yerine boost kullandık. Böylece boost'u windows'un yanında Linux 'da da indirebildik ve container'ı ayağa kaldırabildik. 



------------------------------------------------------------------------------------------
docker-compose up --build -d
