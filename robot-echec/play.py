from robotarm import Joint
import pyfirmata
from utils import *
from serial.serialutil import SerialException
from constantes import *
import keyboard

import os
import time
import threading
import sys
            

"""GUI
"""
    
def clear():
    if os.name=="posix":
        os.system("clear")
    else:
        os.system("cls")
        
class PlateauView:
    def __init__(self):
        self.title="plateau"
    def print_header(self):
        print(self.title)
        print("a pour afficher le plateau courant")
        print("c pour modifier une position courante d'une case specifique")
        print("s pour sauvegarder tout le plateeau")
        print("l pour charger la grille sauvegardé")    
        print("echap pour revenir en arriere")
        
    def update(self):
        
        if keyboard.is_pressed('escape'):
            app.change_view("main")
        if keyboard.is_pressed('s'):
            plateau.save_grid()
        if keyboard.is_pressed('a'):
            print("\n")
            print(plateau)
        if keyboard.is_pressed('c'):
            x=input("\n"+"valeur de la case en x").upper()[-1]
            y=input("\n"+"valeur de la case en y").upper()[-1]
            if not plateau.is_valid_pos(x,y):
                print("cette case n'existe pas")
            else:
                plateau.update_case(x,y)
        if keyboard.is_pressed('l'):
            pass
            plateau.load_grid()
                
            



class DirectView:
    def __init__(self):
        self.title="direct"
    def print_header(self):
        print(self.title)
        print("zqsd pour la base et l'epaule,les touches t et g pour le coude,les touches y et h pour le poignet")
        print("echap pour revenir en arriere")
        
    def update(self):

        if keyboard.is_pressed('r'):
            robot.returnToStart()
        
        if keyboard.is_pressed('z'):
            robot.shoulder.addingWrite(DIRECT_STEP)
        if keyboard.is_pressed('s'):
            robot.shoulder.addingWrite(-DIRECT_STEP)
        if keyboard.is_pressed('q'):
            robot.base.addingWrite(-DIRECT_STEP)
        if keyboard.is_pressed('d'):
            robot.base.addingWrite(DIRECT_STEP)
        
        if keyboard.is_pressed('t'):
            robot.elbow.addingWrite(DIRECT_STEP)
        if keyboard.is_pressed('g'):
            robot.elbow.addingWrite(-DIRECT_STEP)
            
        if keyboard.is_pressed('y'):
            robot.wrist.addingWrite(DIRECT_STEP)
        if keyboard.is_pressed('h'):
            robot.wrist.addingWrite(-DIRECT_STEP)
        
        if keyboard.is_pressed('escape'):
            app.change_view("main")


class MainView:
    def __init__(self):
        self.title="main"
        self.choices={"1":"direct","2":"plateau"}
    def print_header(self):
        print(self.title)
        print("1.Piloter le robot en mode direct")
        print("2.Gestion du plateau")
        

    def update(self):
        sys.stdin.flush()
        select=input(">")
        try:
            choice=self.choices[select]
            if choice!=None:
                app.change_view(choice)
        except:
            print("veuiller saisir un choix valide")
        


class Application:
    
    def __init__(self):
        self.views=[]
        self.curr_view=None

    def add_view(self,view):
        if view not in self.views:
            self.views.append(view)
            
    def change_view(self,name):
        for v in self.views:
            if v.title==name:
                self.curr_view=v
                clear()
                v.print_header()
                return
        print("didnt find "+name)
        
    def update(self):
        time.sleep(0.1)
        if self.curr_view!=None:
            self.curr_view.update()
        else:
            print("Aucune vue")


"""ROBOT
"""
"""
!!!n'a pas été testé car pas le robot sous la main...!!!
"""
class RobotArm:
    def __init__(self, board, pins=[ARM_BASE,ARM_SHOULDER,ARM_ELBOW,ARM_WRIST,ARM_GRIPPER], startAngles=[BASE_START,SHOULDER_START,ELBOW_START,WRIST_START,GRIPPER_START]):
        self.base = Joint(board, pins[0], startAngles[0],"base")
        self.shoulder=Joint(board, pins[1], startAngles[1],"shoulder")
        self.elbow=Joint(board, pins[2], startAngles[2],"elbow")
        self.wrist=Joint(board, pins[3], startAngles[3],"wrist")        
        self.gripper=Joint(board,pins[-1],startAngles[-1],"gripper")
        
        self.startAngles = startAngles

    def returnToStart(self):
        """
        returnToStart permet de retourner à la position de base pour tout les servos
        """
        thr=threading.Thread(target=self.moveTo,args=([self.startAngles]))
        thr.start()

    def setTarget(self, targets):
        """
        setTarget permet de mettre a jour les objectif de tout les joints
        Arguments:
            targets {int [0-180][]} -- tableau de positions finales
        """
        self.base.setTarget(targets[0])
        self.shoulder.setTarget(targets[1])
        self.elbow.setTarget(targets[2])
        self.wrist.setTarget(targets[3])
        self.gripper.setTarget(targets[-1])

    def atTargets(self):
        """
        atTargets retourne vrai si le bras est totalement positioné

        Return:
            bool -- etat du bras robotisé
        """
        return self.base.haveFinished() and self.shoulder.haveFinished() and self.elbow.haveFinished() and self.wrist.haveFinished()and self.gripper.haveFinished()

    def step(self):
        """
        step bouge tout les joints d'une unité vers leur objectif

        """
        self.base.moveToTarget()
        self.shoulder.moveToTarget()
        self.elbow.moveToTarget()
        self.wrist.moveToTarget()
        self.gripper.moveToTarget()
        
    def moveTo(self, targets, delayMs=50):
        """
        moveTo bouger le bras complet de maniere propre avec un delay

        Arguments:
            targets {int[]} -- tableau de positions final

        Keyword Arguments:
            delayMs {int} --  delay entre tout les mouvemebts (default: {50})
        """

        self.setTarget(targets)
        while not self.atTargets():
            self.step()
            time.sleep(delayMs/1000)
    def getPos(self):
        """getPos recuperer toute les positions des servos sous forme d'une liste

        Returns:
            [int[]]: positions des servos
        """
        return [self.base.angle,self.shoulder.angle,self.elbow.angle,self.wrist.angle,self.gripper.angle]


