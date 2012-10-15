#! /etc/bash
#
# Analyse a pgm-ascii-file and show the "arithemic", the median and the value of the colors.

if [ $# == 1 ];

	then

		FILE=$1;

		echo -ne '\n''Reading:' $FILE '\n\n';

		# parseFile;

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
		SIZE="${SIZE[0]}px x ${SIZE[1]}px (= $((${SIZE[0]}*${SIZE[1]})))";

		echo 'TYPE:' $TYPE;
		echo 'RANGE:' $RANGE;
		# echo 'COMMENTS:' $COMMENTS;
		echo 'SIZE:' $SIZE;

		echo -en '\n';

		# pure data - line doesnt matter
		BODY=$(grep -v -e '^P[1-6]$' -e '^#.*$' -e '^[0-9]* [0-9]*$' -e '^255$' $FILE);


		# creating an empty array
		REF=();

		# setting default values
		while [ ${#REF[@]} -le 255 ]; do REF+=(0); done; #  less equals


		COUNTER=0;	# increment per operation - amount of entries
		SUM=0;		# total sum
		MEDIAN=();	# median list


		# counting entries on top
		for DATA in $BODY; 	do
								REF[$DATA]=$((${REF[$DATA]}+1));
								COUNTER=$(($COUNTER+1));
								SUM=$(($SUM+$DATA));
								MEDIAN+=($DATA);

							done;

		ARITH=$(($SUM/$COUNTER));

		# this way or does the median also take 0 in account , then allways at the 127 ?
		MEDIAN=${MEDIAN[$((${#MEDIAN[@]}/2))]};


		#  wrong calculation > better way using median or arith...
		P=$((100/$COUNTER));

		# show results - equalize lengths (index, value, percent)
		TEMP1="";
		TEMP2="";
		TEMP3="";

		# current percentage
		CURRENT="";

		echo "value count percentage";
		echo "----- ----- ----------";

		for (( i=0; i<=255 ; i++ ));
			do
				if [ $i -lt 10 ];		then TEMP1="  ";
				elif [ $i -lt 100 ];	then TEMP1=" ";
										else TEMP1="";
				fi

				if [ ${REF[$i]} -lt 10 ];		then TEMP2="   ";
				elif [ ${REF[$i]} -lt 100 ];	then TEMP2="  ";
				elif [ ${REF[$i]} -lt 1000 ];	then TEMP2=" ";
												else TEMP2="";
				fi

				CURRENT=$(( ${REF[$i]} * $P ));

				if [ $CURRENT -lt 10 ];		then TEMP3="  ";
				elif [ $CURRENT -lt 100 ];	then TEMP3=" ";
											else TEMP3="";
				fi

				echo "$TEMP1$i: $TEMP2${REF[$i]} || $TEMP3$CURRENT% ";
		done;

		echo -en "\n";

		echo "Mittelwert:" $ARITH;
		echo "Median:" $MEDIAN;


		exit 0;

	else

		echo 'Invalid input';
		exit 127;
fi

