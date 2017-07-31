KO=usb-storage.ko 
num=`lsmod | grep usb_storage | wc -l`

# $1 = 1 : insmod
# $1 = 0 : rmmod

if [ $# -ne 1 ];then
	echo "Usage : $0 1(insmod) / 0(rmmod)"
	exit
fi

if [ $1 -eq 1 ]
then
	if [ $num -ne 1 ]
	then
		insmod /lib/modules/4.4.0-31-generic/kernel/drivers/usb/storage/usb-storage.ko
	else
		echo "usb_storage.ko already exist"
	fi
elif [ $1 -eq 0 ]
then
	if [ $num -eq 1 ];then
		rmmod usb-storage.ko
	elif [ $num -eq 0 ];then
		echo "error : $KO has not been insmod"
		exit
	fi
fi
