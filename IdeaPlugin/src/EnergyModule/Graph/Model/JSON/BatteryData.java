package EnergyModule.Graph.Model.JSON;

import com.google.gson.annotations.SerializedName;

/**
 * Created by kd913 on 05/05/17.
 */
public class BatteryData {

    @SerializedName("MicroVoltage")
    private CumulativeLongMeasurement microVoltage;

    @SerializedName("MicroCurrent")
    private CumulativeLongMeasurement microCurrent;

    @SerializedName("Power")
    private CumulativeDoubleMeasurement power;

    public BatteryData() {
    }

    public CumulativeLongMeasurement getMicroVoltage() {
        return microVoltage;
    }

    public void setMicroVoltage(CumulativeLongMeasurement microVoltage) { this.microVoltage = microVoltage; }

    public CumulativeLongMeasurement getMicroCurrent() {
        return microCurrent;
    }

    public void setMicroCurrent(CumulativeLongMeasurement microCurrent) {
        this.microCurrent = microCurrent;
    }

    public CumulativeDoubleMeasurement getPicoPower() {
        return power;
    }

    public void setPicoPower(CumulativeDoubleMeasurement picoPower) {
        this.power = picoPower;
    }
}
