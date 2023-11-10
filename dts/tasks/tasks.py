from celery import shared_task
from time import sleep
import os


import subprocess

checker_code = os.path.join(os.path.dirname(__file__), "checker.cpp")

@shared_task
def process_code_cpp(request_data):
    cpp_filename = "temp.cpp"
    print(checker_code)
    with open(cpp_filename, "w") as cpp_file:
        cpp_file.write(request_data)

    try:
        compile_checker_process = subprocess.run(["g++", "-std=c++20", cpp_filename], capture_output=True, text=True)
        if compile_checker_process.returncode != 0:
            return None, compile_checker_process.stderr

        run_process = subprocess.run(["./a.out", cpp_filename], capture_output=True, text=True)
        print(run_process.stdout)
        return run_process.stdout
    finally:
        subprocess.run(["rm", "a.out"])