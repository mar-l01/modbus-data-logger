import os
import re
import subprocess
import sys

# minimal versions
OS_MIN_VERSION = {"debian" : "10", "ubuntu": "18.04"}
CMAKE_MIN_VERSION = "3.13"
GCC_MIN_VERSION = "8.3.0"

# colored test-results
ERROR = "\033[91mERROR\033[00m"
OK = "\033[92mOK\033[00m"

def save_version_numbers_in_files(target_dir):
    print("================================")
    return_codes = []

    # get os version
    with open("{}/os_version".format(target_dir), "w") as f_os:
        rc = subprocess.call(["cat", "/etc/os-release"], stdout=f_os)
        return_codes.append(rc)

    # get cmake version
    with open("{}/cmake_version".format(target_dir), "w") as f_cmake:
        rc = subprocess.call(["cmake", "--version"], stdout=f_cmake)
        return_codes.append(rc)

    # get gcc version
    with open("{}/gcc_version".format(target_dir), "w") as f_gcc:
        rc = subprocess.call(["gcc", "--version"], stdout=f_gcc)
        return_codes.append(rc)

    for exit_code in return_codes:
        if exit_code is not 0:
            print("[{}] Failed to find all required modules!".format(ERROR))
            sys.exit(1)

    print("[{}] Found all required modules!".format(OK))
    print("================================")

def check_version_numbers(target_dir):
    req_fulfilled = True

    # check os version
    with open("{}/os_version".format(target_dir), "r") as f_os:
        v_id, v_nb = find_version_string(f_os, is_os_version=True)
        is_version_sufficient = compare_version_numbers(v_nb, OS_MIN_VERSION[v_id])
        print("[{}] OS version ({} {})..".format(OK if is_version_sufficient else ERROR, v_id, v_nb))

        if not is_version_sufficient:
            req_fulfilled = False
            print("---> At least {} version {} is required!".format(v_id, OS_MIN_VERSION[v_id]))

    # check cmake version
    with open("{}/cmake_version".format(target_dir), "r") as f_cmake:
        _, v_nb = find_version_string(f_cmake)
        is_version_sufficient = compare_version_numbers(v_nb, CMAKE_MIN_VERSION)
        print("[{}] CMake version ({})..".format(OK if is_version_sufficient else ERROR, v_nb))

        if not is_version_sufficient:
            req_fulfilled = False
            print("---> At least CMake version {} is required!".format(CMAKE_MIN_VERSION))

    # check gcc version
    with open("{}/gcc_version".format(target_dir), "r") as f_gcc:
        _, v_nb = find_version_string(f_gcc)
        is_version_sufficient = compare_version_numbers(v_nb, GCC_MIN_VERSION)
        print("[{}] GCC version ({})..".format(OK if is_version_sufficient else ERROR, v_nb))

        if not is_version_sufficient:
            req_fulfilled = False
            print("---> At least GCC version {} is required!".format(GCC_MIN_VERSION))

    return req_fulfilled

def find_version_string(search_file, is_os_version=False):
    version_id = ""
    version_nb = ""
    found_version = False

    # return first occurrence of a version number if found, else None
    for line in search_file:
        if is_os_version:
            # os version does not always has a dotted version number
            if not found_version:
                version_nb = re.search(r"\d+(\.\d+)*", line)
                found_version = True
            # get os name, too
            if re.match("^ID", line):
                # version id is given as, e.g., ID=debian
                version_id = line.split("=")[1]
        else:
            # find version number (e.g. 1.2, 1.2.3 but not 2018, 1.2.)
            if not found_version:
                version_nb = re.search(r"\d+(\.\d+)+", line)
                found_version = True

    return version_id.strip(), version_nb.group()

def compare_version_numbers(version_number_source, version_number_minimal):
    # split version number at dots
    source_version = version_number_source.split('.')
    minimal_version = version_number_minimal.split('.')

    # patch source version with 0 if not of equal length
    if len(source_version) < len(minimal_version):
        for i in range(len(minimal_version) - len(source_version)):
            source_version.append('0')

    is_version_sufficient = True
    # each version part of source version has to be at least equal to required version
    for (idx, nb) in enumerate(minimal_version):
        if int(source_version[idx]) < int(nb):
            is_version_sufficient = False
            break

    return is_version_sufficient

if __name__ == "__main__":
    # get path to temporary directory
    cur_file_dir = os.path.dirname(os.path.realpath(__file__))
    tmp_dir =  cur_file_dir + "/../builds/tmp_version_check/"

    # create directory if it does not exist
    if not os.path.isdir(tmp_dir):
        os.mkdir(tmp_dir)

    save_version_numbers_in_files(tmp_dir)
    all_requirements_fulfilled = check_version_numbers(tmp_dir)
    print("================================")

    if not all_requirements_fulfilled:
        print("[{}] Not all requirements are fulfilled!".format(ERROR))
        print("================================")
        sys.exit(1)

    print("[{}] All requirements are fulfilled!".format(OK))
    print("================================")
    sys.exit(0)
