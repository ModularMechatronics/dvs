from test_runner import *

class Test0(ManualTestCase):
    def __init__(self, **kwargs):
        init_super_class(self, **kwargs)

        self._assignment_name = "EmptyAssignment"
        self._test_description = "Check that you can do that"

    def setUp(self):
        print(f"Setup {self.__class__}")

    def tearDown(self):
        print(f"Teardown {self.__class__}")

    def run(self):
        self.open_assignment()
        print(f"Running {self.__class__}, path: {self._mcd_tool_path}")
