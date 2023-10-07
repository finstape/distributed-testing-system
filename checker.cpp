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

    std::string code((std::istreambuf_iterator<char>(inputFile)),
                     std::istreambuf_iterator<char>());
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


    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    // Запускаем
    int run_status = system("temp_program.exe");
    if (run_status != 0) {
        std::cerr << "Ошибка выполнения" << std::endl;
        return 1;
    }

    // Удаление временных файлов
    system("del temp_source.cpp");
    system("del temp_program.exe");

    freopen("CON","w",stdout);
    freopen("CON","r",stdin);

    vector<string> c_o, o;
    int temp_str;

    freopen("correct_output.txt", "r", stdin);
    cout << "text\n";
    cin >> temp_str;
    cout << "123" << endl;
    cout << temp_str << endl;

    return 0;
}
