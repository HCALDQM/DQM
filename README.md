# Description of HCAL DQM CMSSW related Components
* DQM/HcalCommon - Core Components of the HCAL DQM CMSSW. Contains declarations and definitions of Containers, Mappers, Axis Classes. Utilities are also implemented in this package. Base Processing Classes (DQModule, DQTask, DQHarvester) reside there as well.
* DQM/HcalTasks - DQMEDAnalyzers - all the main monitoring tasks are here
* DQM/HcalClients - clients, at the moment client functionality is not being used
* DQM/cfgs - contain all the cmsRun configuration files
* DQM/SpeicalTaks - tasks to be executed on demand: VME vs uTCA comparison. QIE10, etc...

**See HowTo Section for the usage.**

## HCAL DQM Plugins/Sources for Production
A list of available tasks(in DQM/HcalTasks):
* Raw
* Digi
* DigiPhase1
* RecHit
* TP
* LED
* Laser
* Pedestal
* RadDam
* + TestTasks + SpecialTasks

## HCAL DQM Clients/Harvesters
* Harvestors are used in both Online/Offline Processing.

## Python HCAL DQM Application Config Files
* cfg_central - used in online processing
* cfg_private - used in private processing
* For Offline processing, there is no HCAL-defined sequence
