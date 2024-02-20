import json
import os
import subprocess
import sys


def getBinDir():
    return '/Users/sylwesterdawida/7bitConf/cmake-build-debug/bin'
    if len(sys.argv) != 2:
        raise Exception("binary directory not provided")
    binDir = sys.argv[1]
    if not os.path.exists(binDir):
        raise Exception("Binary directory does not exist")
    return binDir


class CliEchoTest:
    def __init__(self, binDir):
        self.binDir = binDir
        self.echoExecPath = os.path.join(self.binDir, "CliEcho" + ('.exe' if sys.platform == 'win32' else ''))
        self.testsData = self.__getTestsData()

    def __getTestsData(self):
        with open('testData.json') as data:
            return json.load(data)

    def __runTest(self, args, expectedJson):
        result = subprocess.run([self.echoExecPath, *args], capture_output=True, text=True)
        if result.returncode:
            raise Exception(f"test returned non zero code {result.returncode}")
        formatedOutput = json.dumps(json.loads(result.stdout))
        formatedExpected = json.dumps(expectedJson)
        if formatedExpected != formatedOutput:
            raise Exception(f"result of running test: '{formatedOutput}' does not match expected: '{formatedExpected}'")

    def __runTestAndSummarize(self, testData):
        args, expectedJson = testData
        try:
            self.__runTest(args, expectedJson)
            print(f"Test for args: {args} succeeded")
            return True
        except Exception as e:
            print(f"Test for args: {args} failed: {e}")
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
    CliEchoTest(getBinDir()).run()
