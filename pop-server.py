#!/usr/bin/python
"""
Usage:
    python pop-server.py serverAddress:port messagePath
"""
import logging
import os
import socket
import sys
import traceback

logging.basicConfig(format="%(name)s %(levelname)s - %(message)s")
log = logging.getLogger("pypopper")
log.setLevel(logging.INFO)


class ChatterboxConnection(object):
    END = "\n"
    def __init__(self, conn):
        self.conn = conn
    def __getattr__(self, name):
        return getattr(self.conn, name)
    def sendall(self, data, END=END):
        if len(data) < 50:
            log.debug("send: %r", data)
        else:
            log.debug("send: %r...", data[:50])
        data += END
        self.conn.sendall(data)
    def recvall(self, END=END):
        while True:
            chunk = self.conn.recv(4096)
            if END in chunk:
                break
        return chunk[:-1]


class ListeMessages():
    def __init__(self, databasePath):
        self.messagePathList = os.listdir(databasePath)
        self.nombreMessages = len(self.messagePathList)
        self.liste = []
        for (i, path) in enumerate(self.messagePathList):
            self.liste.append(Message(databasePath + "/" + path, i))

    def printList(self):
        for mex in self.liste:
            mex.printMessage()

    def getMexById(self, id):
        for mex in self.liste:
            if mex.id == id:
                return mex

class Message(object):
    def __init__(self, filePath, id):
        msg = open(filePath, "r")
        try:
            self.filePath = filePath
            self.id = id
            self.data = msg.read()[:-2]
            self.size = len(self.data)
            self.top, self.body = self.data.split("\n\n")
        finally:
            msg.close()

    def printMessage(self):
        print "Message: \nID: {0}\nTOP: {1}\nBODY: {2}\n".format(self.id, self.top, self.body)


def handleUser(data, msgList):
    userName = "moi"
    request, arg = data.split(" ", 1)
    print "handleUser, arg:{0}".format(arg)
    if '\n' in arg: 
        arg = arg[:-1]
    if(arg == userName):
        return "+OK user accepted"
    else:
        return "-ERR user NOT accepted"

def handlePass(data, msgList):
    passWord = "a"
    request, arg = data.split(" ", 1)
    if '\n' in arg:
        arg = arg[:-1]
    if(arg == passWord):
        return "+OK pass accepted"
    else:
        return "-ERR pass NOT accepted"

def handleList(data, msgList):
    reponse = []
    reponse.append("+OK {0} messages\n".format(msgList.nombreMessages))
    for mex in msgList.liste:
        reponse.append("{0} {1}\n".format(mex.id, mex.size))
    reponse.append(".")
    return "".join(reponse)

def handleTop(data, msgList):
    try:
        print "in handleTOP,data:%s" % data
        if '\n' in data: data = data[:-1]
        result = data.split(' ')
        print "LISTA:\n{0}".format(result)
        n = int(result[1])
        n2 = int(result[2])
        if(n2 != 0): return "-ERR TOP request"
    except:
        return "-ERR TOP request"

    mex = msgList.getMexById(n)
    if mex == None:
        return "-ERR message not found"
    return "+OK top of message follows\n%s\n." % mex.top

def handleRetr(data, msgList):
    log.info("message sent")
    try:
        print "in handleRetr,data:%s" % data
        if '\n' in data: data = data[:-1]
        result = data.split(' ', 1)
        print "LISTA:\n{0}".format(result)
        n = int(result[1])
    except:
        return "-ERR TOP request"


    mex = msgList.getMexById(n)
    if mex == None:
        return "-ERR message not found"
    return "+OK %i octets\n%s\n." % (mex.size, mex.data)

def handleNoop(data, msgList):
    return "+OK"

def handleQuit(data, msgList):
    print "in quit request, data:{0}".format(data)
    return "+OK pypopper POP3 server signing off"

dispatch = dict(
    USER=handleUser,
    PASS=handlePass,
    LIST=handleList,
    TOP=handleTop,
    RETR=handleRetr,
    NOOP=handleNoop,
    QUIT=handleQuit,
)

def serve(host, port, databasePath):
    assert os.path.exists(databasePath)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((host, port))
    try:
        if host:
            hostname = host
        else:
            hostname = "localhost"
        log.info("pypopper POP3 serving '%s' on %s:%s", databasePath, hostname, port)
        while True: 
            sock.listen(1)
            conn, addr = sock.accept()
            log.debug('Connected by %s', addr)
            try:
                msgListList = ListeMessages(databasePath)
                conn = ChatterboxConnection(conn)
                #conn.sendall("+OK pypopper file-based pop3 server ready")
                while True:
                    data = conn.recvall()#[:-1]
                    print "data contains: {0}".format(data)
                    log.debug('data contains: %s', data)
                    command = data.split(None, 1)[0]
                    log.debug('command contains: %s', command)

                    try:
                        cmd = dispatch[command]
                    except KeyError:
                        conn.sendall("-ERR unknown command")
                    else:
                        conn.sendall(cmd(data, msgListList))
                        if cmd is handleQuit:
                            break
            finally:
                conn.close()
                msgList = None
    except (SystemExit, KeyboardInterrupt):
        log.info("pypopper stopped")
    except Exception, ex:
        log.critical("fatal error", exc_info=ex)
    finally:
        sock.shutdown(socket.SHUT_RDWR)
        sock.close()

def createDatabase(databasePath):
    contenu = open(databasePath).read()
    messages = contenu.split("**//**\n")

    directory = "messagesDir"
    if not os.path.exists(directory):
        os.makedirs(directory)

    for (i, mex) in enumerate(messages):
        #print mex
        filePath = directory + "/" + str(i+1) + ".txt" 
        print "filePath: " + filePath
        f = open(filePath, "w")
        f.write(mex)
        
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print "USAGE: [<host>:]<port> <databasePath>"
    else:
        _, port, databasePath = sys.argv
        if ":" in port:
            host = port[:port.index(":")]
            port = port[port.index(":") + 1:]
        else:
            host = ""
        try:
            port = int(port)
        except Exception:
            print "Unknown port:", port
        else:
            createDatabase("database.txt")
            if os.path.exists(databasePath):
                serve(host, port, databasePath)
            else:
                print "Database not found:", databasePath