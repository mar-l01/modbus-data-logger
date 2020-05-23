import os
import subprocess

# get path of this script
file_path = os.path.dirname(os.path.realpath(__file__))

# get path to build-directory
build_dir = file_path + "/../builds/build_native"

# check if path exists
if not os.path.isdir(build_dir):
    print("Build directory '" + build_dir + "does not exist.")
    exit(1)

# run unittests
return_code = subprocess.call(build_dir + "/framework_test")
return_code = subprocess.call(build_dir + "/gateway_test")
return_code = subprocess.call(build_dir + "/entity_test")

# exit script depending on test-result with error or not
exit(return_code)
