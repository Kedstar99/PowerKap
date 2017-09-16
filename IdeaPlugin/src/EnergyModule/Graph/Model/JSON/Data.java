package EnergyModule.Graph.Model.JSON;

import com.google.gson.annotations.SerializedName;

/**
 * Created by kd913 on 05/05/17.
 */
public class Data {

    @SerializedName("CPUData")
    private PackageInfo packageInfo;

    @SerializedName("ThermalData")
    private ThermalData thermalData;

    @SerializedName("BatteryData")
    private BatteryData batteryData;

    @SerializedName("DiskData")
    private DiskData diskData;

    @SerializedName("NetworkData")
    private NetworkData networkData;

    public Data() {
    }

    public PackageInfo getPackageInfo() {
        return packageInfo;
    }

    public ThermalData getThermalData() {
        return thermalData;
    }

    public BatteryData getBatteryData() {
        return batteryData;
    }

    public DiskData getDiskData() { return diskData; }

    public NetworkData getNetworkData() {
        return networkData;
    }

    public void setNetworkData(NetworkData networkData) {
        this.networkData = networkData;
    }

    public void setPackageInfo(PackageInfo packageInfo) {
        this.packageInfo = packageInfo;
    }

    public void setThermalData(ThermalData thermalData) {
        this.thermalData = thermalData;
    }

    public void setBatteryData(BatteryData batteryData) {
        this.batteryData = batteryData;
    }

    public void setDiskData(DiskData diskData) { this.diskData = diskData; }

    String getStackTrace(Data prev) {
        String result = "";
        if (prev != null) {
            if (packageInfo != null && prev.packageInfo != null) {
                result += packageInfo.getStackTrace(prev.packageInfo) + " ";
            }
            if (diskData != null && prev.diskData != null) {
                result += diskData.getStackTrace(prev.diskData) + " ";
            }
            if (networkData != null && prev.networkData != null) {
                result += networkData.getStackTrace(prev.networkData) + " ";
            }

        }
        return result;
    }

    String getStackTrace() {
        String result = "";
        if (packageInfo != null) {
            result += packageInfo.getStackTrace() + " ";
        }
        if (diskData != null) {
            result += diskData.getStackTrace() + " ";
        }
        if (networkData != null) {
            result += networkData.getStackTrace() + " ";
        }
        return result;
    }
}
