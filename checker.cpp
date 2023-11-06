#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>
#include <atomic>
#include <thread>

void exit_program() {
    system("rm temp_source.cpp");
    system("rm temp_program");
    system("rm temp_output.txt");
    system("rm temp_count.txt");
}

void get_tests_number(int &count_tests, std::string &task_name) {
    system("touch temp_count.txt");
    std::ifstream count_file("temp_count.txt");
    system(("ls " + task_name + " | grep in | wc -l > temp_count.txt").c_str());
    count_file >> count_tests;
    count_file.close();
}

void run_source(std::string &task_input, int &test_index) {
    std::string max_runtime_awake = "10s";
    // usr/bin/time -f "&U %M" bash -c 'cat | ./source > temp_output.txt' > temp_runtime_info.txt
    int run_status = system(("cat " + task_input + " | timeout " + max_runtime_awake + " ./temp_program > temp_output.txt").c_str());
    if (run_status != 0) {
        std::cout << "RE " << test_index << std::endl;
        exit_program();
        exit(1);
    }
}

int main() {
    // Имя файла для чтения
    std::string filename = "source.cpp";

    // Чтение содержимого файла
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Error opening file \"" << filename << "\"" << std::endl;
        return 1;
    }

    std::string code((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Сохраняем исходный код во временный файл
    std::ofstream outputFile("temp_source.cpp");
    outputFile << code;
    outputFile.close();

    // Компилируем
    int compile_status = system("g++ -O2 temp_source.cpp -o temp_program");
    if (compile_status != 0) {
        std::cerr << "CE" << std::endl;
        exit_program();
        return 1;
    }

    std::string task_name;
    std::cout << "Task name: ";
    std::cin >> task_name;

    int count_tests = 0;
    get_tests_number(count_tests, task_name);
    if (count_tests == 0) {
        std::cout << "Incorrect task name" << std::endl;
        exit_program();
        return 2;
    }

    bool final_verdict = true;
    for (int i = 1; i <= count_tests; i++) {
        std::string task_input = task_name + "/" + std::to_string(i) + ".in";

        // Запускаем
        std::atomic<bool> is_done(false);
        std::thread t([&is_done, &task_input, &i]{
            run_source(task_input, i);
            is_done = true;
        });

        // Проверка на то, что программа завершится в адекватное время (step в милисекундах)
        for (size_t step = 10; step <= 2000; step += 10) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (is_done) {
                t.join();
                break;
            }
        }

        if (!is_done) {
            t.detach();
            std::cout << "TL " << i << std::endl;
            exit_program();
            exit(2);
        }

        std::vector<std::string> correct_data, temp_data;
        std::string temp_str;

        std::string str;

        std::ifstream correct_output;
        str = task_name +"/" + std::to_string(i) + ".out";
        correct_output.open(str);
        while (getline(correct_output, temp_str)) {
            std::string::iterator it = std::remove(temp_str.begin(), temp_str.end(), '\r');
            temp_str.erase(it, temp_str.end());
            correct_data.push_back(temp_str);
        }
        correct_output.close();

        std::ifstream source_output;
        source_output.open("temp_output.txt");
        while (getline(source_output, temp_str)) {
            temp_data.push_back(temp_str);
        }
        source_output.close();
        bool is_OK = true;
        if (correct_data.size() != temp_data.size()) {
            std::cout << "PE " << i << std::endl;
            final_verdict = false;
            break;
        }

        for (int j = 0; j < correct_data.size(); j++) {
            if (correct_data[j] != temp_data[j]) {
                is_OK = false;
                break;
            }
        }
        if (!is_OK) {
            std::cout << "WA " << i << std::endl;
            final_verdict = false;
            break;
        }
    }

    if (final_verdict) std::cout << "OK" << std::endl;
    exit_program();
    return 0;
}
