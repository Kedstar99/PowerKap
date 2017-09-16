package EnergyModule.Graph.Model;

import EnergyModule.Graph.Model.JSON.CumulativeMeasurement;
import EnergyModule.Graph.Model.JSON.JSONReference;
import org.jfree.data.xy.AbstractIntervalXYDataset;
import org.jfree.data.xy.YIntervalSeries;
import org.jfree.data.xy.YIntervalSeriesCollection;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.stream.Collectors;

/**
 * Created by kd913 on 06/05/17.
 */
abstract class ProcessMeasurements {

    HashMap<String, JSONReference[]> measurements;

    ProcessMeasurements(HashMap<String, JSONReference[]> values) {
        this.measurements = values;
    }

    public AbstractIntervalXYDataset produceDataset() {
        YIntervalSeriesCollection dataset = new YIntervalSeriesCollection();
        for (String key: measurements.keySet()) {
            processDataset(key + " ", measurements.get(key), dataset);
        }
        return dataset;
    }

    public abstract void processDataset(String tag, JSONReference[] input, AbstractIntervalXYDataset dataset);


    private void processCumulativeMeasurementDataSet(YIntervalSeries set, ArrayList<CumulativeMeasurement> measurements, ArrayList<? extends Number> times){
        if (!measurements.isEmpty() && !times.isEmpty()) {
            ArrayList<? extends Number> stdDev = measurements.stream().map(CumulativeMeasurement::getStdDev).collect(Collectors.toCollection(ArrayList::new));
            for (int i = 0; i < times.size(); i++) {
                int time = times.get(i).intValue();
                set.add(time, measurements.get(i).getAverage().doubleValue(), measurements.get(i).getMinFromStdDev(stdDev.get(i)), measurements.get(i).getMaxFromStdDev(stdDev.get(i)) );
            }
        }
    }

    void processCumulativeMeasurement(String tag, AbstractIntervalXYDataset dataset, ArrayList<Long> times, ArrayList<CumulativeMeasurement> data) {
        if (!data.isEmpty() && dataset instanceof YIntervalSeriesCollection) {
            YIntervalSeries powerSeries = new YIntervalSeries(tag);
            this.processCumulativeMeasurementDataSet(powerSeries, data, times);
            YIntervalSeriesCollection coll = (YIntervalSeriesCollection) dataset;
            coll.addSeries(powerSeries);
        }
    }


}
