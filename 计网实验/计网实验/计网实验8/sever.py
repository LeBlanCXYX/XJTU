import socket
import threading
import os
class ChatServer:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1) 
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(20)  # Listen for up to 5 connections
        self.Lock = threading.Lock()  # 创建互斥锁
        print(f"Chat server listening on {self.host}:{self.port}")
        self.wating_file_response=False
        self.clients = []  # List to store connected client sockets
        self.user_db = {}  # User database (username: password)
        self.login_db={}    #user logining now
        self.file_confirm=threading.Event()
        self.file_accept=False
        self.wating_audio_response=False
        self.audio_accept=False
        self.audio_confirm=threading.Event()
        self.audio_list=[]
        self.socket_lock=threading.Lock()
        self.file_send_lst=[]
        self.file_send_lock=threading.Lock()#文件的
        self.lock=threading.Lock()

    def handle_client(self, client_socket, client_address):
        try:
            while True:
                data = client_socket.recv(1024)
                if not data:
                    break
                message = data.decode()
                print(f"Received from {client_address}: {message}")
                self.Lock.acquire()
                 # Check if the message is a login request
                if message.startswith("LOGIN:"):
                    _, username, password = message.split(":")
                    if username in self.user_db and self.user_db[username] == password and username not in self.login_db:
                        client_socket.send(b"LOGIN_SUCCESS")
                        self.login_db[username]=client_socket
                        self.Lock.release()
                        break
                    elif username in self.user_db and self.user_db[username]!= password:
                        client_socket.send(b"LOGIN_FAILED because the password is wrong")
                    elif username in self.user_db and self.user_db[username] == password and username in self.login_db:
                        client_socket.send(b"LOGIN_FAILED because the account is using now")
                    else:
                        client_socket.send(b"LOGIN_FAILED because the account not registed now")
                elif message.startswith("REGISTER:"):
                    _, username , password = message.split(":")
                    if username in self.user_db:
                        client_socket.send(b"LOGIN_FAILED because the account's already registed")
                    else:
                        client_socket.send(b"REGISTER_SUCCESS")
                        self.user_db[username]=password
                self.Lock.release()    
            while True:#succeed to login
                data = client_socket.recv(1024)
                if not data:
                    break
                with self.socket_lock:
                    if client_socket in self.audio_list:
                        try:
                            temp=data.decode()
                            temp=temp.split(':')[1]
                            if temp=='AUDIOEXIT':
                                self.audio_list.remove(client_socket)
                                continue
                            else:
                                for to_client in self.audio_list:
                                    if to_client!=client_socket:
                                        to_client.sendall(data)
                        except UnicodeDecodeError:
                            for to_client in self.audio_list:
                                if to_client!=client_socket:
                                    to_client.sendall(data)
                        continue
                try:
                    message = data.decode()
                except UnicodeDecodeError:
                    continue
                if message=="AUDIOEXITKEYWORD!!!":
                    with self.socket_lock:
                        self.audio_list.remove(client_socket)
                    continue
                if self.wating_audio_response==True:
                    if message=='AUDIOyes!!':#
                        self.audio_accept=True
                        self.audio_confirm.set()
                    elif message=='AUDIOno!!':
                        self.audio_accept=False
                        self.audio_confirm.set()
                    else:
                        self.audio_accept=False
                        self.audio_confirm.set()
                        print("something wrong happen,audio connection failed")                    
                    continue
                if message.startswith("AUDIO!!!"):
                    self.username1=message.split()[4]#who ask for audio
                    self.username2=message.split()[2]#who is asked for
                    if self.username2 not in self.login_db:
                        print(f"audio falid because there is no such use named {self.username2}")
                        client_socket.send(f"FAILAUDIO!!! because here is no such user named{self.username2}".encode())
                    else:#there is such a username
                        temp_socket=self.login_db[self.username2]
                        temp_socket.send(f"AUDIOREQUEST!!! {self.username1} is asking for a audio for you,do you want to receive?yes/no".encode())
                        self.wating_audio_response=True
                        self.audio_confirm.wait()
                        self.audio_confirm.clear()
                        self.wating_audio_response=False
                        if self.audio_accept==True:
                            self.audio_accept=False
                            client_socket.send("SUCCEEDAUDIO!!!".encode())
                            self.audio_list.append(temp_socket)
                            self.audio_list.append(client_socket)
                        else:
                            client_socket.send(f"FAILAUDIO!!! becauese {self.username2} refused your audio request".encode())
                            
                else:               
                    if message=='yes this is real a receving file keyword!!!':
                        with self.Lock:
                            self.file_accept=True
                            self.file_send_lst.append(client_socket)
                            self.file_confirm.set()
                            continue
                    elif message=='no this is real a receving file keyword!!!':
                        with self.Lock:
                            self.file_accept=False
                            self.file_confirm.set()
                            continue
                    if message.startswith("quit"):
                        with self.Lock:
                            self.login_db.pop(username)
                            self.clients.remove(client_socket)
                        print(f"quit from {username} address is {client_address}")                        
                        client_socket.send("rec_over!!!".encode())
                        break
                    elif message.startswith('FILE_TRANSFER'):
                        self.receive_and_broadcast_file(client_socket, message)
                    else:
                        print(f"{message} from {client_address}")                   # Broadcast the message to all other connected clients
                        for other_client in self.clients:
                            if other_client != client_socket and other_client not in self.file_send_lst:
                                other_client.send(data)
        except KeyboardInterrupt:
            print("keyboard interrupt happens")
        except Exception as e:
            print(f"Error handling client {client_address}: {e}")
        finally:
            with self.Lock:                
                print(f"Client {client_address} disconnected")

    def receive_and_broadcast_file(self, client_socket, message):
        filename = message.split()[1]
        print("Receiving file", filename)

        # 接收文件大小
        file_size = int(message.split()[2])
        send_username=message.split()[4]
        # 接收文件内容
        received = 0
        sent=0
        save_path = os.path.join(os.getcwd(), filename)
        with open(save_path, 'wb') as f:
            while received < file_size:
                chunk = client_socket.recv(8192)
                f.write(chunk)
                received += len(chunk)
        print("Sever succeed to save the file")
        # 将文件写入当前目录
        print(f"File {filename} received and saved at {save_path}")

        # 广播文件给其他客户端
        # 发送文件信息给其他客户端
        for other_client in self.clients:
            if other_client != client_socket:
                # 发送文件传输请求
                other_client.sendall(f'FILE_TRANSFER_REQUEST {filename} {file_size} from {send_username}'.encode())
                # 等待客户端回复
                self.file_confirm.wait()
                self.file_confirm.clear()
                if  self.file_accept==True:
                    self.file_accept=False
                    with open(save_path,'rb') as f:
                        self.file_send_lock.acquire()
                        while sent<file_size:
                            chunk = f.read(8192)
                            other_client.sendall(chunk)
                            sent+=len(chunk) 
                        self.file_send_lst.remove(other_client)
                        self.file_send_lock.release()                     
                    print(f"File {filename} sent to {other_client.getpeername()} sucessfully")

    def start(self):
        try:
            while True:
                client_socket, client_address = self.server_socket.accept()
                with self.Lock:
                    print(f"Accepted connection from {client_address}")
                    self.clients.append(client_socket)
                    # Create a separate thread for each connected client
                client_thread = threading.Thread(target=self.handle_client, args=(client_socket, client_address))
                client_thread.start()
        except KeyboardInterrupt:
            print("Server interrupted by user. Shutting down gracefully.")
        except Exception as e:
            print(f"Sever close because Some fault{e}")
        finally:
            self.server_socket.close()

if __name__ == "__main__":
    server_host = "0.0.0.0"  # Use your server's IP address or hostname
    server_port = 8887  # Choose an available port
    chat_server = ChatServer(server_host, server_port)
    chat_server.start()
