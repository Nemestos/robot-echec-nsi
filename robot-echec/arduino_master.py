#!/usr/bin/env python
import threading
import time
import os
from os import system
from pynput.keyboard import Key, Listener
import sys
import utils
from enum import Enum
import serial
import threading
current_key = ""
class MessageType(Enum):
    SERIALIZE=1,
    DIRECT=2,
    SCREEN =3,
        

class ArduinoBridge(threading.Thread):
    """
    ArduinoBridge : classe permettant de faire le pont entre un arduino et le code python et faire des echanges unidirectionnelles

    Attributes:
        connection(Serial) : objet stockant la connection avec l'arduino
        port(str): emplacement où est connecté l'arduino (du type USB+index)
        baud(int):nombre de bits par seconde dans la liaison série

    """

    def __init__(self, port, baud):
        self.connection = None
        self.port = port
        self.baud = baud

    def init_connection(self):
        """
        init_connection: initialise la connexion avec l'arduino et quitte le prog si erreur
        """
        try:
            #linux os
            if os.name=="posix":
                print(self.port)
                self.connection = serial.Serial(port='/dev/tty'+self.port, baudrate=self.baud,timeout=None)
                print("Connection SUCESS with", self.port)
            else:
                self.connection = serial.Serial(self.port,self.baud,timeout=None,)
                print("Connection SUCESS with", self.port)

        except Exception as e:
            print(e)
            sys.exit()
    def run(self):
        self.init_connection()
        while True:
            print("AVAILABE COMMANDS:")
            print("d booleen(activer le mode direct sur l'arduino ou non)")
            print("s(sauvegarder dans un fichier les positions moteurs de chaque case")
            



# callbacks


def key_press(key):
    global current_key
    current_key = str(key).replace("'", "").replace("Key.", "")


def key_release(key):
    pass

# arduino bridge setup
if os.name=="posix":
    bridge = ArduinoBridge("ACM0", 9600)
    bridge.init_connection()
else:
    bridge = ArduinoBridge("COM3", 9600)
    bridge.init_connection()

# keyboard setup
listener = Listener(on_press=key_press, on_release=key_release)
listener.start()


loop=asyncio.get_event_loop()#boucle comportant toute les taches asynchrones(voir doc)
loop.add_reader()