#include <stdio.h>
#include <string.h>

#include "structs.h"
#include "functions.h"

#define H 1 // Запись стандартных значений в файл при запуске программы. 1 - запуск с записью. 0 - стандартный запуск

int a, b, c;
char str[50];

int main() {
    #if H == 1
        Club clubs[6] = {{"teamcats", "dogcity", "45" , "catleague", "catman"},
        {"tea", "london", "34" , "europe league", "bob miles"},
        {"train", "los angeles", "47" , "US league", "jack johnson"},
        {"bears", "moscow", "335" , "premier league", "boris borisovich"},
        {"baguet", "paris", "64" , "champion league", "louis"}};

        FILE * src_file = fopen("data.bin", "wb");
        if (src_file == NULL) {
            perror("Ошибка открытия файла");
            return -1;
        }
        fwrite(clubs, sizeof(Club), 5, src_file);
        fclose(src_file);
    #endif

    while(1) {
        printf("Выберите действие:\n"
        "1. Добавление записи в файл\n"
        "2. Удаление заданной записи из файла по порядковому номеру записи\n"
        "3. Поиск записей по полю структуры\n"
        "4. Редактирование (изменение) заданной записи\n"
        "5. Вывод на экран содержимого файла в табличном виде\n"
        "6. Завершение программы\n"
        "(в таблицу добавлять только латиницу и цифры)\n");

        a = intscanf();
        switch (a)
        {
        case 1:
            add_struct("data.bin");
            break;
        case 2:
            printf("Введите порядковый номер записи:\n");
            b = intscanf();
            remove_struct("data.bin", b);   
            break;
        case 3:
            printf("Введите поисковый запрос:\n");
            scanstr(str, 50);
            search_struct(str, "data.bin");
            break;
        case 4:
            printf("Введите порядковый номер записи:\n");
            b = intscanf();
            printf("Выберите изменяемое поле:\n"
            "1. Название\n"
            "2. Город\n"
            "3. Очки\n"
            "4. Лига\n"
            "5. Тренер\n");
            c = intscanf();
            edit_struct("data.bin", b, c);
            break;
        case 5:
            write_struct("data.bin");
            break; 
        case 6:
            return 0;
            break;
        default:
            printf("Недопустимое значение\n");
        }
    }
    return 0;
}