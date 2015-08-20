# Description of HCAL DQM Software
HCAL DQM Software generically can be divided into 2 components: CMSSW and 
non-CMSSW. CMSSW incorporates all the Analysis Tools, handles the Monitor 
Elements Collections(booking, resetting, however **NO DRAWING!**), provides 
generic functionality to monitor the status of HCAL and its every subsystem.
Non-CMSSW Component can be further divided into plugins for the DQM GUI and
generic HCAL DQM scripts that provide some functionality for the Local Detector
Diagnostics(analysis of the Local Runs), handling of some data copying, etc...
Therefore, for the purpose of the description, we differentiate between CMSSW 
and non-CMSSW components of the HCALDQM.

**See HowTo Section for the usage.**

## CMSSW Components/Modules/Packages/Plugins
HCAL DQM CMSSW Modules/Packages can be divided into:
* Plugins/Sources(a la DQMEDAnalyzer)
* Clients(a la DQMEDHarverster)
* HCAL DQM Application Config Files - which are typically being run with cmsRun

#### HCAL DQM Plugins/Sources
Reside in either DQM/HcalTasks or DQM/HcalMonitorTasks. The first one contains 
the most recent HCAL DQM Tasks that utilize new functionality in DQM/HcalCommon. 
The later one contains old HCAL DQM Monitor Tasks that have been during Run I, 
LS1, and will partially be used during Run 2. These two are written by 2 different
groups of people, however provide similar functionality. 
**Here the preference is given to the newest modules residing in DQM/HcalTasks.
For further reference regarding the old modules, consult here https://twiki.cern.ch/twiki/bin/viewauth/CMS/HcalDQM**.

A list of available tasks(in DQM/HcalTasks):
* Raw
* Digi
* RecHit
* TP
* Timing
* LED
* Laser
* Pedestal

#### HCAL DQM Clients
Reside in either HcalMonitorClient or HcalClients. For the timebeing, only
HcalMonitorClient is being used for Determining the Status of HCAL. I want to make
it clear from onset, **there is only 1 real DQMEDHarvester and several simple 
classes that are being called from it. This is done because these clients are run
in sequential order always and also becasue that is the easiest way to generate
a summary. That is for the Old Clients**. 
Typically these clients aren't used for any offline analysis if someone wishes
to use HCAL DQM Modules, cause they need the histograms themselves, not the 
status.

A list of clients used to generate HCAL Summary/Status:
* DeadCellMonitor
* HotCellMonitor
* RecHitMonitor
* DigiMonitor
* RawDataMonitor
* TrigPrimMonitor
* NZSMonitor
* BeamMonitor
* CoarsePedestalMonitor
* DetDiagTimingMonitor
* Summary

Not all of them are actually used, but listed nevertheless...

#### Python HCAL DQM Application Config Files
Reside just in the root folder of the repo. Here they are:
* `new_hcal_dqm_Global.py`
* `new_hcal_dqm_Calib.py`
* `new_hcal_dqm_Local.py`

Names should obviously reflect the usecase of these config files. Global is what 
is being run online, calib is what is being run during the Abort gap as DQM uses
DQMCalibrationStream for that purpose. And finally for the local detector 
diagnostics we run Local. Each file is well documented and should be easy to 
follow. At the beginning of each there are some bolean variables that help 
transition from lxplus to P5 or select a map from DB...

## non-CMSSW Software
**To be Provided**

## HowTo
As of 09/08/2015, the PRs to include these new modules and new configs have been
submitted, however not merged. Therefore some tweaking will be neccesary to run 
everything. The assumption is that you are goin to run on **lxplus**

Current Working Branches for Use:
* master-74X for `CMSSW_7_4_X`
* master-75X for `CMSSW_7_5_X`

1. cmsrel CMSSW greater or equal than 74X and setup the environment
2. git cms-addpkg DQM/Integration
3. You can either just clone the repo or actually fork it...
4. You will have to copy the neccessary folders from the cloned/forked repo to
the location of your cmssw release, into the DQM/ folder. Just like 
DQM/Integration.
5. Copy the `hcal_dqm_*` files into the DQM/Integration/python/test
6. Build CMSSW: scram b -j 8 from the src dir of the CMSSW
7. To Run `new_hcal_dqm_Global.py` on lxplus, you have to set OfflineGT to True,
useFileInput to True and put the right tag in.
8. In DQM/Integration/python/test there is a file `fileinputsource_cfi.py`, which 
contains the definition for the Pool Source. Put in there the filenames you want to run over. This step is exactly the same as for any analysis - you have to 
provide the input files...





