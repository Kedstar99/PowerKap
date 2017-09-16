package EnergyModule.Graph.Model;

import EnergyModule.Graph.Model.JSON.*;
import org.jfree.data.xy.AbstractIntervalXYDataset;
import org.jfree.data.xy.YIntervalSeriesCollection;

import java.util.*;
import java.util.stream.Collectors;

/**
 * Created by kd913 on 06/05/17.
 */
public class ProcessCPUData extends ProcessMeasurements{

    private boolean showCore;
    private boolean showUncore;
    private boolean showDram;
    private boolean showPackageTotal;
    private boolean showEnergyCounter;
    private boolean showPowerCounter;

    public ProcessCPUData(HashMap<String, JSONReference[]> values, boolean showCore, boolean showUncore, boolean showDram, boolean showPackageTotal, boolean showEnergyCounter, boolean showPowerCounter) {
        super(values);
        this.showCore = showCore;
        this.showUncore = showUncore;
        this.showDram = showDram;
        this.showPackageTotal = showPackageTotal;
        this.showEnergyCounter = showEnergyCounter;
        this.showPowerCounter = showPowerCounter;
    }


    @Override
    public void processDataset(String tag, JSONReference[] input, AbstractIntervalXYDataset dataset) {
        ArrayList<Long> times = Arrays.stream(input).map(JSONReference::getTime).collect(Collectors.toCollection(ArrayList::new));
        ArrayList<PackageInfo> data = Arrays.stream(input).map(JSONReference::getData).filter(Objects::nonNull).map(Data::getPackageInfo).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
        if (!data.isEmpty() && !times.isEmpty()) {
            ArrayList<CPUInfo> packageOne = data.stream().map(PackageInfo::getPackageOne).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
            ArrayList<CPUInfo> packageTwo = data.stream().map(PackageInfo::getPackageTwo).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
            processPackageDataSet(tag + " Package One", dataset, packageOne, times);
            processPackageDataSet(tag + " Package Two", dataset, packageTwo, times);
        }
    }

    private void processPackageDataSet(String tag, AbstractIntervalXYDataset dataset, ArrayList<CPUInfo> packageData, ArrayList<Long> times) {
        if (!packageData.isEmpty() && !times.isEmpty() && dataset instanceof  YIntervalSeriesCollection) {
            String packageName = tag + " " + packageData.get(0).getName();
            if (showPackageTotal) {
                if (showEnergyCounter) {
                    ArrayList<CumulativeMeasurement> energyCounterPackage = packageData.stream().map(CPUInfo::getEnergyCounter).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
                    super.processCumulativeMeasurement(packageName + " EnergyCounter", dataset, times, energyCounterPackage);
                    Long total = 0L;
                    for (CPUInfo packageDatum : packageData) {
                        CumulativeLongMeasurement energyCounter = packageDatum.getEnergyCounter();
                        if (energyCounter != null) {
                            total += energyCounter.getAverage();
                        }
                    }
                    System.out.println(tag + " " + total);

                }
                if (showPowerCounter) {
                    ArrayList<CumulativeMeasurement> powerCounterPackage = packageData.stream().map(CPUInfo::getPowerCounter).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
                    super.processCumulativeMeasurement(packageName + " PowerCounter", dataset,  times, powerCounterPackage);
                }
            }

            ArrayList<CPUZone> coreList = packageData.stream().map(CPUInfo::getCore).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
            processCPUZoneDataSet(packageName + " core", dataset, coreList, times, showCore);
            ArrayList<CPUZone> uncoreList = packageData.stream().map(CPUInfo::getUncore).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
            processCPUZoneDataSet(packageName + " Uncore", dataset, uncoreList, times, showUncore);
            ArrayList<CPUZone> dramList = packageData.stream().map(CPUInfo::getDram).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
            processCPUZoneDataSet(packageName + " Dram", dataset, dramList, times, showDram);
        }
    }

    private void processCPUZoneDataSet(String tag, AbstractIntervalXYDataset dataset, ArrayList<CPUZone> cpuZone, ArrayList<Long> times, boolean showZone) {
        if (!cpuZone.isEmpty() && dataset instanceof YIntervalSeriesCollection && showZone) {
            if (showEnergyCounter) {
                ArrayList<CumulativeMeasurement> energyCounterPackage = cpuZone.stream().map(CPUZone::getEnergyCounter).collect(Collectors.toCollection(ArrayList::new));
                super.processCumulativeMeasurement(tag + " EnergyCounter", dataset, times, energyCounterPackage);
            }
            if (showPowerCounter) {
                ArrayList<CumulativeMeasurement> powerCounterPackage = cpuZone.stream().map(CPUZone::getPowerCounter).collect(Collectors.toCollection(ArrayList::new));
                super.processCumulativeMeasurement(tag + " PowerCounter", dataset,  times, powerCounterPackage);
            }

        }

    }


}
