import os
import subprocess
import sys

# get path of this script
file_path = os.path.dirname(os.path.realpath(__file__))

# get path to build-directory
build_dir = file_path + "/../builds/build_native"

# check if path exists
if not os.path.isdir(build_dir):
    print("Build directory '" + build_dir + "does not exist.")
    sys.exit(1)

# run integrationtests
return_code = subprocess.call(build_dir + "/integrationtest")

# exit script depending on test-result with error or not
sys.exit(return_code)
