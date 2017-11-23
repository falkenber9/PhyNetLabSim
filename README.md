PhyNetLabSim
============

A Simulation Model for IoT-based warehouses or testbeds such as PhyNetLab.



## Requirements

* OMNeT++ 4.6 (versions 4.3 to 5.0-Beta1 should also work)
* Linux operating system (though Windows should work as well)



## Usage Instructions

### Setup
* Download [OMNeT++ 4.6](https://www.omnetpp.org/component/jdownloads/category/32-release-older-versions), unpack and follow installation instructions from https://www.omnetpp.org/doc/omnetpp/InstallGuide.pdf
    *   Minimum steps:
```bash
        $ . setenv
        $ ./configure
        $ make
```
* Dont't forget to **install OMNeT++ dependencies** from the distribution-specific sections in the install guide
* Clone PhyNetLabSim into an arbitrary directory
* Start omentpp and select PhyNetLabSim as working directory
* **do not** install inet and examples from first-start dialog
* Select *File* > *Import...* and select *General* > *Existing Projects into Workspace*
* Select root directory once again to PhyNetLabSim
* Two projects should now be listed under *Projects*:
    *   IEEE802154INET-Standalone
    *   inet
* Select both and *Finish* the wizard.
* Build the projects by selecting *Project* > *Build all*

### Launch Examples in GUI (Tkenv)
* Unfold *IEEE802154INET-Standalone* > *simulations* and **select** *omnetpp.ini*
* Click *Run* > *Run*
* Select config name / run number
* Press run to follow a visualization of the simulation

### Launch Examples in Eclipse-Console (Cmdenv)
* Select project *IEEE802154INET-Standalone*
* Click *Run* > *Run Configuration...*
* In *OMNeT++ Simulation* > *simulations* set *User interface* to *Command line*
* In addition, select *omnetpp.ini*, a configuration and a (sub)set of runs, eg., "0..20" or "*". (hover the field with your mouse to get a tooltip with examples)
* Press *Run* to start simulation
* The statistics of each node in each run are appended to the following logfile:
* PhyNetLabSim/IEEE802154INET-Standalone/simulations/results.txt

### Launch Examples in Linux Terminal
* Execute the following commands in terminal:
```bash
    $ cd PhyNetLabSim/IEEE802154INET-Standalone/simulations
    $ ../src/IEEE802154INET-Standalone -u Cmdenv -c <config-name> -n ../src:.:../../inet/examples:../../inet/src -l ../../inet/src/inet omnetpp.ini
```
* Replace ``<config-name>`` by a configuration from omnetpp.ini, eg., "SysCon_QRR80_BE3"
* In case omnet environment variables are not set, preceed the previous step by following commands:
```bash
    $ cd <omnet-dir>
    $ . setenv
```
* The statistics of each node in each run are appended to the following logfile:
* PhyNetLabSim/IEEE802154INET-Standalone/simulations/results.txt



## Directory structure

#### /IEEE802154INET-Standalone:
adapted version of IEEE 802.15.4-2006 Simulation Model for OMNeT++ / INET

- /IEEE802154INET-Standalone/simulations --> contains network definition, omnetpp.ini and scenarios.ini where all settings are defined
- /IEEE802154INET-Standalone/src/... --> contains source code under Modules and util folders and NED files under NED folder

#### /inet:
included INET Framework for OMNEST/OMNeT++ version 2.6-b673687