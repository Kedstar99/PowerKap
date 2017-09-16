package EnergyModule.Graph.Model.JSON;

import com.google.gson.annotations.SerializedName;

/**
 * Created by kd913 on 05/05/17.
 */
public class CPUZone {

    @SerializedName("EnergyCounter")
    private CumulativeLongMeasurement energyCounter;

    @SerializedName("PowerCounter")
    private CumulativeLongMeasurement powerCounter;

    @SerializedName("EnergyTotal")
    private long energyTotal;

    public CPUZone() {
    }

    public long getEnergyTotal() {
        return energyTotal;
    }

    public void setEnergyTotal(long energyTotal) {
        this.energyTotal = energyTotal;
    }

    public CumulativeLongMeasurement getEnergyCounter() {
        return energyCounter;
    }

    public void setEnergyCounter(CumulativeLongMeasurement energyCounter) {
        this.energyCounter = energyCounter;
    }

    public CumulativeLongMeasurement getPowerCounter() {
        return powerCounter;
    }

    public void setPowerCounter(CumulativeLongMeasurement powerCounter) {
        this.powerCounter = powerCounter;
    }

    String getStackTrace(CPUZone prev) {
        Long energyVal = energyTotal;
        if (prev != null) {
            energyVal -= prev.energyTotal;
        }
        return energyVal.toString();
    }

    String getStackTrace() {
        Long energyVal = energyTotal;
        return energyVal.toString();
    }

}
