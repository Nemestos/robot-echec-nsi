import pyfirmata


class Joint:
    def __init__(self, board: pyfirmata.Arduino, pin, start,name):
        """
        __init__ crée un composant du bras controllable individuellement

        Arguments:
            name{str} -- nom du joint
            board {pyfirmata.Arduino} -- instance a la carte arduino
            pin {int} -- le pin où le servo est connecté
            start {int} -- la position initiale du servo [0-180]
        """
        board.digital[pin].mode = pyfirmata.SERVO
        self.name=name
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
        return self.target==None or int(self.angle)== int(self.target)

    def moveToTarget(self):
        """
        moveToTarget bouge a la finalité en fonction d'un pas
        """

        self.angle += self.stepsize*(-1 if self.target-self.angle < 0 else 1)
        print(f'"\n"{self.name}:{self.angle}')
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
    
    def addingWrite(self,value):
        """addingWrite permet d'ajouter la valeur a la precedente 
        et l'ecrire sur le moteur

        Args:
            value ([int]): valeur ajoute

        Returns:
            [type]: [description]
        """
        self.target=None
        self.angle+=value
        self.angle=max(0,min(self.angle,180))
        print(f'"\n"{self.name}:{self.angle}')
        self.servo.write(self.angle)

