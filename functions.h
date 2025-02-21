#ifndef functions
#define functions

int compare_name(char* str, Club club);
int compare_city(char* str, Club club);
int compare_points(char* str, Club club);
int compare_league(char* str, Club club);
int compare_trainer(char* str, Club club);
int advstrcmp(char* str1, char* str2);
void separate();

void scanstr(char* str, int len) {
    fgets(str, len, stdin);
    for (int i=0; i<len; i++) {
        if (*(str + i) == '\n') {
            *(str + i) = '\0';
        }
    }
}

int intscanf() { // функция для безопасного сканирования целых чисел
    int number;
    while (1) {
        if (scanf("%d", &number) == 1) {
            while (getchar() != '\n');
            return number;
        }
        else {
            printf("Ошибка! Пожалуйста, введите целое число\n");
            while (getchar() != '\n');
        }
    }
}

int add_struct(const char * filename) {
    Club club;

    FILE * src_file = fopen(filename, "a");
    if (src_file == NULL) {
        perror("Ошибка открытия файла");
        return -1;
    }
    printf("Введите название команды:\n");
    scanstr(club.name, 50);
    
    printf("Введите город:\n");
    scanstr(club.city, 50);

    printf("Введите количество набранных очков в сезоне:\n");
    scanstr(club.points, 50);

    printf("Введите название лиги:\n");
    scanstr(club.league, 50);

    printf("Введите ФИО главного тренера:\n");
    scanstr(club.trainer, 50);

    size_t written = fwrite(&club, sizeof(Club), 1, src_file);

    fclose(src_file);
    return 0;
}

int remove_struct(const char * filename, int index) {

    FILE * src_file = fopen(filename, "r");
    if (src_file == NULL) {
        perror("Ошибка открытия файла");
        return -1;
    }

    Club buffer;
    size_t readen;

    fseek(src_file, 0L, SEEK_END);
    long len = ftell(src_file);

    if (len < index * sizeof(Club) || index < 0) {
        printf("Неверный порядковый номер\n");
        return -1;
    }

    FILE *temp_file = fopen("tempdata.bin", "w");
    if (temp_file == NULL) {
        perror("Не удалось создать временный файл");
        fclose(src_file);
        return -1;
    }

    fseek(src_file, 0L, SEEK_SET);

    while(ftell(src_file) < sizeof(Club) * index) {
        fread(&buffer, 1, sizeof(Club), src_file);
        fwrite(&buffer, 1, sizeof(Club), temp_file); 
    }

    fseek(src_file, sizeof(Club) * (index+1), SEEK_SET);
    while(fread(&buffer, 1, sizeof(Club), src_file) > 0) {
        fwrite(&buffer, 1, sizeof(Club), temp_file);
    }

    fclose(src_file);
    fclose(temp_file);

    remove(filename);

    rename("tempdata.bin", filename);
    return 0;
}

int edit_struct(const char * filename, int index, int element) {

    FILE * src_file = fopen(filename, "r+");
    if (src_file == NULL) {
        perror("Ошибка открытия файла");
        return -1;
    }

    Club club;
    fseek(src_file, 0L, SEEK_END);
    long len = ftell(src_file);

    if (len < index * sizeof(Club) || index < 0) {
        printf("Неверный порядковый номер\n");
        return -1;
    }

    fseek(src_file, sizeof(Club) * index, SEEK_SET);
    fread(&club, sizeof(Club), 1, src_file);
    
    switch (element)
    {
    case 1:
        printf("Введите название команды:\n");
        scanstr(club.name, 50);
        break;
    case 2: 
        printf("Введите город:\n");
        scanstr(club.city, 50);
        break;
    case 3:
        printf("Введите количество набранных очков в сезоне:\n");
        scanstr(club.points, 50);
        break;
    case 4:
        printf("Введите название лиги:\n");
        scanstr(club.league, 50);
        break;
    case 5:
        printf("Введите ФИО главного тренера:\n");
        scanstr(club.trainer, 50);
        break;
    default:
        return -1;
    }

    fseek(src_file, sizeof(Club) * index, SEEK_SET);
    fwrite(&club, sizeof(Club), 1, src_file);
    fclose(src_file);
    return 0;
}

