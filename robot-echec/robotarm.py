import pyfirmata


class Joint:
    def __init__(self, board: pyfirmata.Arduino, pin, start):
        """
        __init__ crée un composant du bras controllable individuellement

        Arguments:
            board {pyfirmata.Arduino} -- instance a la carte arduino
            pin {int} -- le pin où le servo est connecté
            start {int} -- la position initiale du servo [0-180]
        """
        board.digital[pin].mode = pyfirmata.SERVO

        # d pour digital et s pour servo
        self.servo = board.digital[pin]
        self.angle = start
        self.servo.write(start)

        self.target = None
        self.stepsize = None

    def setTarget(self, target):
        """
        setTarget permet de mettre a jour l'objectif final en fonction d'un 
        pas qui est calculée a partir de la position initiale

        Arguments:
            target {int[0-180]} -- position finale a atteindre
        """
        self.target=target

        self.stepsize = 2 if abs(target-self.angle) > 100 else 1

    def haveFinished(self):
        """
        haveFinished retourne si le joint a atteint son objectif

        Returns:
            [bool] -- etat du mouvement
        """
        return int(self.angle)== int(self.target)

    def moveToTarget(self):
        """
        moveToTarget bouge a la finalité en fonction d'un pas
        """

        self.angle += self.stepsize*(-1 if self.target-self.angle < 0 else 1)
        self.servo.write(self.angle)

    def hardWrite(self,value):
        """
        hardWrite permet de bouger le servo de maniere directe

        Arguments:
            value {int} -- valeur du servo
        """
        self.angle=value
        self.target=None
        self.servo.write(value)


class Arm:
    def __init__(self, board: pyfirmata.Arduino, pins, startAngles):
        """
        __init__ bras possedant plusieurs joints
        Arguments:
            board {pyfirmata.Arduino} -- instance a l'arduino
            pins {int[]} -- tableau de tout les pins des joints
            startAngles {int[]} -- tableau de tout les angles de departs 
                                   de tout les joints 
        """
        self.joints=[Joint(board,pins[i],startAngles[i])for i in range(len(pins))]


    def setTargets(self,targets):
        """
        setTargets permet de mettre a jour la position de tout
        les joins composant le bras

        Arguments:
            targets {int[]} -- tableau de position pour les servo
        """

        #le zip permet de faire un for avec deux variables de deux tableaux
        for joint,target in zip(self.joints,targets):
            joint.setTarget(target)

    def allFinished(self):
        """
        allFinished retourne si tout les servos ont atteint leur position

        Returns:
            {bool} -- retourne si le bras a totalement finit
        """

        #all retourne vrai si tout est vrai dans le tableau
        # ex :all([True,True]) -> True; all([True,False])->False
        return all(joint.haveFinished() for joint in self.joints)
    
    def moveToTargets(self):
        """
        moveToTargets permet de mettre a jour la position de tout les joints

        """
        for joint in self.joints:
            joint.moveToTarget()