# Description of HCAL DQM CMSSW related Components
* DQM/HcalCommon - Core Components of the HCAL DQM CMSSW. Contains declarations and definitions of Containers, Mappers, Axis Classes. Utilities are also implemented in this package. Base Processing Classes (DQModule, DQTask, DQHarvester) reside there as well.
* DQM/HcalTasks - DQMEDAnalyzers - all the main monitoring tasks are here
* DQM/HcalClients - clients, at the moment client functionality is not being used
* DQM/cfgs - contain all the cmsRun configuration files
* DQM/TestTasks - all the Test Tasks are residing here
* DQM/SpeicalTaks - tasks to be executed on demand: VME vs uTCA comparison. QIE10, etc...

**See HowTo Section for the usage.**

## HCAL DQM Plugins/Sources for Production
A list of available tasks(in DQM/HcalTasks):
* Raw
* Digi
* RecHit
* TP
* LED
* Laser
* Pedestal
* RadDam
* + TestTasks + SpecialTasks

## HCAL DQM Plugins/Sources for Testing/Playback
* QIE10Test
* DigiComparison
* TPComparison

## HCAL DQM Clients/Harvesters
Currently this functionality isn't being used.

## Python HCAL DQM Application Config Files
Reside just in the root folder of the repo. Here they are:
* `hcal_dqm_sourceclient-live_cfg_.py`
* `hcalcalib_dqm_sourceclient-live_cfg.py`
* `hcal_dqm_sourceclient-offline_cfg.py`

## HowTo
80X - most recent PR branch. see tags for 80X...
