import os
import subprocess

# print informatin header before running test
def printTestInfoHeader(test_name):
    print("")
    print("############################")
    print("[TEST]     "+test_name)
    print("############################")
    print("")

# get path of this script
file_path = os.path.dirname(os.path.realpath(__file__))

# get path to build-directory
build_dir = file_path + "/../builds/build_native"

# check if path exists
if not os.path.isdir(build_dir):
    print("Build directory '" + build_dir + "does not exist.")
    exit(1)

printTestInfoHeader("Framework")
return_code = subprocess.call(build_dir + "/framework_test")

printTestInfoHeader("Gateway")
return_code = subprocess.call(build_dir + "/gateway_test")

# exit script depending on test-result with error or not
if return_code == 1:
    exit(1)
else:
    exit(0)
