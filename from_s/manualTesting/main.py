import subprocess
import xml.etree.ElementTree as ET
import sys
import os
import importlib
# from PyQt5 import QApplication, QLabel, QtGui, QtCore

from test_runner import ManualTestCase

sys.path.append('./')

executable_location = "C:\\mcdhub_git\\software\\components\\build\\Debug\\MCDTool.exe"

assignment_loc = "C:\\mcdhub_git\\software\\systemtest\\Lintest.shaf"

user_settings_path = "C:\\Users\\sx28744\\AppData\\Local\\Temp\\MCD\\UserSettings\\user.config"

def find_node_in_tree(node, tag_name):
    
    for child in node:
        if child.tag == tag_name:
            return child
        else:
            return find_node_in_tree(child, tag_name)
    
    return None

def find_settings_tag_in_list():
    name = ''
    settings_node = find_node_in_tree(root, 'MCDTool.GUI.Properties.Settings')
    
    for child in settings_node:
        if child.attrib and ('name' in child.attrib.keys()) and child.attrib['name'] == 'Username':
            return child.text


if __name__ == "__main__":

    """tree = ET.parse(user_settings_path)
    root = tree.getroot()
    subprocess.run([executable_location, assignment_loc])

    user_name = find_settings_tag_in_list()"""
    
    """app = QApplication([])
    label = QLabel('Hello World!')
    label.show()

    app.exec()"""

    test_files = os.listdir("./test_scripts")
    
    if os.getcwd().split('\\')[-1] != 'manualTesting':
        raise Exception('Script must be executed from systemtest\\manualTesting')

    for tf in test_files:
        if tf.find('test_') == -1:
            continue

        file_name = tf.split(".")[0]
        exec('from test_scripts import ' + file_name + ' as test_module')

        test_case_class_matches = [n for n in test_module.__dir__() if n.startswith('Test')]

        if len(test_case_class_matches) == 0:
            raise Exception(f"Couldn't find any classes with name pattern 'Test' classes in test script '{tf}'")

        for test_class_name in test_case_class_matches:
            exec(f'test_case_class = test_module.{test_class_name}')
            
            if not issubclass(test_case_class, ManualTestCase):
                raise Exception(f"Test case class '{test_case_class}' from '{tf}' is not a subclass of ManualTestCase, which is a requirement!")

            tc = test_case_class(exe_path=executable_location)

            test_case_name = str(tc.__class__).lstrip("<class '").rstrip("'>")

            if 'testDescription' in tc.__dir__():
                test_description = tc.testDescription()
            else:
                test_description = "<No test description given>"
            
            if 'setUp' in tc.__dir__():
                tc.setUp()

            tc.run()

            if 'tearDown' in tc.__dir__():
                tc.tearDown()