int write_struct(const char * filename) {
    Club club;
    int i = 0;

    FILE * src_file = fopen(filename, "r");
    if (src_file == NULL) {
        perror("Ошибка открытия файла");
        return -1;
    }

    fseek(src_file, 0L, SEEK_END);

    printf("|-------------------------|-------------------------|-------|-------------------------|------------------------------|----|\n");
    printf("|%-25s|%-25s|%-7s|%-25s|%-30s|%-4s|\n", "name", "city", "points", "league", "trainer", "ID");
    printf("|-------------------------|-------------------------|-------|-------------------------|------------------------------|----|\n");

    fseek(src_file, 0, SEEK_SET);

    while(fread(&club, sizeof(Club), 1, src_file) > 0) {
        printf("|%-25s|%-25s|%-7s|%-25s|%-30s|%-4d|\n", club.name, club.city, club.points, club.league, club.trainer, i);
        i++;
    }
    printf("|-------------------------|-------------------------|-------|-------------------------|------------------------------|----|\n");

    fclose(src_file);
    return 0;
}
int search_struct(char* str, char* filename) {
    FILE * src_file = fopen(filename, "r");
    if (src_file == NULL) {
        perror("Ошибка открытия файла");
        return -1;
    }
    
    Club club;
    int (*compare[5])(char*, Club) = {compare_name, compare_city, compare_points, compare_league, compare_trainer}, a = 0, b = 0;

    fseek(src_file, 0, SEEK_SET);
    printf("Найденные совпадения:\n");
    printf("|%-25s|%-25s|%-7s|%-25s|%-30s|%-4s|\n", "name", "city", "points", "league", "trainer", "ID");
    printf("|-------------------------|-------------------------|-------|-------------------------|------------------------------|----|\n");

    while(fread(&club, sizeof(Club), 1, src_file) > 0) {
        for (int i = 0; i < 5; i++) {
            if (compare[i](str, club) == 1) {
                printf("|%-25s|%-25s|%-7s|%-25s|%-30s|%-4d|\n", club.name, club.city, club.points, club.league, club.trainer, a);
                b++;
                i = 5;
            }
        }
        a++;
    }
    if (b == 0) {
        printf("|%-25s|\n\n", "No matches");
    }
    else {
        printf("\n");
    }
}

int compare_name(char* str, Club club) {
    if (advstrcmp(club.name, str) == 0) {
        return 1;
    }
    return 0;
}
int compare_city(char* str, Club club) {
    if (advstrcmp(club.city, str) == 0) {
        return 1;
    }
    return 0;
}
int compare_points(char* str, Club club) {
    if (advstrcmp(club.points, str) == 0) {
        return 1;
    }
    return 0;
}
int compare_league(char* str, Club club) {
    if (advstrcmp(club.league, str) == 0) {
        return 1;
    }
    return 0;
}
int compare_trainer(char* str, Club club) {
    if (advstrcmp(club.trainer, str) == 0) {
        return 1;
    }
    return 0;
}

int advstrcmp(char* str1, char* str2) { // Позже узнал про strstr(), но решил оставить
    int size1 = strlen(str1);
    int size2 = strlen(str2);

    if (size2 >= size1) {
        return -1;
    }
    int count1 = size1 - size2;
    int count2 = 0;

    for (int i = 0; i <= count1; i++) {
        for (int j = 0; j < size2; j++) {
            if (*(str1 + i + j) != *(str2 + j)) {
                count2 -= 1;
            }
        }
        if (count2 == 0) {
            return 0;
        }
        count2 = 0;
    }
    return -1;
}

#endif