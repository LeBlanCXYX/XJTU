import socket
import threading # type: ignore
import os
import time
import pyaudio
import tkinter as tk
class Client:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.client_socket.connect((self.host, self.port))
        except Exception:
            print("failed to connect to sever")
        self.username=''
        self.password=''
        self.flag=0#表示是否登陆成功的变量
        self.stop_event=threading.Event()
        self.e=threading.Event()
        self.send_audio_event=threading.Event()#请求语音连接的另一方，收到回复后，告诉发送线程是否可以进入语音的事件
        self.send_audio_request=False
        self.whether_audio_flag=False#used to show recv thread is waiting for send thread to decide whether permit the audio connect
        self.whether_audio_event=threading.Event()#after send threading decide,tell the recv threading
        self.audio_accept=False#接受语音一方的标志
        self.audio_admit=False#请求语音连接一方的标志
        self.e.clear()
        self.file_flag=False
        self.file_response_answer=''
        self.chunk_size = 1024  # 512
        self.audio_format = pyaudio.paInt16
        self.channels = 1
        self.rate = 20000
        self.p = pyaudio.PyAudio() 
        self.permit_audio_running=True
        self.playing_stream = self.p.open(format=self.audio_format, channels=self.channels, rate=self.rate, output=True,
                                          frames_per_buffer=self.chunk_size)
        self.recording_stream = self.p.open(format=self.audio_format, channels=self.channels, rate=self.rate, input=True,
                                            frames_per_buffer=self.chunk_size)
        print(f"Connected to the server at {self.host}:{self.port}.")
    def receive_audio(self):
        while self.permit_audio_running==True:    
            try:
                data.decode()
                data=self.client_socket.recv(1024)
                self.playing_stream.write(data)
            except Exception:
                pass  
    
    def send_audio(self):
        while self.permit_audio_running==True:
            try:
                data=self.recording_stream.read(1024)
                self.client_socket.sendall(data)
            except Exception:
                pass
    def on_button_click(self):
        self.permit_audio_running=False
        time.sleep(2)
        self.client_socket.sendall("AUDIOOVERKEYWORD")


    def register(self):
        self.client_socket.send(f"REGISTER:{self.username}:{self.password}".encode())
        response = self.client_socket.recv(1024).decode()
        if response == "REGISTER_SUCCESS":
            print("Registration successful!")
        else:
            print("Registration failed. Username may already be taken.")

    def login(self):
        self.client_socket.send(f"LOGIN:{self.username}:{self.password}".encode())
        response = self.client_socket.recv(1024).decode()
        if response == "LOGIN_SUCCESS":
            print("Login successful!")
            self.flag=1
        else:
            print(response)

    def send_message(self):
        try:
            while True:
                temp=''
                user_input = input("Enter a message to send ('quit' to quit): ")
                if user_input.startswith("#mul!"):
                    temp+=user_input
                    if user_input=='#mul!':
                        temp+='\n'
                    while not user_input.endswith("#endmul!"):#输入"#endmul!"进入多行模式
                        user_input=input("#mul now you can end with '#endmul!'to end: ")#以"#endmul!"结束来退出多行模式
                        temp+=user_input+'\n'
                    while temp.endswith('\n'):
                        temp=temp[0:-1]
                    user_input=temp[4:-8]
                    while user_input.endswith('\n'):
                        user_input=user_input[0:-1]
                if self.whether_audio_flag==True:
                    while True:
                        if user_input=='yes':#yes to connect接收方开始发送语音
                            self.client_socket.send("AUDIOyes!!".encode())
                            self.audio_accept=True
                            self.whether_audio_event.set()
                            time.sleep(1)
                            self.send_audio()
                            break
                        elif user_input=='no':
                            self.client_socket.send("AUDIOno!!".encode())
                            self.audio_accept=False
                            self.whether_audio_event.set()
                            break
                        else:
                            user_input=input("wrong enter,enter again! yes/no")
                    continue
                if user_input.startswith('audio') and user_input.endswith('!'):#inputlike"audio xyx!"
                    audio_to_username=user_input.split()[1]
                    audio_to_username=audio_to_username[0:-1]
                    print(f"you are trying to audio to {audio_to_username}")
                    self.client_socket.send(f"AUDIO!!! to {audio_to_username} from {self.username}".encode())
                    self.send_audio_request=True
                    self.send_audio_event.wait()
                    self.send_audio_request=False
                    self.send_audio_event.clear()#can be used again
                    if self.audio_admit==False:
                        pass
                    else:#succed to audio
                        self.audio_admit=False
                        #发送放开始发送语音
                        self.root=tk.Tk()
                        self.root.title("Audio")
                        button=tk.Button(self.root,text='Exit Audio',command=self.on_button_click)
                        button.pack(pady=20)
                        worker_thread_obj=threading.Thread(target=self.send_audio)
                        worker_thread_obj.start()
                        self.root.mainloop()                        
                    continue #正常准备发送消息   
                        
                if self.file_flag==True:
                    self.file_response_answer=user_input.lower()
                    if self.file_response_answer=='yes':
                        self.client_socket.send('yes this is real a receving file keyword!!!'.encode())
                    else:
                        self.client_socket.send('no this is real a receving file keyword!!!'.encode())
                    self.e.set()
                else:
                    if user_input.lower() == 'quit':
                        self.client_socket.send(f"quit:{self.username}".encode())
                        self.stop_event.set()
                        break
                    elif user_input.lower().startswith("file"):
                        file_path = user_input.split()[1]
                        if os.path.exists(file_path):
                            self.send_file(file_path)
                        else:
                            print("File Not Found")
                    else:   
                        self.client_socket.send(f"{self.username}:{user_input}".encode())
        except KeyboardInterrupt:
            print("keyboardintterupt")
            self.client_socket.send(f"quit:{self.username}".encode())
            self.stop_event.set()
        except Exception as e:
            print("An error occurred while sending data?:", e)

    def send_file(self, file_path):
        try:
            filename=os.path.basename(file_path)
            file_size=os.path.getsize(file_path)
            self.client_socket.send(f'FILE_TRANSFER {filename} {str(file_size)} from {self.username}'.encode())
            time.sleep(1)
            with open(file_path, 'rb') as file:
                while True:
                    data = file.read(1024)
                    if not data:
                        break
                    self.client_socket.send(data)
            print(f"File {filename} sent successfully")            
        except Exception:
            print("File transfer failed")

    def rec_message(self):
            while not self.stop_event.is_set():
                try:
                    while True:
                        data=self.client_socket.recv(1024)
                        if not data:
                            break
                        message=data.decode()
                        if message.startswith("AUDIOREQUEST!!!"):#means some user want to audio to now socket
                            print(message)
                            self.whether_audio_flag=True
                            self.whether_audio_event.wait()
                            self.whether_audio_event.clear()
                            self.whether_audio_flag=False
                            if self.audio_accept==True:#接收方开始接受语音
                                self.audio_accept=False
                                self.root=tk.Tk()
                                self.root.title("Audio")
                                button=tk.Button(self.root,text='Exit Audio',command=self.on_button_click)
                                button.pack(pady=20)
                                worker_thread_obj=threading.Thread(target=self.receive_audio)
                                worker_thread_obj.start()
                                self.root.mainloop()                                
                            else:
                                pass
                            continue
                        if self.send_audio_request==True:#对于语音请求线程socket在等待是否是连接的判断
                            if message.startswith('FAILAUDIO!!!'):
                                print(message)
                                self.audio_admit=False
                                self.send_audio_event.set()
                            else:#作为发起语音的一方，语音被接受，准备进入语音
                                print(message)
                                self.audio_admit=True
                                self.send_audio_event.set()
                                #发起语音方开始接收语音
                                self.receive_audio()
                            continue#进入到audio的判断就不会管其他剩余的了
                        if not self.stop_event.is_set():
                            message=data.decode()
                            print(f"{message}")
                        if message.startswith('FILE_TRANSFER'):
                            print("Do you want to receive this file?Print Enter and Then input 'yes'")
                            self.file_flag=True
                            self.e.wait()
                            self.e.clear()#event e can be used agin
                            self.file_flag=False#send thread can work normally
                            if self.file_response_answer == 'yes':
                                self.receive_file(message)
                except KeyboardInterrupt:
                    print("keyboardintterupt")
                except UnicodeDecodeError:
                    print("receive audio as news")
                except Exception as e:
                    print("An error occurred while receiving data:", e)


    def receive_file(self, message):
        filename = message.split()[1]
        file_size = int(message.split()[2])

        print(f"Receiving file {filename} ({file_size} bytes)...")
        received = 0
        save_path = os.path.join(os.getcwd(), filename)

        with open(save_path, 'wb') as f:
            while received < file_size:
                chunk = self.client_socket.recv(1024)
                f.write(chunk)
                received += len(chunk)
        print(f"File {filename} received successfully and saved at {save_path}")

    def close_connection(self):
        self.client_socket.close()

    def start(self):
        self.username = input("Enter your username: ")
        self.password = input("Enter your password: ")
        try:
            # Register or login
            while True:
                action = input("Do you want to (R)egister or (L)ogin? ").lower()
                if action == 'r':
                    self.register()
                elif action == 'l':
                    self.login()
                    if self.flag==1:
                        break
                else:
                    print("Invalid choice. Please enter 'R' or 'L'.")
                self.username=input("Enter your username again: ")
                self.password = input("Enter your password again: ")
        except KeyboardInterrupt:
            print("A keyboard interrupt happen")
            self.client_socket.send("quit".encode())
        # Start sending and receiving messages
        send_thread = threading.Thread(target=self.send_message)
        rec_thread = threading.Thread(target=self.rec_message)
        send_thread.start()
        rec_thread.start()
        send_thread.join()
        rec_thread.join()
        self.close_connection()

if __name__ == '__main__':
    server_host = '123.249.2.46'  # 服务器的 IP 地址
    server_port = 8888  # 服务器的端口号
    client = Client(server_host, server_port)
    client.start()
