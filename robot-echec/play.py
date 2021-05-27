from robotarm import Joint
import pyfirmata
from utils import *
from serial.serialutil import SerialException
from constantes import *
import time
import gui

class RobotArm:
    def __init__(self, board, pins=[ARM_BASE], startAngles=[BASE_START]):
        self.base = Joint(board, pins[0], startAngles[0])

        self.startAngles = startAngles

    def returnToStart(self):
        """
        returnToStart permet de retourner à la position de base
        """

    def setTarget(self, targets):
        """
        setTarget permet de mettre a jour les objectif de tout les joints
        Arguments:
            targets {int [0-180][]} -- tableau de positions finales
        """
        self.base.setTarget(targets[0])

    def atTargets(self):
        """
        atTargets retourne vrai si le bras est totalement positioné

        Return:
            bool -- etat du bras robotisé
        """
        return self.base.haveFinished()

    def step(self):
        """
        step bouge tout les joints d'une unité vers leur objectif

        """
        self.base.moveToTarget()

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
        return f'x:{self.x},y:{self.y},servos:{self.servos}'


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
                pos = Position(chr(65+j), i, [SERVO_DEFAULT]*SERVO_COUNT)
                ligne.append(pos)
            self.grille.append(ligne)

    def get_pos_by_case(self, x, y):
        #on parcoure notre grille
        for i in range(self.taille):
            for j in range(self.taille):

                case = self.grille[i][j]
                #si les positions de la case sont bonne
                if case.x == x and case.y == y:
                    #on renvoie l'objet Position
                    return case
        return None


if __name__ == '__main__':

    #connexion avec l'arduino
    try:
        board = pyfirmata.Arduino(ARDUINO_LINUX_PORT)
        print("connection sucesxws")
    except SerialException:
        print("erreur de port")
        exit(1)
    #definition du thread servant a la gestion des evenement venant de l'arduino dans un thread separé
    iter8 = pyfirmata.util.Iterator(board)
    iter8.start()

    #demarrage de la gui
    app=gui.get_qt_inst()
    fen=gui.MainWindow()
    robot = RobotArm(board)

    if DIRECT:

        commands_joint = {board.get_pin(f'a:{JOY_X_PIN}:p'): update_pot}
        for i in commands_joint.keys():
            i.mode = pyfirmata.INPUT
            i.enable_reporting()

        while True:
            for comm, func in commands_joint.items():
                func()
