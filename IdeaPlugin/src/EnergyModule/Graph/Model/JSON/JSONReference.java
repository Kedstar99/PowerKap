package EnergyModule.Graph.Model.JSON;

import com.google.gson.annotations.SerializedName;

/**
 * Created by kd913 on 05/05/17.
 */
public class JSONReference {
    @SerializedName("Data")
    private Data data;

    @SerializedName("ElapsedTime (ms)")
    private long time;

    public JSONReference() {
    }

    public Data getData() {
        return data;
    }

    public void setData(Data data) {
        this.data = data;
    }

    public long getTime() {
        return time;
    }

    public void setTime(long time) {
        this.time = time;
    }

    public String getStackTrace(JSONReference prev) {
        String result = "";
        if (data != null && prev.data != null) {
            result += "SampleTime: " + time + ", ";
            result += data.getStackTrace(prev.data);
        }
        return  result;
    }

    public String getStackTrace() {
        String result = "";
        if (data != null) {
            result += "SampleTime: " + time + ", ";
            result += data.getStackTrace();
        }
        return  result;
    }


}
