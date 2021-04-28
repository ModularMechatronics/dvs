from PyQt5.QtWidgets import * 
from PyQt5 import QtCore, QtGui
from PyQt5.QtGui import * 
from PyQt5.QtCore import * 
from PyQt5.QtCore import QObject, QThread, pyqtSignal
import sys

from datetime import datetime
import subprocess
import xml.etree.ElementTree as ET
import sys
import json
import os
import importlib
from test_runner import ManualTestCase

sys.path.append('./')

executable_location = "C:\\mcdhub_git\\software\\components\\build\\Debug\\MCDTool.exe"

class TestRunner:
    def __init__(self):
        if os.getcwd().split('\\')[-1] != 'manualTesting':
            raise Exception('Script must be executed from systemtest\\manualTesting')

        self._test_files = sorted([tf for tf in os.listdir("./test_scripts") if tf.find('test_') != -1])

        self._current_test_case_name = "<name not set>"
        self._current_test_case_description = "<description not set>"

        self._test_results = {}
        self._test_cases = []

        self._test_is_running = False

        self.setup_test_cases()

    def setup_test_cases(self):

        for test_file in self._test_files:

            if test_file.find('test_') == -1:
                continue

            file_name = test_file.split(".")[0]
            exec('from test_scripts import ' + file_name + ' as test_module', globals())

            test_case_class_matches = [n for n in test_module.__dir__() if n.startswith('Test')]

            if len(test_case_class_matches) == 0:
                raise Exception(f"Couldn't find any classes with name pattern 'Test' classes in test script '{test_file}'")

            for test_class_name in test_case_class_matches:
                exec(f'test_case_class = test_module.{test_class_name}', globals())

                if not issubclass(test_case_class, ManualTestCase):
                    raise Exception(f"Test case class '{test_case_class}' from '{test_file}' is not a subclass of ManualTestCase, which is a requirement!")
                tc = test_case_class(exe_path=executable_location)

                self._test_cases.append(tc)

    def execute_next_test_case(self):
        self._test_is_running = True
        if len(self._test_cases) == 0:
            return

        tc = self._test_cases.pop(0)

        self._current_test_case_name = tc.get_test_case_name()

        tc_methods = tc.__dir__()

        self._current_test_case_description = tc.get_test_description()

        if 'setUp' in tc_methods:
            tc.setUp()

        tc.run()

        if 'tearDown' in tc_methods:
            tc.tearDown()

        self._test_is_running = False

    def is_test_case_running(self):
        return self._test_is_running

    def current_test_case_name(self):
        return self._test_cases[0].get_test_case_name()

    def current_test_case_description(self):
        return self._test_cases[0].get_test_description()

    def set_results_of_current_test(self, res):
        self._test_results[self._current_test_case_name] = res

    def tests_finished(self):
        return len(self._test_cases) == 0

    def save_to_json(self):
        time_str = str(datetime.now()).replace('-', '_').replace(' ', '__').replace(':', '_').split('.')[0]

        if not os.path.exists('./test_results'):
            os.makedirs('./test_results')
        file_name = './test_results/run_' + time_str + '.json'
        with open(file_name, "w") as fp:
            json.dump(self._test_results, fp, indent=4)


test_rnr = TestRunner()


class Worker(QObject):
    finished = pyqtSignal()

    def run(self):
        """for i in range(5):
            sleep(1)
            self.progress.emit(i + 1)"""
        test_rnr.execute_next_test_case()
        self.finished.emit()


class ScrollLabel(QScrollArea):

    def __init__(self, *args, **kwargs):
        QScrollArea.__init__(self, *args, **kwargs)

        self.setWidgetResizable(True)
        content = QWidget(self)

        self.setWidget(content)
        lay = QVBoxLayout(content)

        self.label = QLabel(content)
        self.label.setWordWrap(True)

        lay.addWidget(self.label)
  
    def set_text(self, text):
        self.label.setText(text)


class Window(QMainWindow):

    def __init__(self):
        super().__init__()

        self.setWindowTitle("Manual tests")
        self.setGeometry(10, 50, 600, 550)

        self.setup_ui()

        self.show()
        
    def setup_ui(self):
        # Setup text fields
        self.test_title = ScrollLabel(self)
        self.test_description = ScrollLabel(self)

        self.test_title.label.setFont(QtGui.QFont("Times", 20, QtGui.QFont.Bold))
        self.test_title.label.setAlignment(Qt.AlignCenter | Qt.AlignTop)

        self.test_description.label.setAlignment(Qt.AlignLeft | Qt.AlignTop)

        self.test_description.set_text("")
        self.test_title.set_text("<No test loaded>")

        self.test_title.setGeometry(10, 10, 580, 55)
        self.test_description.setGeometry(10, 70, 580, 400)

        # Setup buttons
        self.button_passed = QPushButton('Test passed', self)
        self.button_passed.setGeometry(500, 500, 90, 30)
        self.button_passed.clicked.connect(self.handle_button_test_passed)
        self.button_passed.setIcon(QIcon('check.png'))
        self.button_passed.hide()

        self.button_failed = QPushButton('Test failed', self)
        self.button_failed.setGeometry(10, 500, 90, 30)
        self.button_failed.clicked.connect(self.handle_button_test_failed)
        self.button_failed.setIcon(QIcon('cross.png'))
        self.button_failed.hide()

        self.button_start = QPushButton('Start tests', self)
        self.button_start.setGeometry(355, 500, 90, 30)
        self.button_start.clicked.connect(self.handle_button_start)

    def handle_button_start(self):
        self.button_start.hide()
        self.button_passed.show()
        self.button_failed.show()

        self.next_test()

    def handle_button_test_passed(self):
        if test_rnr.is_test_case_running():
            print("Test is still running! Exit MCDTool to continue with the next test...")
        else:
            test_rnr.set_results_of_current_test("Passed")
            self.next_test()

    def handle_button_test_failed(self):
        if test_rnr.is_test_case_running():
            print("Test is still running! Exit MCDTool to continue with the next test...")
        else:
            test_rnr.set_results_of_current_test("Failed")
            self.next_test()

    def next_test(self):
        if test_rnr.tests_finished():
            test_rnr.save_to_json()
            sys.exit(0)

        self.test_title.set_text(test_rnr.current_test_case_name())
        self.test_description.set_text(test_rnr.current_test_case_description())

        self.thread = QThread()
        self.worker = Worker()
        self.worker.moveToThread(self.thread)
        self.thread.started.connect(self.worker.run)
        self.worker.finished.connect(self.thread.quit)
        self.worker.finished.connect(self.worker.deleteLater)
        self.thread.finished.connect(self.thread.deleteLater)
        self.thread.start()


if __name__ == "__main__":

    App = QApplication(sys.argv)

    window = Window()

    window.show()

    sys.exit(App.exec())
