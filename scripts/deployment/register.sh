#!/bin/sh

#
#	Registration Script. 
#	Registers HCALDQM Outputs with HCALDQM GUI at cmshcalweb01
#

#	predefine paths
DATAPOOL=/HCAL_Web/Webserver_repo/current/data01/DetDiag/Local_HTML
DQMGUI=/home/hcaldqm/HCALDQM/GUI
FILENAMEBASE=DQM_V0001_Hcal_R000
LENGTHFILENAMEBASE=${#FILENAMEBASE}
LOGFILEBASE="/tmp/hcaldqm/register.log"

#	Initialize env vars
source $DQMGUI/current/apps/dqmgui/128/etc/profile.d/env.sh

#	list all the directories containing root files
#	and index them if they haven't been yet
for DIR in $DATAPOOL/DQM_V0001_Hcal_*; do

	#	Some RUNNUMBER extraction
	LENGTHBASE=${#DIR}
	POS=`expr $LENGTHBASE + 1 + $LENGTHFILENAMEBASE`
	LENGTH=6
	FILENAME=`ls $DIR/*`
	RUNNUMBER="${FILENAME:$POS:$LENGTH}"
	LOGFILE="$LOGFILEBASE.$RUNNUMBER"

	#	If it's been registered, skip
	if [[ ! -e $DQMGUI/indexed/$RUNNUMBER ]]; then
		echo "Indexing FileName: $FILENAME" > $LOGFILE
		if visDQMIndex add --dataset /Global/Online/ALL $DQMGUI/state/dqmgui/dev/ix128 $FILENAME > $LOGFILE; then
			touch $DQMGUI/indexed/$RUNNUMBER
		fi
	fi
done










