#!/bin/bash
text=`cat $1`
echo "$text" | sed -e 's/\\\\\"/\"/g'
