#! /etc/bash
#
# Analyse a pgm-ascii-file and show the ..., the median and the value of the colors.


# reading the passed file
function parseFile(){

	TYPE=$(grep -e '^P[1-6]$' $FILE);

	# check for valid file
	if [ -z $TYPE ]; then echo 'Invalid File Input'; exit 13; fi;


	# missing format ? || 65536
	RANGE=$(grep -e '^255$' $FILE);

	# unknown structure / order, not just line count etc.
	COMMENTS=$(grep -e '^#.*$' $FILE);

	# calculate size
	SIZE=$(grep -e '^[0-9]* [0-9]*$' $FILE)
	SIZE=(${SIZE// / }); # ' ' as  dilemeter after // , better than cut -d' ' -f1 piping
	SIZE=$((${SIZE[0]}*${SIZE[1]}));

	echo 'TYPE:' $TYPE;
	echo 'RANGE:' $RANGE;
	echo 'COMMENTS:' $COMMENTS;
	echo 'SIZE:' $SIZE;

	echo -en '\n';

	# pure data - line doesnt matter
	BODY=$(grep -v -e '^P[1-6]$' -e '^#.*$' -e '^[0-9]* [0-9]*$' -e '^255$' $FILE);


	# creating an empty array
	REF=();

	# setting default values
	while [ ${#REF[@]} -le 255 ]; do REF+=(0); done; #  less equals

	# counting entries on top
	for DATA in $BODY; do REF[$DATA]=$((${REF[$DATA]}+1)); done;

	# show results - equalize lengths
	TEMP="";

	for (( i=0; i<=255 ; i++ ));
		do
			if [ $i -lt 10 ];		then TEMP=00;

			elif [ $i -lt 100 ];	then TEMP=0;

									else TEMP="";
			fi



			echo "$TEMP$i:" ${REF[$i]};
	done;
}


if [ $# == 1 ];

	then

		FILE=$1;

		echo -ne '\n''Reading:' $FILE '\n\n';

		parseFile;

		exit 0;

	else

		echo 'Invalid input';
		exit 127;
fi

