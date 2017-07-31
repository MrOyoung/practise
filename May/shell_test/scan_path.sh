#!/bin/sh

CWD=`pwd`;echo $CWD

cmd="./gpuinfo GPU3D"

function() {
	filename=gpu$1d_profiling.txt

	if [ ! -f ${filename} ];then
		echo "not exist"
		touch ${filename}
	fi 
 
	if [ ! -w ${filename} ];then
		echo "${gpu2d_file} can not write"
		chmod u+w ${gpu2d_file} ;chmod g+w ${gpu2d_file}
	fi
}

function 2
function 3

if [ $# -eq 1 ];then
	if [ "$1" = ">" ];then
		echo "1:$1"
		#${cmd}
	elif [ "$1" = ">>" ];then
		echo "2:$1"
	fi
else
	echo "Usage : $0 '>' or '>>'"
	echo "exit"
	exit
fi
 
