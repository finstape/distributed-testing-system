#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>

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
    int compile_status = system("g++ temp_source.cpp -o temp_program");
    if (compile_status != 0) {
        std::cerr << "CE" << std::endl;
        return 1;
    }

    std::string task_name;
    std::cout << "Task name: ";
    std::cin >> task_name;

    for (int i = 1; i <= 52; i++) {
        std::string task_input = task_name + "/" + std::to_string(i) + ".in";

        // Запускаем
        int run_status = system(("cat " + task_input + "| ./temp_program > temp_output.txt").c_str());
        if (run_status != 0) {
            std::cerr << "Ошибка выполнения" << std::endl;
            return 1;
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
            break;
        }

        for (int j = 0; j < correct_data.size(); j++) {
            if (correct_data[j] != temp_data[j]) {
                is_OK = false;
                break;
            }
        }
        if (is_OK) {
            std::cout << "OK " << i << std::endl;
        } else {
            std::cout << "WA " << i << std::endl;
            break;
        }
    }

    // Удаление временных файлов
    system("rm temp_source.cpp");
    system("rm temp_program");
    system("rm temp_output.txt");
    return 0;
}
