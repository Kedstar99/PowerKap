package EnergyModule.Graph.Model.JSON;

import com.google.gson.annotations.SerializedName;

/**
 * Created by kd913 on 29/05/17.
 */
public class CumulativeDoubleMeasurement implements CumulativeMeasurement<Double> {

    @SerializedName("avg")
    private double average;

    @SerializedName("max")
    private double max;

    @SerializedName("min")
    private double min;

    @SerializedName("stddev")
    private double stdDev;

    public CumulativeDoubleMeasurement() {
    }

    public Double getAverage() {
        return average;
    }

    public void setAverage(double average) {
        this.average = average;
    }

    public Double getMax() {
        return max;
    }

    public void setMax(double max) {
        this.max = max;
    }

    public Double getMin() {
        return min;
    }

    public void setMin(double min) {
        this.min = min;
    }

    public Double getStdDev() {
        return stdDev;
    }

    public void setStdDev(double stdDev) {
        this.stdDev = stdDev;
    }


    public double getMaxFromStdDev(Number elem) {
        return average + elem.doubleValue();
    }

    public double getMinFromStdDev(Number elem) {
        return average - elem.doubleValue();
    }
}
