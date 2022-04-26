#!/bin/bash


current_time(){

	var1=`date +"%s.%3N"`
	var1=`echo "$var1 * 1000" | bc`
}
current_time

start_interval
end_interval


echo $var1

z=`echo "$var1 + 3000" | bc`

echo $z
#(( $(echo "$z > $var1" |bc -l) ))

#while [ $var1 -ne $z ]

while ((  $(echo "$start_interval > $var1" |bc -l)  ))
do
current_time
done
`kill -2 $pid`


while ((  $(echo "$end_interval > $var1" |bc -l)  ))
do
current_time
done
`kill -10 $pid`


