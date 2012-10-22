#! /etc/bash
#
# Analyse a 'pgm-ascii' file and show the arithemic mean, the median and the probability of the colors.

# minimum input
if [ $# == 1 ];

	then

		FILE=$1;

		echo -en "\n Reading: $FILE  \n\n";

		TYPE=$(grep '^P[1-6]$' $FILE);

		# check for valid file
		if [ -z $TYPE ]; then echo 'Wrong File Input !'; exit 2; fi;


		# data body
		BODY=$(grep -v -e '^P[1-6]$' -e '^#.*$' -e '^[0-9]* [0-9]*$' -e '^255$' $FILE);

		# see 8bit limitation
		RANGE=$(grep '^255$' $FILE);


		COMMENTS=$(grep '^#.*$' $FILE);


		# calculate size
		LENGTH=$(echo $BODY | wc -w);
		SIZE=$(grep '^[0-9]* [0-9]*$' $FILE)


		echo "TYPE: $TYPE";
		echo "RANGE: $RANGE";
		echo "COMMENTS: $COMMENTS";
		echo "SIZE: $SIZE (= $LENGTH pixel)";

		echo -en '\n';


		echo -e "value \t count";
		echo -e "----- \t -----";


		SUM=0;		# sum to compare
		ARITH=0;	#
		MEDIAN=-1;	# default value


		# counting the amount of specific numbers
		for (( i=0; i<=255 ; i++ ));

			do

				if [ $i -lt 10 ];		then TEMP1="  ";
				elif [ $i -lt 100 ];	then TEMP1=" ";
										else TEMP1="";
				fi

				COUNTER=$(echo $BODY | tr ' ' '\n' | grep "^$i$" | wc -w); # grep scrapes the lines

				if [ $COUNTER -lt 10 ];		then TEMP2="   ";
				elif [ $COUNTER -lt 100 ];	then TEMP2="  ";
				elif [ $COUNTER -lt 1000 ];	then TEMP2=" ";
											else TEMP2="";
				fi

				echo -e "$TEMP1$i: \t $TEMP2$COUNTER";


				SUM=$(($SUM+$COUNTER));

				ARITH=$(($ARITH+$i*$COUNTER));

				if [ $MEDIAN -eq "-1" ] && [ $SUM -gt $(($LENGTH/2)) ]

					then

						MEDIAN=$i;
				fi

		done;



		ARITH=$(($ARITH/$LENGTH));

		echo -en "\n";

		echo "Arithemic mean: " $ARITH;
		echo "Median: " $MEDIAN;

		exit 0;

	else

		echo "Invalid Input !";
		exit 1;
fi

