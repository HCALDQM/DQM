#!/bin/sh

#
#	Deploy Script. For copying and deploying
#	Deployes based on the SUBJECT
#

DEST=$1
HCALDQM=$2
SUBJECT=$3

#	1 - registering scheme for either cmshcalweb01 or hcaldqm machines
#	2 - Image Producers/Savers/Generators
case $SUBJECT in
	1) 
		LIST=$HCALDQM/scripts/register.sh
		echo "### Copying $LIST to $DEST"
		cp $LIST $DEST
		;;
	2)
		imgs=`ls $HCALDQM/scripts/ImageManagement/*`
		runners="$HCALDQM/scripts/deployment/genPlots.py"
		utils="$HCALDQM/scripts/utilities/utilities.py"
		LIST="$imgs $runners $utils"
		echo "### Coying $LIST to $DEST"
		cp $LIST $DEST
		;;
esac




















