#! /etc/bash
#
# Invert a 'pgm-ascii' file and write the result into a new file.

# minimum input
if [ $# == 2 ];

	then

		SOURCE=$1;
		TARGET=$2;

		if [ -r $TARGET ]; then echo "Target already exists !"; exit 3; fi;

		TYPE=$(grep '^P[1-6]$' $SOURCE);

		# check for valid file
		if [ -z $TYPE ]; then echo 'Wrong File Input !'; exit 2; fi;


		echo -en "\nReading:" $SOURCE "\n\n";

		echo -en "Start calculating the pixel values... \n\n";

		# calculate size
		SIZE=$(grep -e '^[0-9]* [0-9]*$' $SOURCE)

		RANGE=$(grep -e '^255$' $SOURCE);

		# creating the file
		> $TARGET;

		# copy header, keep structure
		echo $TYPE >> $TARGET;
		grep '^#.*$' $SOURCE >> $TARGET;
		echo $SIZE >> $TARGET;
		echo $RANGE >> $TARGET;

		# data body
		BODY=$(grep -v -e '^P[1-6]$' -e '^#.*$' -e '^[0-9]* [0-9]*$' -e '^255$' $SOURCE);

		# counting entries on top, keep formating
		COUNTER=0;

		for DATA in $BODY; 	do

								if [ $DATA -lt 10 ];	then TEMP="  ";
								elif [ $DATA -lt 100 ];	then TEMP=" ";
														else TEMP="";
								fi

								echo -en ".";
								echo -en "$TEMP$((255-$DATA)) " >> $TARGET;

								# formating like original
								COUNTER=$(($COUNTER+1));
								if [ $COUNTER -eq 17 ]; then echo "" >> $TARGET; COUNTER=0; fi;

							done;

		echo -en "\n\nFinished - written into \"$TARGET\".\n";

		exit 0;

	else

		echo "Invalid Input !";
		exit 1;
fi

