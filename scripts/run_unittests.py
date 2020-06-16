import os
import subprocess
import sys

# colored test-results
FAILED = "\033[91mFAILED\033[00m"
PASSED = "\033[92mPASSED\033[00m"

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
rc = subprocess.call(build_dir + "/framework_test")
return_codes.append((rc, "[Framework] --> {}".format(PASSED if rc == 0 else FAILED)))
rc = subprocess.call(build_dir + "/gateway_test")
return_codes.append((rc, "[Gateway]   --> {}".format(PASSED if rc == 0 else FAILED)))
rc = subprocess.call(build_dir + "/entity_test")
return_codes.append((rc, "[Entity]    --> {}".format(PASSED if rc == 0 else FAILED)))
rc = subprocess.call(build_dir + "/utility_test")
return_codes.append((rc, "[Utility]   --> {}".format(PASSED if rc == 0 else FAILED)))

# print summarized information about all unittests at the end
print("\n====== TEST RESULTS ======")
for test_rc in return_codes:
    print("= {} =".format(test_rc[1]))
print("==========================")

# exit script depending on test-result with error or not
for (exit_code, _) in return_codes:
    if exit_code is not 0:
        sys.exit(1)

sys.exit(0)
