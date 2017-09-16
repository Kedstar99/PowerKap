package EnergyModule.Graph.Model;

import EnergyModule.Graph.Model.JSON.*;
import org.jfree.data.xy.AbstractIntervalXYDataset;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Objects;
import java.util.stream.Collectors;

/**
 * Created by kd913 on 25/05/17.
 */
public class ProcessNetworkData extends ProcessMeasurements {

    private boolean showSentBytes;
    private boolean showReceivedBytes;

    public ProcessNetworkData(HashMap<String, JSONReference[]> values, boolean sentBytes, boolean receivedBytes) {
        super(values);
        this.showSentBytes = sentBytes;
        this.showReceivedBytes = receivedBytes;
    }


    @Override
    public void processDataset(String tag, JSONReference[] input, AbstractIntervalXYDataset dataset) {
        ArrayList<Long> times = Arrays.stream(input).map(JSONReference::getTime).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
        ArrayList<NetworkData> networkData = Arrays.stream(input).map(JSONReference::getData).filter(Objects::nonNull).map(Data::getNetworkData).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
        if (!times.isEmpty() && !networkData.isEmpty()) {
            if (showReceivedBytes) {
                ArrayList<CumulativeMeasurement> bytesReceived = networkData.stream().map(NetworkData::getReceivedBytes).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
                super.processCumulativeMeasurement(tag + " bytesReceived", dataset, times, bytesReceived);
            }
            if (showSentBytes) {
                ArrayList<CumulativeMeasurement> bytesSent = networkData.stream().map(NetworkData::getSentBytes).filter(Objects::nonNull).collect(Collectors.toCollection(ArrayList::new));
                super.processCumulativeMeasurement(tag + " bytesSent", dataset,  times, bytesSent);
            }

        }
    }

}
