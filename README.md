# Description of HCAL DQM CMSSW related Components
DQM/HcalCommon - Core Components of the HCAL DQM CMSSW. Contains declarations and definitions of Containers, Mappers, Axis Classes. Utilities are also implemented in this package. Base Processing Classes (DQModule, DQTask, DQHarvester) reside there as well.
DQM/HcalTasks - DQMEDAnalyzers - all the main monitoring tasks are here
DQM/HcalClients - clients, at the moment client functionality isn't being used
DQM/cfgs - contain all the cmsRun configuration files

**See HowTo Section for the usage.**

## HCAL DQM Plugins/Sources
A list of available tasks(in DQM/HcalTasks):
* Raw
* Digi
* RecHit
* TP
* LED
* Laser
* Pedestal
* RadDam

## HCAL DQM Clients/Harvesters
Currently this functionality isn't being used.

## Python HCAL DQM Application Config Files
Reside just in the root folder of the repo. Here they are:
* `hcal_dqm_sourceclient-live_cfg_.py`
* `hcalcalib_dqm_sourceclient-live_cfg.py`
* `hcal_dqm_sourceclient-offline_cfg.py`

## HowTo
dev-master is the branch being **master** development

1. cmsrel CMSSW greater or equal than 74X and setup the environment
2. git cms-addpkg DQM/Integration
3. You can either just clone the repo or actually fork it...
4. cp -r HcalCommon HcalTasks **your_cmssw**/src/DQM
5. Copy the `hcal_dqm_*` files into the DQM/Integration/python/test
6. Build CMSSW: scram b -j 8 from the src dir of the CMSSW
7. To Run `.py` on lxplus, you have to set OfflineGT to True,
useFileInput to True and put the right tag in(However, current tag should be reasonable).
8. In DQM/Integration/python/test there is a file `fileinputsource_cfi.py`, which 
contains the definition for the Pool Source. Put in there the filenames you want to run over. This step is exactly the same as for any analysis - you have to 
provide the input files...





