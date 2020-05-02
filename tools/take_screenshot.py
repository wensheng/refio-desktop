import sys

from PySide2.QtCore import Qt, QCoreApplication, QUrl
from PySide2.QtWidgets import QApplication
from PySide2.QtGui import QPainter, QImage
from PySide2.QtWebEngineWidgets import QWebEngineView, QWebEngineSettings
from functools import partial



class Screenshot(QWebEngineView):
    def __init__(self):
        QWebEngineView.__init__(self)

    def capture(self, url, output_file):
        self.load(QUrl(url))
        #self.setAttribute(Qt.WA_DontShowOnScreen)
        self.resize(1280, 2000)
        self.show()
        self.page().loadFinished.connect(partial(self.onDone, output_file))

    def onDone(self, output_file, loadOK):
        if not loadOK:
            print("Could not load page")
            return
        self.page().settings().setAttribute(QWebEngineSettings.ScreenCaptureEnabled, True)
        self.grab().save("page.png")
        QCoreApplication.instance().quit()


if __name__ == "__main__":
    app = QApplication([])
    s = Screenshot()
    s.capture('https://doc.qt.io/qtforpython/PySide2/QtWebEngineWidgets/QWebEngineView.html',
          'C:/Users/wensheng/Pictures/web_page.png')
    sys.exit(app.exec_())
