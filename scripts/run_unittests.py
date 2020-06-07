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

# holds the return-code of each unit-testsuite
return_codes = []

# run unittests
return_codes.append(subprocess.call(build_dir + "/framework_test"))
return_codes.append(subprocess.call(build_dir + "/gateway_test"))
return_codes.append(subprocess.call(build_dir + "/entity_test"))

# exit script depending on test-result with error or not
exit_code = 1 if 1 in return_codes else 0
sys.exit(exit_code)
