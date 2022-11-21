#!/bin/bash
echo 'int main(){printf("Выполнение задания 6\n");}'>$1 
gcc -include stdio.h $1 -o $2 2>errors.txt
 if [ -f errors.txt ]; then
  if [ -s errors.txt ]; then
    echo "Ошибка при компиляции"
  else
    ./$2
  fi  
else 
  echo "Файла для чтения ошибок не существует"
fi

