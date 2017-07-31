#!/bin/sh

file_check_func() {
	filename=gpu$1d_profiling.txt
	if [ ! -f ${filename} ];then
		touch ${filename}
	fi
	if [ ! -w ${filename} ];then
		echo "${filename} can not write"
 		chmod u+w ${filename};chmod g+w ${filename}
 	fi
}

file_check_func 2
file_check_func 3
