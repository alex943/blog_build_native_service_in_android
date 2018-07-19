#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：client.py

import socket               # 导入 socket 模块

def socket_client():
    s = socket.socket()         # 创建 socket 对象
    host = "localhost"          # 获取本地主机名
    #host = socket.gethostname()
    port = 8080                 # 设置端口好

    s.connect((host, port))
    #print s.recv(1024)
    #s.send("This is client.")

    while True:
        msg = s.recv(1024)
        print msg
        if msg == 'quit':
            break

    s.close()  


socket_client()    