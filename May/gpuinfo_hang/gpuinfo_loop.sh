#!/bin/sh

cmd="./mmdc_hang "

file_check_func() {
	filename=gpu$1d_profiling.txt

	if [ ! -f ${filenam} ];then
		touch ${filename}
	fi
	if [ ! -w ${filename} ];then
		echo "${filename} can not write"
		chmod u+w ${filename};chmod g+w ${filename}
	fi
}

file_check_func 2
file_check_func 3

if [ $# -eq 1 ];then
	if [ "$1" = ">" ];then
		echo "GPU2D : " > ${__2d_file} 
		echo "GPU3D : " > ${__3d_file}
	elif [ "$1" = ">>" ];then
		echo "----------------------------------------" >> ${__2d_file}
		echo "--------------   wl-gear  --------------" >> ${__2d_file}
		echo "----------------------------------------" >> ${__2d_file}
		echo "----------------------------------------" >> ${__3d_file}
		echo "--------------   wl-gear  --------------" >> ${__3d_file}
		echo "----------------------------------------" >> ${__3d_file}
	fi
else
	echo "Usage : $0 '>' or '>>'"
	echo "exit"
	exit
fi

count="1 2 3 4 5 6 7 8 9 10"

while true;do
	for i in $count
	do
		${cmd} GPU2D >> ${__2d_file}
		sleep 1
		echo "---------------$i--------------------" >> ${__2d_file}
	done

	sleep 5

	for i in  $count
	do
		${cmd} GPU3D >> ${__3d_file}
		sleep 1
		echo "---------------$i--------------------" >> ${__3d_file}
	done
	
	break
done


