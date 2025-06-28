import socket
import time

APP1_HOST = "192.168.1.242"  # Hedef IP (veriyi alacak sunucu)
APP1_PORT = 5000  # Hedef port (dinleyen sunucu portu)


def send_to_app1(data):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        try:
            s.connect((APP1_HOST, APP1_PORT))
            s.sendall(data.encode('utf-8'))
            print(f"[Client → Server] Sent: {data}")
        except Exception as e:
            print(f"Connection failed: {e}")


def main():
    array = [1, 2, 3, 4, 5]
    while True:
        # Diziyi gönder
        data = ','.join(str(i) for i in array)
        send_to_app1(data)

        # 3 saniye bekle
        time.sleep(3)

        # Her elemana +5 ekle
        array = [x + 5 for x in array]


if __name__ == '__main__':
    main()
