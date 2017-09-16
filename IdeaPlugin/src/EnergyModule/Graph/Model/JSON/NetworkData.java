package EnergyModule.Graph.Model.JSON;

import com.google.gson.annotations.SerializedName;

/**
 * Created by kd913 on 01/06/17.
 */
public class NetworkData {

    @SerializedName("ReceivedBytes")
    private CumulativeLongMeasurement receivedBytes;

    @SerializedName("SentBytes")
    private CumulativeLongMeasurement sentBytes;

    @SerializedName("ReceivedTotal")
    private long totalReceived;

    @SerializedName("SentTotal")
    private long totalSent;

    public NetworkData() {
    }

    public CumulativeLongMeasurement getReceivedBytes() {
        return receivedBytes;
    }

    public void setReceivedBytes(CumulativeLongMeasurement receivedBytes) {
        this.receivedBytes = receivedBytes;
    }

    public CumulativeLongMeasurement getSentBytes() {
        return sentBytes;
    }

    public void setSentBytes(CumulativeLongMeasurement sentBytes) {
        this.sentBytes = sentBytes;
    }

    public long getTotalReceived() {
        return totalReceived;
    }

    public void setTotalReceived(long totalReceived) {
        this.totalReceived = totalReceived;
    }

    public long getTotalSent() {
        return totalSent;
    }

    public void setTotalSent(long totalSent) {
        this.totalSent = totalSent;
    }

    String getStackTrace(NetworkData networkData) {
        String result = "";
        Long read = totalReceived;
        Long write = totalSent;

        if (networkData != null) {
            read = totalReceived - networkData.totalReceived;
            write = totalSent - networkData.totalSent;
        }
        result += "Received Total(bytes): " + read.toString() + " ";
        result += "Sent Total(bytes): " + write.toString() + " ";

        return result;
    }

    String getStackTrace() {
        return this.getStackTrace(null);
    }
}
