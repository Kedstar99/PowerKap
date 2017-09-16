package EnergyModule.Graph.Model;

import EnergyModule.Graph.Model.JSON.CumulativeMeasurement;
import EnergyModule.Graph.Model.JSON.Data;
import EnergyModule.Graph.Model.JSON.DiskData;
import EnergyModule.Graph.Model.JSON.JSONReference;
import org.jfree.data.xy.AbstractIntervalXYDataset;
import org.jfree.data.xy.YIntervalSeriesCollection;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Objects;
import java.util.stream.Collectors;

/**
 * Created by kd913 on 25/05/17.
 */
public class ProcessDiskData extends ProcessMeasurements {

    private boolean showWriteBytes;
    private boolean showReadBytes;

    public ProcessDiskData(HashMap<String, JSONReference[]> values, boolean showWriteBytes, boolean showReadBytes) {
        super(values);
        this.showWriteBytes = showWriteBytes;
        this.showReadBytes = showReadBytes;
    }

    @Override
    public void processDataset(String tag, JSONReference[] input, AbstractIntervalXYDataset dataset) {
        if (dataset instanceof YIntervalSeriesCollection) {
            ArrayList<Long> times = Arrays.stream(input).map(JSONReference::getTime).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
            ArrayList<DiskData> diskData = Arrays.stream(input).map(JSONReference::getData).filter(Objects::nonNull).map(Data::getDiskData).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
            if (!times.isEmpty() && !diskData.isEmpty()) {
                if (showReadBytes) {
                    ArrayList<CumulativeMeasurement> bytesRead = diskData.stream().map(DiskData::getReadBytes).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
                    super.processCumulativeMeasurement(tag+ " bytesRead", dataset, times, bytesRead);
                }
                if (showWriteBytes) {
                    ArrayList<CumulativeMeasurement> bytesWritten = diskData.stream().map(DiskData::getWriteBytes).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
                    super.processCumulativeMeasurement(tag+ " bytesWritten", dataset, times, bytesWritten);
                }


            }

        }
    }
}
