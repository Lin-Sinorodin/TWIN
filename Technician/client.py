import socket

HOST = "127.0.0.1"
PORT = 12345
MSG_LEN_SIZE = 4  # length assumed to be DWORD
ENCODING = "utf-8"


def send_message(sock: socket.socket, message: bytes) -> None:
	sock.send(len(message).to_bytes(MSG_LEN_SIZE, byteorder="little"))
	sock.send(message)


def recv_message(sock: socket.socket) -> bytes:
	recv_len = int.from_bytes(sock.recv(MSG_LEN_SIZE), 'little')
	return sock.recv(recv_len)


def command_to_server(sock: socket.socket, command: bytes) -> None:
	send_message(sock, command)
	response = recv_message(sock)
	print(f"> command:  {command.decode(ENCODING)}")
	print(f"< response: {response.decode(ENCODING)}")
	print()


def execute_on_server(sock: socket.socket, exe_path: bytes) -> None:
	send_message(sock, b"RUN")
	send_message(sock, exe_path)
	response = recv_message(s)
	print(f"> command:  RUN {exe_path.decode(ENCODING)}")
	print(f"< response: {response.decode(ENCODING)}")
	print()


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
	s.connect((HOST, PORT))
	print("Connected to server\n")

	command_to_server(s, b"PING")
	command_to_server(s, b"HELLO")
	execute_on_server(s, b"C:\\WINDOWS\\system32\\notepad.exe")
