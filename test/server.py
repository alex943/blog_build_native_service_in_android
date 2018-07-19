#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：server.py

import queue
import socket               # 导入 socket 模块
import threading
import time
import sys

class SocketControl():

    '''
    出现问题:
    v = conn.send(msg)
    error: [Errno 32] Broken pipe
    '''
    def msg_consumer(what, conn):
        print '\nacppect a conn in consumer.'
        while True:
            if conn is not None:
                msg = msg_que.get()
                v = conn.send(msg)
                print 'sent %d', v


    def msg_producer():
        while True:
            try:
                msg = raw_input("Msg: ")
            except:
                print 'exception'
            msg_que.put(msg)


    def socket_server():
        print "socket_server"
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)         # 创建 socket 对象
        s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)       # Send quick
        host = "localhost"          # 获取本地主机名
        port = 8080                 # 设置端口
        s.bind((host, port))        # 绑定端口
        s.listen(5)                 # 等待客户端连接

        while True:
            conn, addr = s.accept()     # 建立客户端连接。
            cons_thread = threading.Thread(target=msg_consumer, args=("Socket", conn))
            cons_thread.start()


    def start_method(s_method=1):
        print "s_method %d", s_method
        if s_method == 1:
            socket_server()
        elif s_method == 2:
            st = threading.Thread(target=socket_server)
            st.start()    
        else:
            st = threading.Thread(target=socket_server)
            st.start()
            msg_producer()


from flask import Flask, render_template, request

app = Flask(__name__)
sockets = SocketControl()
msg_que = queue.Queue()


@app.route('/submit', methods=["POST"])
def submit():
    msg = str(request.args.get('radio'))
    msg_que.put(msg)
    return 'Msg Sent'


@app.route('/')
def index():
    return render_template('index.html')


def is_close(connection):
    return False


if __name__ == '__main__':
    #app.run(host='127.0.0.1', port=8090)
    app.run()
    sockets.start_method(2) 
    
