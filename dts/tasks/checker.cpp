#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>
#include <atomic>
#include <thread>

void exit_program(int code) {
    // delete temp_source.cpp temp_program temp_output.txt temp_count.txt temp_runtime_info.txt
    system("rm temp*");
    exit(code);
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
    system("touch temp_runtime_info.txt");
    int run_status = system(("/usr/bin/time -o temp_runtime_info.txt -f \"%U %M\" bash -c 'cat " + task_input + " | timeout " + max_runtime_awake + " ./temp_program > temp_output.txt'").c_str());
    if (run_status != 0) {
        std::cout << "RE " << test_index << std::endl;   // вычисление RE неподтверждено
        exit_program(1);
    }
}

void get_runtime_info(double &time_usage, double &memory_usage) {
    std::ifstream runtime_info;
    runtime_info.open("temp_runtime_info.txt");
    runtime_info >> time_usage >> memory_usage;
    runtime_info.close();
    memory_usage = memory_usage/1024.0;
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

    // Получаем имя задачи
    std::string task_name;
    std::cout << "Task name: ";
    std::cin >> task_name;

    // Считаем количество тестов и получаем ограничения
    int count_tests = 0;
    get_tests_number(count_tests, task_name);
    if (count_tests == 0) {
        std::cout << "Incorrect task name" << std::endl;
        exit_program(2);
    }

    // время в секундах, память в мегабайтах
    double time_limit = 2.0, memory_limit = 128.0;

    // Компилируем
    int compile_status = system("g++ -O2 temp_source.cpp -o temp_program");
    if (compile_status != 0) {
        std::cerr << "CE" << std::endl;
        exit_program(1);
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

        // Проверка на то, что программа завершится в адекватное время (2 секунды, проверяем каждые 10 мс)
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
            exit_program(2);
        }

        // Проверка на TL и ML
        double time_usage, memory_usage;
        get_runtime_info(time_usage, memory_usage);
        if (time_usage > time_limit || memory_usage > memory_limit) {
            if (time_usage > time_limit) {
                std::cout << "TL " << i << std::endl;
            } else {
                std::cout << "ML " << i << std::endl;
            }
            exit_program(2);
        }

        std::vector<std::string> correct_data, temp_data;
        std::string temp_str;

        std::ifstream correct_output;
        correct_output.open(task_name +"/" + std::to_string(i) + ".out");
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
            exit_program(2);
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
    else std::cout << "Checker error" << std::endl;
    exit_program(0);
}