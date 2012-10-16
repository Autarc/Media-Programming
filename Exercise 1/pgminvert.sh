#! /etc/bash
#
# Invert a pgm-ascii-file and write the result into a new file.

if [ $# == 2 ];

	then

		# parameter
		SOURCE=$1;
		TARGET=$2;

		if [ -r $TARGET ]; then echo "The target already exists !"; exit 11; fi;

		TYPE=$(grep -e '^P[1-6]$' $SOURCE);

		# check for valid file
		if [ -z $TYPE ]; then echo "Invalid File Input"; exit 13; fi;


		echo -en "\n""Reading:" $SOURCE "\n";

		echo -en "\n""Start calculating the pixel values...""\n\n";

		# calculate size
		SIZE=$(grep -e '^[0-9]* [0-9]*$' $SOURCE)

		# missing format ? || 65536
		RANGE=$(grep -e '^255$' $SOURCE);

		# creating the file
		> $TARGET;

		# copy header, keep structure
		echo $TYPE >> $TARGET;
		grep -e '^#.*$' $SOURCE >> $TARGET;
		echo $SIZE >> $TARGET;
		echo $RANGE >> $TARGET;


		# pure data - line doesnt matter
		BODY=$(grep -v -e '^P[1-6]$' -e '^#.*$' -e '^[0-9]* [0-9]*$' -e '^255$' $SOURCE);

		# initial entry counter
		COUNTER=0;

		# counting entries on top
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

		echo -en "\n\n""Finished - written into \"$TARGET\".\n";

		exit 0;

	else

		echo -en "\n\n""Invalid input. \n";
		exit 127;
fi

