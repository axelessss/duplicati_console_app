#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void backup(const char *backup_path, const char *source_path, const char *db_path, const char *passphrase) 
{
    char command[1024];
    snprintf(command, sizeof(command), 
             "duplicati-cli backup file://%s %s --backup-name=MyBackup --dbpath=%s --encryption-module=aes --compression-module=zip --dblock-size=50mb --passphrase=%s --retention-policy=\"1W:1D,4W:1W,12M:1M\"",
             backup_path, source_path, db_path, passphrase);
    system(command);
}

void delete_backup(const char *backup_path, const char *db_path, const char *passphrase) 
{
    char command[1024];
    snprintf(command, sizeof(command), 
             "duplicati-cli delete file://%s --dbpath=%s --passphrase=%s",
             backup_path, db_path, passphrase);
    system(command);
}

void check_status(const char *backup_path, const char *db_path, const char *passphrase) 
{
    char command[1024];
    snprintf(command, sizeof(command), 
             "duplicati-cli list file://%s --dbpath=%s --passphrase=%s",
             backup_path, db_path, passphrase);
    FILE *fp;
    char result[1024];

    fp = popen(command, "r");
    if (fp == NULL) 
    {
        printf("Не удалось запустить команду\n");
        return;
    }

    while (fgets(result, sizeof(result), fp) != NULL) 
        printf("%s", result);
    

    if (pclose(fp) == -1) 
        printf("Ошибка при закрытии потока\n"); 
}

int main() 
{
    int choice;
    char backup_path[256];
    char source_path[256];
    char db_path[256];
    char passphrase[256];

    printf("Введите путь к резервной копии: ");
    scanf("%s", backup_path);
    printf("Введите путь к исходным данным: ");
    scanf("%s", source_path);
    printf("Введите путь к базе данных: ");
    scanf("%s", db_path);
    printf("Введите пароль: ");
    scanf("%s", passphrase);

    while (true) 
    {
        printf("Выберите действие:\n");
        printf("1. Сделать бэкап\n");
        printf("2. Удалить бэкап\n");
        printf("3. Проверить статус бэкапа\n");
        printf("4. Выйти\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                backup(backup_path, source_path, db_path, passphrase);
                break;
            case 2:
                delete_backup(backup_path, db_path, passphrase);
                break;
            case 3:
                check_status(backup_path, db_path, passphrase);
                break;
            case 4:
                exit(0);
            default:
                printf("Неверный выбор, попробуйте снова.\n");
        }
    }

    return 0;
}