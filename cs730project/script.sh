#!/bin/bash


current_time(){

	var1=`date +"%s.%3N"`
	var1=`echo "$var1 * 1000" | bc`
}
current_time

echo $var1

z=`echo "$var1 + 3000" | bc`

echo $z
#(( $(echo "$z > $var1" |bc -l) ))

#while [ $var1 -ne $z ]

while ((  $(echo "$z > $var1" |bc -l)  ))
do
  echo Number: $var1
  echo "z is : $z"
current_time
done




