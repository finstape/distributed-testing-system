#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <vector>

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    // Имя файла для чтения
    std::string filename = "source.cpp";

    // Чтение содержимого файла
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Ошибка открытия файла " << filename << std::endl;
        return 1;
    }

    std::string code((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Сохраняем исходный код во временный файл
    std::ofstream outputFile("temp_source.cpp");
    outputFile << code;
    outputFile.close();

    // Компилируем
    int compile_status = system("g++ temp_source.cpp -o temp_program.exe");
    if (compile_status != 0) {
        std::cerr << "Ошибка компиляции" << std::endl;
        return 1;
    }



    string task_name;
    cout << "Task name: ";
    cin >> task_name;
    for (int i = 1; i <= 52; i++) {
        string suka = task_name + "/" + to_string(i) + ".in";
        freopen(suka.c_str(), "r", stdin);
        freopen((task_name + "/result" + to_string(i) + ".txt").c_str(), "w", stdout);
        // Запускаем
        int run_status = system("temp_program.exe");
        if (run_status != 0) {
            std::cerr << "Ошибка выполнения" << std::endl;
            return 1;
        }

        freopen("CON","w",stdout);
        freopen("CON","r",stdin);
        vector<string> c_o, o;
        string temp_str;

        string str;

        ifstream file_c_o;
        str = (task_name +"/" + to_string(i) + ".out");
        file_c_o.open(str);
        while (getline(file_c_o, temp_str)) {
            c_o.push_back(temp_str);
        }
        file_c_o.close();

        ifstream file_o;
        str = task_name + "/result" + to_string(i) + ".txt";
        file_o.open(str);
        while (getline(file_o, temp_str)) {
            o.push_back(temp_str);
        }
        file_o.close();
        bool is_OK = true;
        //cout << c_o.size() << " " << o.size() << endl;
        //cout << c_o[i-1] << " " << o[i-1] << endl;
        if (c_o.size() != o.size()) {
            cout << "WA" << i << endl;
            continue;
        }
        for (int j = 0; j < c_o.size(); j++) {
            if (c_o[j] != o[j]) {
                is_OK = false;
                break;
            }
        }
        if (is_OK) {
            cout << "OK" << i << endl;
        } else {
            cout << "WA" << i << endl;
        }
    }
    // Удаление временных файлов
    system("del temp_source.cpp");
    system("del temp_program.exe");
    return 0;
}
