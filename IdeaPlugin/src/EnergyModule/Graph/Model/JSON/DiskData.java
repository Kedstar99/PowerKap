package EnergyModule.Graph.Model.JSON;

import com.google.gson.annotations.SerializedName;

/**
 * Created by kd913 on 25/05/17.
 */
public class DiskData
{
    @SerializedName("ReadBytes")
    private CumulativeLongMeasurement readBytes;

    @SerializedName("WriteBytes")
    private CumulativeLongMeasurement writeBytes;

    @SerializedName("ReadTotal")
    private long readTotal;

    @SerializedName("WriteTotal")
    private long writeTotal;

    public DiskData() {
    }

    public CumulativeLongMeasurement getReadBytes() {
        return readBytes;
    }

    public void setReadBytes(CumulativeLongMeasurement readBytes) {
        this.readBytes = readBytes;
    }

    public CumulativeLongMeasurement getWriteBytes() {
        return writeBytes;
    }

    public void setWriteBytes(CumulativeLongMeasurement writeBytes) {
        this.writeBytes = writeBytes;
    }

    public long getReadTotal() {
        return readTotal;
    }

    public void setReadTotal(long readTotal) {
        this.readTotal = readTotal;
    }

    public long getWriteTotal() {
        return writeTotal;
    }

    public void setWriteTotal(long writeTotal) {
        this.writeTotal = writeTotal;
    }

    String getStackTrace(DiskData diskData) {
        String result = "";
        Long read = readTotal;
        Long write = writeTotal;
        if (diskData != null) {
            read = readTotal - diskData.readTotal;
            write = writeTotal - diskData.writeTotal;
        }
        result += "Read Total: " + read.toString() + " ";
        result += "Write Total: " + write.toString() + " ";

        return result;
    }

    String getStackTrace() {
        return this.getStackTrace(null);
    }
}
