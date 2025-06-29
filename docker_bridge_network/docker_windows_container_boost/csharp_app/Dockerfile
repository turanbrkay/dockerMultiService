# Build aşaması (Windows SDK)
FROM mcr.microsoft.com/dotnet/sdk:8.0-nanoserver-ltsc2022 AS build
WORKDIR /app
COPY . .
RUN dotnet publish -c Release -o /app/out

# Runtime aşaması (Windows Runtime)
FROM mcr.microsoft.com/dotnet/runtime:8.0-nanoserver-ltsc2022
WORKDIR /app
COPY --from=build /app/out .
ENTRYPOINT ["dotnet", "csharp_app.dll"]
