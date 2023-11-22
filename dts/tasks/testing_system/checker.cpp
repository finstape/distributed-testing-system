#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>
#include <atomic>
#include <thread>

// {verdict, test, max_time_used, max_memory_used}
std::vector <std::string> result(4);

void return_result() {
    std::cout << result[0] << " " << result[1] << " " << result[2] << " " << result[3];
}

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
        result[0] = "RE";   // вычисление RE неподтверждено
        result[1] = std::to_string(test_index);
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

int main(int argc, char** argv) {
    // Имя файла для чтения
    std::string filename = argv[1];
    // std::cout << filename << std::endl;

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
    task_name = argv[2];

    // std::cout << task_name << std::endl;

    // Считаем количество тестов и получаем ограничения
    int count_tests = 0;
    get_tests_number(count_tests, task_name);
    if (count_tests == 0) {
        std::cout << "Incorrect task name" << std::endl;
        exit_program(2);
    }

    // время в секундах, память в мегабайтах
    double time_limit = 2.0, memory_limit = 1280.0;

    // Компилируем
    int compile_status = system("g++ -O2 temp_source.cpp -o temp_program");
    if (compile_status != 0) {
        result[0] = "CE";
        result[1] = "1";
        result[2] = "0";
        result[3] = "0";
        return_result();
        exit_program(1);
    }

    for (int i = 1; i <= count_tests; i++) {
        std::string task_input = task_name + "/" + std::to_string(i) + ".in";

        // Меняем номер теста в результате
        result[1] = std::to_string(i);

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
            result[0] = "TL";
            result[2] = "2000";
            return_result();
            exit_program(2);
        }

        // Проверка на TL и ML
        double time_usage, memory_usage;
        get_runtime_info(time_usage, memory_usage);
        if (time_usage > time_limit || memory_usage > memory_limit) {
            if (time_usage > time_limit) {
                result[0] = "TL";
                result[2] = std::to_string(time_limit);
                result[3] = std::to_string(memory_usage);
            } else {
                result[0] = "ML";
                result[2] = std::to_string(time_usage);
                result[3] = std::to_string(memory_limit);
            }
            return_result();
            exit_program(2);
        }

        result[2] = std::to_string(std::max(std::stod(result[2]), time_usage));
        result[3] = std::to_string(std::max(std::stod(result[3]), memory_usage));

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
            result[0] = "PE";
            result[2] = std::to_string(time_usage);
            result[3] = std::to_string(memory_usage);
            return_result();
            exit_program(2);
        }

        for (int j = 0; j < correct_data.size(); j++) {
            if (correct_data[j] != temp_data[j]) {
                is_OK = false;
                break;
            }
        }
        if (!is_OK) {
            result[0] = "WA";
            result[2] = std::to_string(time_usage);
            result[3] = std::to_string(memory_usage);
            return_result();
            exit_program(2);
        }
    }

    result[0] = "OK";
    return_result();
    exit_program(0);
}