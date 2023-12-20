from celery import shared_task
import os
import asyncio
from asgiref.sync import async_to_sync
import subprocess

testing_system_path = os.path.join(os.path.dirname(__file__), "testing_system")
checker_path = os.path.join(testing_system_path, "checker.cpp")


async def compile_checker(checker_path):
    process = await asyncio.create_subprocess_exec(
        "g++", "-std=c++20", checker_path,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE,
        text=True
    )
    stdout, stderr = await process.communicate()
    return process.returncode, stdout, stderr


async def run_checker(cpp_filename, task_path):
    process = await asyncio.create_subprocess_exec(
        "./a.out", cpp_filename, task_path,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE,
        text=True
    )
    stdout, stderr = await process.communicate()
    return stdout, stderr


@shared_task()
def process_code_cpp(request_data, task_id):
    cpp_filename = os.path.join(testing_system_path, "source.cpp")
    task_path = os.path.join(testing_system_path, f"task{task_id}")

    with open(cpp_filename, "w") as cpp_file:
        cpp_file.write(request_data)

    try:
        compile_code, compile_stdout, compile_stderr = async_to_sync(compile_checker)(checker_path)
        if compile_code != 0:
            return None, compile_stderr

        run_stdout, run_stderr = async_to_sync(run_checker)(cpp_filename, task_path)
        print(run_stdout)
        return run_stdout
    finally:
        subprocess.run(["rm", "a.out"])

# @shared_task
# def process_code_cpp(request_data):
#     return 'yey'
