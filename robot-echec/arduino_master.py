#!/usr/bin/env python
import threading
import time
import os
from os import system
import sys
import utils
from enum import Enum
from pySerialTransfer import pySerialTransfer as txfer
class MessageType(Enum):
    SERIALIZE=1,
    DIRECT=2,
    SCREEN =3,
        
def validate_command(commande):
    return commande[0]=="<" and commande[-1]==">"
def validate_response(resp):
    resp=resp.strip()
    return resp[:3]=="ACK" and resp[3]=="<" and resp[-1]==">"
class SerialHandler(object):
    """
    SerialHandler : classe permettant separement dans un thread de faire des echanges

    Attributes:
        ser(Serial) : instance de la connection avec le serial
    """


    def __init__(self,ser_instance):
        self.ser=ser_instance
        #files comportant les commandes et les reponses pas encore traitées
        self.commandes=[]
        self.reponses=[]

    def send_commande(self,cmd):
        self.commandes.append(cmd) 

    def query_commande(self,cmd,callback):
        self.reponses.append([cmd,callback])

    def do_query(self,cmd):
        self.ser.write(cmd.encode())
        time.sleep(0.5)
        while self.ser.in_waiting==0:
            print("rien")
            pass
        print(self.ser.readline())
        return self.ser.read(self.ser.in_waiting)

    def main_loop(self):
        while self.ser.is_open:
            while self.reponses:
                cmd,callback=self.reponses.pop(0)
                resp=self.do_query(cmd)
                if not validate_response(resp):
                    resp=""
                callback(cmd,resp)
                    
            while self.commandes:
                print("lol")
                self.ser.write((self.commandes.pop(0)).encode())
            time.sleep(0.01)

    def run_thread(self):
        self.thread = threading.Thread(target=self.main_loop)
        self.thread.start()

    def terminate(self):
        self.ser.close()
        self.thread.join()
        
class ArduinoBridge():
    """
    ArduinoBridge : classe permettant de faire le pont entre un arduino et le code python

    Attributes:
        connection(Serial) : objet stockant la connection avec l'arduino
        port(str): emplacement où est connecté l'arduino (du type USB+index)
        baud(int):nombre de bits par seconde dans la liaison série
        pending(bool):booléen permettant de savoir si on a une commande en cours

    """

    def __init__(self, port, baud):
        self.connection = None
        self.port = port
        self.baud = baud
        self.pending=False

        self.init_connection()
        self.handler=SerialHandler(self.connection)
    
    def on_result(self,cmd,result):
        """
        on_result callback appellée quand l'arduino repond a une commande

        Arguments:
            cmd {string} -- commande envoyé a l'arduino
            result {string} -- resultat de celle ci
        """
        if result=="":
            print("aucune reponse")
        else:
            print(f'l arduino a repondu à {cmd} avec {result}.')
        self.pending=False

    def init_connection(self):
        """
        init_connection: initialise la connexion avec l'arduino et quitte le prog si erreur
        """
        try:
            #linux os
            if os.name=="posix":
                print(self.port)
                self.connection = txfer.SerialTransfer('/dev/tty'+self.port)
                print("Connection SUCESS with", self.port)
            else:
                self.connection = serial.Serial(self.port)
                print("Connection SUCESS with", self.port)
            self.connection.open()
            time.sleep(2)

        except Exception as e:
            print(e)
            sys.exit()
    def run(self):
        self.handler.run_thread()
        while True:
            #si on attend pas de resultat
            if not self.pending:
                print("\nAVAILABE COMMANDS:(finir la commande avec ;)")
                print("<save>(sauvegarder dans un fichier les positions moteurs de chaque case)")
                print("<direct,boolean>(toogle le mode direct sur l'arduino")
                cmd=input(">")
                if validate_command(cmd):
                    self.pending=True
                    self.handler.query_commande(cmd=cmd,callback=self.on_result)
                else:
                    print("veuillez entrer une commande valide")
            time.sleep(0.01)
            



# callbacks

# arduino bridge setup
if os.name=="posix":
    bridge = ArduinoBridge("ACM0", 9600)
    
else:
    bridge = ArduinoBridge("COM3", 9600)

bridge.run()