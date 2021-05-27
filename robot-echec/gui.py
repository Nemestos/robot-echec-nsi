from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt
from PyQt5.QtGui import *
import sys
def get_qt_inst():

    app = QApplication.instance()
    #si l'instance de l'application n'a pas encore été crée
    if not app:
        #on crée un nouvelle objet QApplication(base de toute application Qt)
        app = QApplication(sys.argv)
    return app

class Window(QWidget):
    def __init__(self,width,heigt):
        #on appelle le constructure de la classe mère
        super().__init__()
        #layout
        layout =QHBoxLayout()

        self.setFixedSize(width,heigt)
        self.setWindowTitle("Robot echec")

        self.title=QLabel("Controlleur du bras",self)
        self.title.setStyleSheet("border: 1px solid black; font-weight:bold")
        self.title.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.title)

        self.author=QLabel("Controlleur du bras",self)
        self.author.setStyleSheet("border: 1px solid black; font-weight:bold")
        self.author.setAlignment(Qt.AlignRight)
        layout.addWidget(self.author)


        self.btn=QPushButton("EXEMPLE BOUTON")

        layout.setAlignment(Qt.AlignTop)
        self.setLayout(layout)



    


