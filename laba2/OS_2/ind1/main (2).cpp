#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "iostream"
#include "vector"
#include <exception>
#include <filesystem>
#define BUF_SIZE  4096
char buffer [BUF_SIZE];


using namespace std;

void mycopyfile (const char *source_file, const char *destination_file)
{

    int infd, outfd;
    ssize_t bytes;

    infd = open (source_file, O_RDONLY);
    if (infd == -1)
    {
        fprintf (stderr, "НЕ могу открыть входящий файл ""(%s)\n", source_file);
        exit (1);
    }

    outfd = open (destination_file, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if (outfd == -1)
    {
        fprintf (stderr, "НЕ могу открыть выходящий файл ""(%s)\n", destination_file);
        exit (2);
    }

    while ((bytes = read(infd, buffer, BUF_SIZE)) > 0)
        write (outfd, buffer, bytes);

    close (infd);
    close (outfd);
    //exit (3);
}




int get_file_size(std::string filename) // path to file
{
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    fclose(p_file);
    return size;
}


int main (void)
{
    DIR *dir1, *dir2;
    struct dirent *entry1, *entry2;
    int flag, flag_name, k, i, m, cur, NPROC;
    string in_str, out_str;
    pid_t pid_end;
    int file_o;
    string str_dir1, str_dir2;
    cout << "Dir1 = ";
    cin >> str_dir1; //= "/Users/ignat/Desktop/dir1";
    cout << "Dir2 = ";
    cin >> str_dir2; //= "/Users/ignat/Desktop/dir2";
    cout << "Число процессов N: \n"; // Введите число процессов
    cin >> NPROC; // Число процессов по адресу NPROC
    pid_t pids[10];
    dir1 = opendir(str_dir1.c_str()); // Результат открытия каталога Dir1
    dir2 = opendir(str_dir2.c_str()); // Результат открытия каталога Dir2

    if ((!dir1)||(!dir2)) // Если неудачное открытие
    {
        perror ("diropen");
        return 1;
    }

    i = 0;
    vector<dirent *> files_in_dir1;
    while ((entry1 = readdir(dir1)) != NULL) {
        if (entry1->d_name[0] == '.')
            continue;
        else;
        files_in_dir1.push_back(entry1);
    }

    vector<dirent *> files_in_dir2;
    while ((entry1 = readdir(dir2)) != NULL) {
        if (entry1->d_name[0] == '.')
            continue;
        else;
        files_in_dir2.push_back(entry1);
    }


    for(int n = 0; n < files_in_dir1.size(); n++) {
        flag = 1;
        for(int j = 0; j < files_in_dir2.size(); j++) {
            for (flag_name = 1, k = 0; (k < MAXNAMLEN) && (files_in_dir1[n]->d_name[k] != '\0'); k++) {
                if (files_in_dir1[n]->d_name[k] != files_in_dir2[j]->d_name[k]) // Если кат. не равны...
                    flag_name = 0;
            }

            if ((files_in_dir2[j]->d_type == files_in_dir1[n]->d_type) &&
                (files_in_dir2[j]->d_reclen == files_in_dir1[n]->d_reclen))
                flag = 0;
        }

        rewinddir (dir2);
        if (flag)  // Если 1
        {
            if (i == NPROC)  // Если равен числу процессов
            {
                pid_end = wait(NULL);//wait end process (Завершение)
                for (m = 0; (m < NPROC) && (pids[m] != pid_end); m++); // Поиск ном. оконч. процесса
                cur = m;
            } else cur = i;

            pids[cur] = fork();  // Порождается процесс
            if (pids[cur] < 0) {
                perror("fork");
                return 1;
            } else if (pids[cur] > 0) {
                in_str = str_dir1;         // Присвоить назв. кат.
                in_str += "/";             // Добавить косую черту
                in_str += files_in_dir1[n]->d_name; // Присв. назв. файла
                out_str = str_dir2;        // Присвоить назв. кат.
                out_str += "/";            // Добавить косую черту
                out_str += files_in_dir1[n]->d_name; // Присвоить назв. файла
                mycopyfile(in_str.c_str(), out_str.c_str()); // Копир.
                cout << "Pid = " << getpid() << " Name = " << files_in_dir1[n]->d_name << " Size = " << get_file_size(in_str) << endl;
                if (i < NPROC) i++;
            }
        }
    }

    closedir (dir1);
    closedir (dir2);
    for (i = 0; i < NPROC; i++)
        waitpid (pids[i], NULL, 0);

    return 0;
    }