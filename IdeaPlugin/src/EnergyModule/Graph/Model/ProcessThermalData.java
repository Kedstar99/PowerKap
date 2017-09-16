package EnergyModule.Graph.Model;

import EnergyModule.Graph.Model.JSON.*;
import org.jfree.data.xy.AbstractIntervalXYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import java.util.*;
import java.util.stream.Collectors;

/**
 * Created by kd913 on 06/05/17.
 */
public class ProcessThermalData extends ProcessMeasurements implements ProcessData {

    public ProcessThermalData(HashMap<String, JSONReference[]> values) {
        super(values);
    }

    @Override
    public AbstractIntervalXYDataset produceDataset() {
        XYSeriesCollection result = new XYSeriesCollection();
        for (String key: measurements.keySet()) {
            processDataset(key, measurements.get(key), result);
        }
        return result;
    }

    @Override
    public void processDataset(String tag, JSONReference[] input, AbstractIntervalXYDataset dataset) {
        if (dataset instanceof XYSeriesCollection) {
            ArrayList<Long> times = Arrays.stream(input).map(JSONReference::getTime).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
            ArrayList<ThermalData> data = Arrays.stream(input).map(JSONReference::getData).filter(Objects::nonNull).map(Data::getThermalData).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
            if (!data.isEmpty() && !times.isEmpty()) {
                Set<String> thData = data.get(0).getThermalData().keySet();
                ArrayList<XYSeries> xySeriesArray = new ArrayList<>();
                for(String s: thData) {
                    xySeriesArray.add(new XYSeries(tag + " " + s));
                }

                for(int i = 0; i < times.size(); i++) {
                    for (XYSeries aXySeriesArray : xySeriesArray) {
                        String key = (String) aXySeriesArray.getKey();
                        key = key.substring(key.lastIndexOf(" ") + 1);
                        aXySeriesArray.add(times.get(i), data.get(i).getThermalData().get(key));
                    }
                }

                XYSeriesCollection result = (XYSeriesCollection) dataset;
                for (XYSeries aXySeriesArray : xySeriesArray) {
                    result.addSeries(aXySeriesArray);
                }
            }

        }

    }

}
