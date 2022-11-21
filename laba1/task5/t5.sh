#!/bin/bash
cd $3 
ls *.$2 > $1 
mv $1 /media/perefurgon/Disk/121702/Zayats/task5 
open $1 2>errors.txt
