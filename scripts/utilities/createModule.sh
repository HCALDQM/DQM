#!/bin/sh

ref=$1
dest=$2
ct=$3

CURRENTFOLDER=$PWD

SRCFOLDER="$CURRENTFOLDER/../HcalCommon"
SRCINTFILE=$SRCFOLDER/interface/HcalDQ.h
SRCSRCFILE=$SRCFOLDER//HcalDQClient.cc

DESTFOLDER="$CURRENTFOLDER/../Hcal$ct"
PTDEST=$DESTFOLDER/python
INTDEST=$DESTFOLDER/interface
PLGDETS=$DESTFOLDER/plugins

cp "Hcal$1Task.py" "Hcal$2Task.py"
cp "../interface/Hcal$1Task.h" "../interface/Hcal$2Task.h"
cp "../plugins/Hcal$1Task.cc" "../plugins/Hcal$2Task.cc"

sed -i "s/$1/$2/g" "Hcal$2Task.py" \
	"../interface/Hcal$2Task.h" \
	"../plugins/Hcal$2Task.cc"
sed -i "s/${1^^}/${2^^}/g" "Hcal$2Task.py" \
	"../interface/Hcal$2Task.h" \
	"../plugins/Hcal$2Task.cc"

