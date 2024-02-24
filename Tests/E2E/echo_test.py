import json
import os
import subprocess
import sys


def get_echo_exec_path():
    if len(sys.argv) != 2:
        raise Exception("Echo executable not provided")
    echo_exec = sys.argv[1]
    if not os.path.exists(echo_exec):
        raise Exception("Echo executable does not exist")
    return echo_exec


class EchoTest:
    def __init__(self, echo_exec_path):
        self.echoExecPath = echo_exec_path
        self.tests_data = self.__get_tests_data()

    @staticmethod
    def __get_tests_data():
        with open('echoTestData.json') as data:
            return json.load(data)

    def __run_test(self, args, env, expected_json):
        result = subprocess.run([self.echoExecPath, *args], env=env, capture_output=True, text=True)
        if result.returncode:
            raise Exception(f"test returned non zero code {result.returncode}")
        output = json.dumps(json.loads(result.stdout))
        expected = json.dumps(expected_json)
        if expected != output:
            raise Exception(f"result of running test: '{output}' does not match expected: '{expected}'")

    def __run_test_and_summarize(self, test_data):
        args = test_data["args"]
        env = test_data["env"]
        expected_json = test_data["expected"]
        try:
            self.__run_test(args, env, expected_json)
            print(f"Test for args: {args}, env: {env} succeeded")
            return True
        except Exception as e:
            print(f"Test for args: {args}, env: {env} failed: {e}")
        return False

    def run(self):
        all_tests = len(self.tests_data)
        succeeded_tests = 0
        for count, testData in enumerate(self.tests_data, start=1):
            print(f"Test {count}/{all_tests}")
            succeeded_tests += self.__run_test_and_summarize(testData)
        if succeeded_tests == all_tests:
            print(f"All test succeeded: {succeeded_tests}/{all_tests}")
        else:
            raise Exception(f"Some tests failed: {all_tests - succeeded_tests}/{all_tests}")


if __name__ == "__main__":
    EchoTest(get_echo_exec_path()).run()
