#!/usr/bin/env sh
echo -e "content-type: text/html\r\n\r\n";
echo -e `ultrason -w`;

var1=$(echo "$QUERY_STRING" | cut -d"&" -f1);
var2=$(echo "$QUERY_STRING" | cut -d"&" -f2);
var3=$(echo "$QUERY_STRING" | cut -d"&" -f3 | tr '+' ' ');

if [ "$var1" ]
then
	echo -e `camera -w -m $var1 -M $var2`;
else
	echo -e `camera -w`;
	
fi

if [ "$var3" ]
then
	echo -e `moteur $var3`;
fi