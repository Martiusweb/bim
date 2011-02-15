#! /bin/sh

if [ $# -eq 1 ]
then
    echo "Usage : $0 nbIterationsTest nbIteration prog1 prog2 [prog3...]"
    exit
fi

# Number of iteration each program will do.
nbIterations=$1
# Number of times each program will be run.
nbIterationsTest=$2
shift
shift

DATAFILES=

for prog in $*
do
    # The effective run time (i.e. System + User).
    real=0
    # The name of the file in which the data are stored.
    PROGNAME=`basename $prog`
    DATAFILE="`dirname $prog`/$PROGNAME.dat"
    echo -n "Testing $prog"
    for i in `seq $nbIterationsTest`
    do
        real=$( 2>&1 /usr/bin/time -f "%E" $prog $nbIterations)
        minutes=`echo $real | cut -d ":" -f 1`
        seconds=`echo $real | cut -d ":" -f 2 | cut -d '.' -f1` 
        cents=`echo $real | cut -d ":" -f 2 | cut -d '.' -f2`
        # Append the results in a file 
        echo "$minutes $seconds $cents" >> $DATAFILE
        echo -n "."
    done
    DATAFILES="$DATAFILES $DATAFILE"
    echo ""
done

# Call the analyzer
benchtool/timeResultParser.py $DATAFILES
