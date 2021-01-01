import os
import subprocess
import sys

# get path of this script
file_path = os.path.dirname(os.path.realpath(__file__))

# get path to build-directory
build_dir = file_path + "/../builds/build_native/"

# get Modbus configuration file path
mb_config_path = file_path + "/../../resources/mbdl_config.json"

# check if path exists
if not os.path.isdir(build_dir):
    print("Build directory '" + build_dir + "does not exist.")
    sys.exit(1)

# start UI application
subprocess.call([build_dir + "/ui_application", mb_config_path])

sys.exit(0)
