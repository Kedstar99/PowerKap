package EnergyModule.Graph.Model.JSON;

import com.google.gson.annotations.SerializedName;

/**
 * Created by kd913 on 05/05/17.
 */
public class CumulativeLongMeasurement implements CumulativeMeasurement<Long> {

    @SerializedName("avg")
    private long average;

    @SerializedName("max")
    private long max;

    @SerializedName("min")
    private long min;

    @SerializedName("stddev")
    private long stdDev;

    public CumulativeLongMeasurement() {
    }

    public Long getAverage() {
        return average;
    }

    public void setAverage(long average) {
        this.average = average;
    }

    public Long getMax() {
        return max;
    }

    public void setMax(long max) {
        this.max = max;
    }

    public Long getMin() {
        return min;
    }

    public void setMin(long min) {
        this.min = min;
    }

    public Long getStdDev() {
        return stdDev;
    }

    public void setStdDev(long stdDev) {
        this.stdDev = stdDev;
    }

    public double getMaxFromStdDev(Number elem) {
        return  (double) (average + elem.longValue());
    }

    public double getMinFromStdDev(Number elem) {
        return (double) (average - elem.longValue());
    }
}
