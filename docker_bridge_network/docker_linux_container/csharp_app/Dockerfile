# İlk aşama: Build
FROM mcr.microsoft.com/dotnet/sdk:8.0 AS build
WORKDIR /app
COPY . .
RUN dotnet publish -c Release -o /app/out

# İkinci aşama: Runtime
FROM mcr.microsoft.com/dotnet/runtime:8.0
WORKDIR /app
COPY --from=build /app/out . 
ENTRYPOINT ["dotnet", "csharp_app.dll"]
