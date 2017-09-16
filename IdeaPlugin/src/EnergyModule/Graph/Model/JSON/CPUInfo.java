package EnergyModule.Graph.Model.JSON;

import com.google.gson.annotations.SerializedName;

/**
 * Created by kd913 on 05/05/17.
 */
public class CPUInfo {

    @SerializedName("Name")
    private String name;

    @SerializedName("EnergyCounter")
    private CumulativeLongMeasurement energyCounter;

    @SerializedName("PowerCounter")
    private CumulativeLongMeasurement powerCounter;

    @SerializedName("EnergyTotal")
    private long energyTotal;


    @SerializedName("core")
    private CPUZone core;

    @SerializedName("uncore")
    private CPUZone uncore;

    @SerializedName("dram")
    private CPUZone dram;

    public CPUInfo() {
    }

    public long getEnergyTotal() {
        return energyTotal;
    }

    public void setEnergyTotal(long energyTotal) {
        this.energyTotal = energyTotal;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
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

    public CPUZone getCore() {
        return core;
    }

    public void setCore(CPUZone core) {
        this.core = core;
    }

    public CPUZone getUncore() {
        return uncore;
    }

    public void setUncore(CPUZone uncore) {
        this.uncore = uncore;
    }

    public CPUZone getDram() {
        return dram;
    }

    public void setDram(CPUZone dram) {
        this.dram = dram;
    }

    String getStackTrace() {
        String result = name + ": {";
        Long energyTotal = this.energyTotal;
        result += "EnergyTotal:" + energyTotal.toString() + ",";
        if (core != null) {
            result += "Core: ";
            result += core.getStackTrace();
            result += ", ";
        }
        if (uncore != null) {
            result += "Unore: ";
            result += uncore.getStackTrace();
            result += ", ";
        }
        if (dram != null) {
            result += "Dram: ";
            result += dram.getStackTrace();
            result += ", ";
        }
        return result;
    }

    String getStackTrace(CPUInfo prev) {
        String result = name + ": {";
        if (prev != null) {
            result += "Energy Total:" + (this.energyTotal - prev.energyTotal) + ",";
            if (core != null && prev.core != null) {
                result += "Core: ";
                result += core.getStackTrace(prev.core);
                result += ", ";
            }
            if (uncore != null && prev.uncore != null) {
                result += "Uncore: ";
                result += uncore.getStackTrace(prev.uncore);
                result += ", ";
            }
            if (dram != null && prev.dram != null) {
                result += "Dram: ";
                result += dram.getStackTrace(prev.dram);
                result += ", ";
            }
        }
        result += "}";
        return  result;
    }
}
