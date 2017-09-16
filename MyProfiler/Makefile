CC := g++
SOURCES := $(shell find . -name "*.cpp")

INCMEASUREMENTS :=-I./src/measurementClasses/Measurements

INCBAT :=-I./src/measurementClasses/SensorData/BatteryData
INCTHERMALDATA :=-I./src/measurementClasses/SensorData/ThermalData
INCDISKDATA :=-I./src/measurementClasses/SensorData/DiskData
INCNETWORKDATA :=-I./src/measurementClasses/SensorData/NetworkData

INCCPUDATA :=-I./src/measurementClasses/SensorData/CpuData
INCCPUINFODATA :=-I./src/measurementClasses/SensorData/CpuData/CpuInfo
INCCPUZONEDATA :=-I./src/measurementClasses/SensorData/CpuData/CpuZone
INCPACKAGEINFODATA :=-I./src/measurementClasses/SensorData/CpuData/PackageInfo
INCCPU := $(INCCPUDATA) $(INCCPUINFODATA) $(INCCPUZONEDATA) $(INCPACKAGEINFODATA) $(INCNETWORKDATA)

INCDATAMEASUREMENTHEADER :=-I./src/measurementClasses/SensorData

INCSRC :=-I./src
INCHELPER :=-I./src/helper
INCPROFILER :=-I./src/Profilers
INCSENSORDATA := $(INCBAT) $(INCTHERMALDATA) $(INCDISKDATA) $(INCCPU) $(INCDATAMEASUREMENTHEADER)
DIRS := $(INCSRC) $(INCHELPER) $(INCPROFILER) $(INCMEASUREMENTS) $(INCSENSORDATA)
INSTALLDIR := /usr/local/bin

all:
	$(CC) $(SOURCES) $(DIRS) -o profiler
debug: 
	$(CC) -g $(SOURCES) $(DIRS) -o profiler
optimised:
	$(CC) -O2 $(SOURCES) $(DIRS) -o profiler
clean:
	rm -f profiler profiler_debug profiler_optimised *.txt *.csv *.tsv *.json *.so
install:
	install -m 755 profiler $(INSTALLDIR)/powerKap
	setcap "CAP_SYS_ADMIN,CAP_NET_ADMIN=pe" $(INSTALLDIR)/powerKap
	install -m 755 enableVirtNet.sh $(INSTALLDIR)/enableVirtNet
uninstall:
	rm -f $(INSTALLDIR)/powerKap $(INSTALLDIR)/enableVirtNet
