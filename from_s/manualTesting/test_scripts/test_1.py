from test_runner import *

class Test1(ManualTestCase):
    def __init__(self, **kwargs):
        init_super_class(self, **kwargs)

        self._assignment_name = "EmptyAssignment"
        self._test_description = "Check that you can add a recorder"

    def setUp(self):
        print(f"Setup {self.__class__}")

    def run(self):
        self.open_assignment()
        print(f"Running {self.__class__}, path: {self._mcd_tool_path}")

class Test2(ManualTestCase):
    def __init__(self, **kwargs):
        init_super_class(self, **kwargs)
        self._assignment_name = "EmptyAssignment"
        self._test_description = "Check that you can see devices"

    def setUp(self):
        print(f"Setup {self.__class__}")

    def run(self):
        self.open_assignment()
        print(f"Running {self.__class__}, path: {self._mcd_tool_path}")
