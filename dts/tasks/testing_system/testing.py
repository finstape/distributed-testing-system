import os
import subprocess

testing_system_path = os.path.join(os.path.dirname(__file__))
print(testing_system_path)
checker_root = os.path.join(testing_system_path, "checker.cpp")

def process_code_cpp(request_data):
    cpp_filename = os.path.join(testing_system_path, "source.cpp")

    with open(cpp_filename, "w") as cpp_file:
        cpp_file.write(request_data)

    try:
        compile_checker_process = subprocess.run(["g++", "-std=c++20", checker_root], capture_output=True, text=True)
        if compile_checker_process.returncode != 0:
            return None, compile_checker_process.stderr


        run_process = subprocess.run(["./a.out", cpp_filename, 'task1'], capture_output=True, text=True)
        print(run_process.stdout)
        return run_process.stdout
    finally:
        subprocess.run(["rm", "a.out"])

process_code_cpp('''#include <iostream>
using namespace std;
int main () {
int a, b;
cin >> a >> b;
cout << a + b;
}''')