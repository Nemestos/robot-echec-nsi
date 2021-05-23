#!/usr/bin/env python
import time
import os
from os import system
from pynput.keyboard import Key, Listener
import threading
import sys
import serial
import utils
from enum import Enum

current_key = ""
# MAiN

def clavier_cmd():
    """
    clavier_cmd : fonction permettant d'envoyer des commandes a l'arduino
    """
    availables_keys=["s","d"]
    print("---COMMANDES---(Echap pour retourner au main menu)")
    global current_key
    print("key "+current_key)
    if current_key == 'esc':
        scr.change("main")

    if current_key in availables_keys:
        #envoie de la commande permettant de serialiser
        if current_key=="s":
            bridge.send_message("", MessageType.SERIALIZE)
        elif current_key=="d":
            bridge.send_message("",MessageType.DIRECT)
            
        current_key = ""
def exit_cmd():
    """
    exit_cmd : fonction permettant de quitter le programme depuis le menu principal
    """
    sys.exit()


class MessageType(Enum):
    SERIALIZE=1,
    DIRECT=2,
    SCREEN =3,


class ArduinoBridge:
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

    def send_message(self, message: str, m_type):
        """
        send_message envoie un message string au serial de l'arduino
        Arguments:
            message {str} -- message à envoyés
            m_type(MessageType) -- type du message à envoyé(situé en en tete du message)
        """
        print("SENDING:")
        payload = "C"+str(+m_type.value[0])+message+"\n"
        print(payload)
        self.connection.write(bytes(payload,'utf-8'))

    def get_message(self):
        """
        get_message permet de recevoir des messages de l'arduino

        
        """
        msg= self.connection.read_until(";").decode()
        return msg
        


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


class Entry:
    """
    Entry : classe servant à contenir le nom et l'action d'une entrée dans un menu

    Attributes:
        name(str) -- nom de l'entrée
        action(pointeur sur fonction) -- reférence à la fonction à executé à chaque tick pour cette entrée

    """

    def __init__(self, name, action):
        self.name = name
        self.action = action

    def update(self):
        """
        update: methode appellant elle même l'action et appelée a chaque tick par le screen

        """
        self.action()


class Menu:
    """
    Menu : classe servant à regrouper des entrées et les afficher pour faire un choix et se rediriger vers celle ci

    Attributes:
        name(str) -- nom du menu qui sera affiché en titre
        entries(list of Entry) -- liste des entrées possibles
    """

    def __init__(self, title, entries=[]):
        self.name = title
        self.entries = entries

    def update(self):
        """
        update :methode appelée par le Screen a chaque tick
        """
        if self.entries == []:
            print("Menu is empty !")
        else:
            print("---"+self.name+"---")
            for i in range(len(self.entries)):
                print(f'{i+1}-{self.entries[i].name}')
            try:
                x = int(input())
                entr = self.entries[x-1]
                scr.change(entr.name)
            except:
                print("mauvaise entrée")

    def add_entry(self, entry):
        """
        add_entry methode permettant d'ajouter une entrée possible

        Arguments:
            entry(Entry) -- entrée a ajouter
        """
        self.entries.append(entry)


class Screen:
    """
    Screen: classe permettant l'affichage d'un display(entry ou menu) et sa mise à jour

    Attributes:
        curr_d(Entry/Menu) -- permet le stockage du display courant a rendre dans la console
        display(dictionnaire -> str:Entry/Menu) -- dictionnaire permettant de stockage de tout les display possible
    """

    def __init__(self):
        self.curr_d = None

        self.display = {}
        self.clear()


    def render(self):
        """
        render: methode de rendu en effacant la console et en rendant le display courant

        """
        self.clear()
        self.curr_d.update()

    def clear(self):
        if os.name=="posix":
            system('clear')
        else:
            system('cls')

    def add_display(self, new):
        """
        add_display: ajoute un display dans le dictionnaire


        Arguments:
            new (Entry/Menu) -- display à ajouter
        """
        self.display[new.name] = new

    def get_display(self, name) -> int:
        """
        get_display: recupère un display dans le dict par son nom

        Arguments:
            name (str) -- nom du display à obtenir

        Returns:
            int/Entry/Menu -- -1 si erreur sinon le display correspondant
        """
        x = self.display.get(name)
        if x != None:
            return x
        return -1

    def change(self, name):
        """
        change: change le display courant par son nom

        Arguments:
            name (str) -- nom du nouveau display
        """
        n = self.get_display(name)
        if n != -1:
            self.curr_d = n


# callbacks


def key_press(key):
    global current_key
    current_key = str(key).replace("'", "").replace("Key.", "")


def key_release(key):
    pass


def main():
    while True:
        scr.render()
        time.sleep(0.3)

def get_commands():
    while True:
        print("dqdqdq")
        m=bridge.get_message()
        print(m)
        


# main
e_clav_cmd = Entry("commandes", clavier_cmd)
e_exit=Entry("quitter",exit_cmd)

m_main = Menu("main")
m_main.add_entry(e_clav_cmd)
m_main.add_entry(e_exit)

# screen setup
scr = Screen()
scr.add_display(m_main)
scr.add_display(e_clav_cmd)
scr.add_display(e_exit)
scr.curr_d = m_main

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
thr_m=threading.Thread(target=main)
thr_m.start()

thr_r=threading.Thread(target=get_commands)
thr_r.start()

