package EnergyModule.Graph.Model.JSON;

import com.google.gson.annotations.SerializedName;

/**
 * Created by kd913 on 05/05/17.
 */
public class PackageInfo {

    @SerializedName("PackageOne")
    private CPUInfo packageOne;

    @SerializedName("PackageTwo")
    private CPUInfo packageTwo;

    public PackageInfo() {
    }

    public CPUInfo getPackageOne() {
        return packageOne;
    }

    public void setPackageOne(CPUInfo packageOne) {
        this.packageOne = packageOne;
    }

    public CPUInfo getPackageTwo() {
        return packageTwo;
    }

    public void setPackageTwo(CPUInfo packageTwo) {
        this.packageTwo = packageTwo;
    }

    String getStackTrace() {
        String result = "";
        if (packageOne != null) {
            result += packageOne.getStackTrace();
        }
        if (packageTwo != null) {
            result += packageTwo.getStackTrace();
        }
        return result;
    }

    String getStackTrace(PackageInfo prev) {
        String result = "";
        if (packageOne != null && prev.packageOne != null) {
            result += packageOne.getStackTrace(prev.packageOne);
        }
        if (packageTwo != null && prev.packageTwo != null) {
            result += packageTwo.getStackTrace(prev.packageTwo);
        }
        return result;
    }

}