class Position:
    def __init__(self, x, y, servos):
        """
        represente ce qui va etre propre a chaque case du plateau pour permettre aux moteurs de si diriger
        """
        self.x = x
        self.y = y
        self.servos = servos

    def __repr__(self) -> str:
        #retourne la representation textuelle si on veut l'afficher dans la console ou le sauvegarder
        return f'|{self.x}{self.y}:{self.servos}|'


class Plateau:
    def __init__(self, taille):
        """
        represente le plateau de jeu contenant une matrice d'objets Position(pour permettre de sauvegarder les positions moteur pour chaque case par la suite)
        """
        self.taille = taille
        self.grille = []
        for i in range(taille, 0, -1):
            ligne = []
            for j in range(taille):
                #on recupere dans la table ascii la lettre en majuscule et on initialise un tableau de positions servo
                pos = Position(chr(65+j), str(i), [0]*SERVO_COUNT)
                ligne.append(pos)
            self.grille.append(ligne)

    def get_pos_index_by_case(self, x, y):
        """permet de recuperer la ligne et la colonne a partir d'une case echec (ex A8)

        Args:
            x (str[a-z]): ligne 
            y (str(1-8)): colonne

        Returns:
            (int,int): tuple des index
        """
        #on parcoure notre grille
        for i in range(self.taille):
            for j in range(self.taille):
                case = self.grille[i][j]
                #si les positions de la case sont bonne
                if case.x == x and case.y == y:
                    #on le tuple avec les index
                    
                    return (i,j)
        return None
    
    
    def save_grid(self):
        """permet de sauvegarder la grille sous un fichier
        """
        #on efface le fichier
        fo=open("grid.txt","w")
        fo.write("")
        fo.close()
        #on ouvre le fichier d'une maniere securisé
        with open("grid.txt","a") as file:
            #pour chaque ligne
            for i in self.grille:
                #tableau des positions 
                servos=[]
                #pour chaque colonne
                for j in i:
                    #on met tout en string
                    j=[str(pos) for pos in j.servos]
                    #on joint toute les positions avec un espace
                    servos.append(" ".join(j))
                #on ecrit en separant chaque case avec un ;
                file.write(";".join(servos)+"\n")
        print("\n"+"grille sauvegardée")
    
    def load_grid(self):
        #on ouvre le fichier en lecture
        with open("grid.txt","r") as file:
            #on parcoure d'une maniere double l'index de la ligne et cette ligne la(le zip permet cela)
            for i,ligne in zip(range(self.taille),file.readlines()):
                #de la meme maniere pour l'index de la colonne et la colonne(on separe la ligne sous differentes listes)
                for j,servos in zip(range(self.taille),ligne.strip().split(";")):
                    #on met toute les positions en int
                    servos=[int(pos) for pos in servos.split()]
                    #on met a jour la case
                    self.grille[i][j].servos=servos
        print("loading grid")
        
            

    def update_case(self,x,y):
        """permet de mettre a jour les positions moteur d'une case

         Args:
            x (str[a-z]): ligne 
            y (str(1-8)): colonne
        """
        #on recupere les index
        i,j=self.get_pos_index_by_case(x,y)
        #on recupere les nouvelles pos
        newPos=robot.getPos()
        #on met a jour 
        self.grille[i][j].servos=newPos
        print(f'{x}{y} est mtn égale à {newPos}')
    
    def is_valid_pos(self,x,y):
        """permet de savoir si une case echec est valide dans ce cas si

          Args:
            x (str[a-z]): ligne 
            y (str(1-8)): colonne

        Returns:
            [bool]: validité de la case
        """
        try:
            #si la case dans le tableau existe, aucune exception sera levée
            i,j =self.get_pos_index_by_case(x,y)
            x=self.grille[i][j]
            #on retourne donc vrai
            if x:return True
        except:
            return False
        
    
    def __repr__(self) -> str:
        f=""
        for i in self.grille:
            for j in i:
                f+=j.__repr__()
            f+="\n"
        return f

def boucle_principale():
    while True:
        app.update()

if __name__ == '__main__':

    #connexion avec l'arduino
    try:
        
        board = pyfirmata.Arduino(ARDUINO_LINUX_PORT)
        print("connection sucess")
    except SerialException:
        print("erreur de port")
        exit(1)
    #definition du thread servant a la gestion des evenement venant de l'arduino dans un thread separé
    iter8 = pyfirmata.util.Iterator(board)
    iter8.start()

    #demarrage de la gui
    app=Application()
    v_main=MainView()
    v_direct=DirectView()
    v_plateau=PlateauView()
    
    app.add_view(v_main)
    app.add_view(v_direct)
    app.add_view(v_plateau)
    app.change_view("main")

    robot = RobotArm(board)
    plateau=Plateau(8)
    
    #on met notre boucle principale sous un thread pour que son appel ne soit pas bloquant
    thread=threading.Thread(target=boucle_principale)
    #on le demarre
    thread.start()


    
   
