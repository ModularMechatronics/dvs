import subprocess
import os


def init_super_class(class_instance, **kwargs):
    super(class_instance.__class__, class_instance).__init__(**kwargs)


class ManualTestCase:
    def __init__(self, **kwargs):
        self._mcd_tool_path = kwargs['exe_path']
        self._assignment_name = "<no assignment name given>"
        self._test_description = "<no test description given>"
        self._test_case_name = str(self.__class__).lstrip("<class '").rstrip("'>").split('test_scripts.')[-1]

    def get_assignment_name(self):
        return self._assignment_name

    def get_test_description(self):
        return self._test_description

    def get_test_case_name(self):
        return self._test_case_name
    
    def open_assignment(self):
        assignment_full_name = os.path.join('assignments', self._assignment_name + '.shaf')
        subprocess.run([self._mcd_tool_path, assignment_full_name])
