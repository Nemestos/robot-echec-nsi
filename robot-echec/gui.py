from PyQt5.QtWidgets import QApplication, QWidget
import sys
def get_qt_inst():

    app = QApplication.instance()
    #si l'instance de l'application n'a pas encore été crée
    if not app:
        #on crée un nouvelle objet QApplication(base de toute application Qt)
        app = QApplication(sys.argv)

class MainWindow(QWidget):
    def __init__(self):
        QWidget.__init__(self)
        self.setWindowTitle("Robot echec")


