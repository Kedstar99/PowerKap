package EnergyModule.Graph.Model;

import EnergyModule.Graph.Model.JSON.BatteryData;
import EnergyModule.Graph.Model.JSON.CumulativeMeasurement;
import EnergyModule.Graph.Model.JSON.Data;
import EnergyModule.Graph.Model.JSON.JSONReference;
import org.jfree.data.xy.AbstractIntervalXYDataset;
import org.jfree.data.xy.YIntervalSeriesCollection;

import java.util.*;
import java.util.stream.Collectors;

/**
 * Created by kd913 on 07/05/17.
 */
public class ProcessBatteryData extends ProcessMeasurements {

    private boolean showMicroVoltage;
    private boolean showMicroCurrent;
    private boolean showPower;

    public ProcessBatteryData(HashMap<String, JSONReference[]> values, boolean showMicroVoltage, boolean showMicroCurrent, boolean showPower) {
        super(values);
        this.showMicroVoltage = showMicroVoltage;
        this.showMicroCurrent = showMicroCurrent;
        this.showPower = showPower;
    }



    @Override
    public void processDataset(String tag, JSONReference[] input, AbstractIntervalXYDataset dataset) {
        if (dataset instanceof YIntervalSeriesCollection) {
            ArrayList<Long> times = Arrays.stream(input).map(JSONReference::getTime).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
            ArrayList<BatteryData> data = Arrays.stream(input).map(JSONReference::getData).filter(Objects::nonNull).map(Data::getBatteryData).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
            if (!data.isEmpty() && !times.isEmpty()) {
                if (showMicroVoltage) {
                    ArrayList<CumulativeMeasurement> microVoltages = data.stream().map(BatteryData::getMicroVoltage).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
                    super.processCumulativeMeasurement(tag + "microVoltages", dataset, times, microVoltages);
                }

                if (showMicroCurrent) {
                    ArrayList<CumulativeMeasurement> microCurrents = data.stream().map(BatteryData::getMicroCurrent).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
                    super.processCumulativeMeasurement(tag + "microCurrent", dataset, times, microCurrents );
                }

                if (showPower) {
                    ArrayList<CumulativeMeasurement> powerList = data.stream().map(BatteryData::getPicoPower).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
                    super.processCumulativeMeasurement(tag + "power", dataset, times, powerList);
                    OptionalDouble avg = data.stream().map(BatteryData::getPicoPower).filter(Objects::nonNull).map(CumulativeMeasurement::getAverage).mapToDouble(Double::doubleValue).average();
                    System.out.println(tag + " " + avg.getAsDouble());

                }

            }

        }
    }
}
