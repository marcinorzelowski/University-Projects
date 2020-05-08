#!/bin/bash

#Script created by Marcin Orzełowski for EOPSY Laboratory. 

# Global variables:
rFLAG=0
hFLAG=0
lFLAG=0
uFLAG=0
passedFlag=""
sedPattern=""
path=""
fileName=""


# Functions:
printHelp () {
	cat << EOT

	modify.sh is script for modifying file names. 

	Can be used as follows:
	modify.sh [-h]
  	modify.sh [-r] [-l|-u] <dir/file names...>
  	modify.sh [-r] <sed pattern> <dir/file names...>

  	Possible flags:

  	-h - shows help
  	-r - recursion is used
  	-l - lowercase names
  	-u - upercase names
  	<sed patter> - set sed pattern to modify the names.

EOT
}

modify() {


	if [ $rFLAG -eq 1 ];
	then
		if [ -d $1 ] ; #if argument is a directory.
		then
	    	cd ./$1; #go to file directory
		    for F in `ls`; #for all files in directory
		    do 
				modify $F; #recursive call
		    done
		    cd .. #go back
		fi
	else
		cd `dirname $1` #in case file is in another directory - need to move there to be able to change files.
	fi


	fileName=`basename "$1"`
	
	#according to the flag use specified method
	if [ $uFLAG -eq 1 ];
	then
		toUppercase $fileName
		return 0
	fi
	if [ $lFLAG -eq 1 ];
	then
		toLowercase $fileName
		return 0
	fi
	if [ -n sedPattern ];
	then
		toSedPattern
		return 0
	fi
	return 0
	
}

toLowercase() {
	#create new file name from old one, changing letter size from upper to lower
	newFileName=`echo $fileName | tr '[:upper:]' '[:lower:]'` 
	if [ "$fileName" != "$newFileName" ]; #checking if files arent the same
	then
       		mv "$fileName" "$newFileName" #change files
	       	echo "File was modyfied"
	fi
}

toUppercase() {
	#create new file name from old one, changing letter size from lower to upper
	newFileName=`echo $fileName | tr '[:lower:]' '[:upper:]'`
	if [ "$fileName" != "$newFileName" ]; 
	then
       		mv "$fileName" "$newFileName"
	       	echo "File was modyfied"
	fi
}

toSedPattern() {
	#creating new file name using sed patter on old one
	newFileName=`echo $fileName | sed $sedPattern`
	if [ "$fileName" != "$newFileName" ]; 
	then
       		mv "$fileName" "$newFileName"
	       	echo "File was modyfied"
	fi
}

checkFlagCorectness() {
	# both lowercase and uppercase
	if [ $uFLAG -ne 0 ] && [ $lFLAG -ne 0 ]
	then
		errorHandle "Both uppercase and lowercase are chosen. You are not allowed to do that."
	fi
	# no flag
	if [ $uFLAG -eq 0 ] && [ $lFLAG -eq 0 ] && [ $rFLAG -eq 0 ] && [ -z $sedPattern ];
	then
		errorHandle "No option chosen. Please follow the instructions:"
	fi
	# wrong path
	if [ ! -d $path ] && [ ! -f $path ] || [ -z $path ];
	then
		errorHandle "Given path is incorrect."
	fi
}

errorHandle() {
	echo "$1" #print error name
	printHelp #calll method to print help
	exit 1
}


# Main script:

# if no arguments or -h flag is used
if [ "$#" -eq "0" ] || [ "$1" = "-h" ];
then
	echo 
	printHelp
	exit 1
fi

# checking flags
while [ "$#" -ne "0" ];
	do
		case "$1" in
			"-r")
				rFLAG="1"
				shift #move to next argument
				;;
			"-l")
				lFLAG="1"
				passedFlag="-l"
				shift #move to next argument
				;;
			"-u")
				uFLAG="1"
				passedFlag="-u"
				shift #move to next argument
				;;
			*)
				
				if [ -f "$1" ] || [ -d "$1" ]; 
				then
					path="$1"
					break #leave the loop
				else
					sedPattern="$1"
			    	passedFlag="$1"
			    	shift
				fi
			    
			    ;;
	esac
done

checkFlagCorectness

#loop for all files
while [ "$#" -ne 0 ];
do
	if [ -f "$1" ] || [ -d "$1" ]; #if argument is file or directory
	then 
		modify $1 
	fi
	shift #move to next argument
done



exit 0
