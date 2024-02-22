import json
import os
import subprocess
import sys


def getEchoExecPath():
    if len(sys.argv) != 2:
        raise Exception("Echo executable not provided")
    echoExec = sys.argv[1]
    if not os.path.exists(echoExec):
        raise Exception("Echo executable does not exist")
    return echoExec


class EchoTest:
    def __init__(self, echoExecPath):
        self.echoExecPath = echoExecPath
        self.testsData = self.__getTestsData()

    def __getTestsData(self):
        with open('echoTestData.json') as data:
            return json.load(data)

    def __runTest(self, args, env, expectedJson):
        result = subprocess.run([self.echoExecPath, *args], env=env, capture_output=True, text=True)
        if result.returncode:
            raise Exception(f"test returned non zero code {result.returncode}")
        formatedOutput = json.dumps(json.loads(result.stdout))
        formatedExpected = json.dumps(expectedJson)
        if formatedExpected != formatedOutput:
            raise Exception(f"result of running test: '{formatedOutput}' does not match expected: '{formatedExpected}'")

    def __runTestAndSummarize(self, testData):
        args = testData["args"]
        env = testData["env"]
        expectedJson = testData["expected"]
        try:
            self.__runTest(args, env, expectedJson)
            print(f"Test for args: {args}, env: {env} succeeded")
            return True
        except Exception as e:
            print(f"Test for args: {args}, env: {env} failed: {e}")
        return False

    def run(self):
        allTests = len(self.testsData)
        succeededTests = 0
        for count, testData in enumerate(self.testsData, start=1):
            print(f"Test {count}/{allTests}")
            succeededTests += self.__runTestAndSummarize(testData)
        if succeededTests == allTests:
            print(f"All test succeeded: {succeededTests}/{allTests}")
        else:
            raise Exception(f"Some tests failed: {allTests - succeededTests}/{allTests}")


if __name__ == "__main__":
    EchoTest(getEchoExecPath()).run()
