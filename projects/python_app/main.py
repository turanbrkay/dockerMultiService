import socket
import time

APP1_HOST = "192.168.1.242"  # Senin bilgisayarının IP'si
APP1_PORT = 5000
APP_MAIN_PORT = 6000         # app2'nin geri göndereceği port (dinlenecek)

def send_to_app1(data):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((APP1_HOST, APP1_PORT))
        s.sendall(data.encode('utf-8'))
        print(f"[app_main → app1] Sent: {data}")

def receive_from_app2():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
        server.bind(('0.0.0.0', APP_MAIN_PORT))
        server.listen(1)
        print(f"[app_main] Listening on port {APP_MAIN_PORT} for response from app2...")

        conn, addr = server.accept()
        with conn:
            print(f"[app_main] Connection from {addr}")
            response = conn.recv(1024).decode('utf-8')
            print(f"[app2 → app_main] Received: {response}")
            return response

def main():
    start = 1
    while start <= 150:
        print("server started")
        end = start + 9
        data = ','.join(str(i) for i in range(start, end + 1))
        send_to_app1(data)
        print("data send")
        # app2'den sonucu dinle
        receive_from_app2()
        print("data receive")
        time.sleep(1)
        start = end + 1

if __name__ == '__main__':
    main()