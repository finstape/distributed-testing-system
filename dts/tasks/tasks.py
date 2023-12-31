from celery import shared_task
from celery import Celery
from time import sleep
import os

import subprocess

testing_system_path = os.path.join(os.path.dirname(__file__), "testing_system")
checker_path = os.path.join(testing_system_path, "checker.cpp")
@shared_task()
def process_code_cpp(request_data, task_id):
    cpp_filename = os.path.join(testing_system_path, "source.cpp")
    task_path = os.path.join(testing_system_path, f"task{task_id}")

    with open(cpp_filename, "w") as cpp_file:
        cpp_file.write(request_data)

    try:
        compile_checker_process = subprocess.run(["g++", "-std=c++20", checker_path], capture_output=True, text=True)
        if compile_checker_process.returncode != 0:
            return None, compile_checker_process.stderr

        run_process = subprocess.run(["./a.out", cpp_filename, task_path], capture_output=True, text=True)
        print(run_process.stdout)
        return run_process.stdout
    finally:
        subprocess.run(["rm", "a.out"])

# @shared_task
# def process_code_cpp(request_data):
#     return 'yey'
