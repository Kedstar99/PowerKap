package EnergyModule.Graph.Model.JSON;

import com.google.gson.*;

import java.lang.reflect.Type;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by kd913 on 05/05/17.
 */
public class ThermalData  {

    public Map<String, Long> getThermalData() {
        return thermalData;
    }

    public void setThermalData(Map<String, Long> thermalData) {
        this.thermalData = thermalData;
    }

    Map<String, Long> thermalData = new HashMap<>();

    public ThermalData(Map<String, Long> thermalData) {
        this.thermalData = thermalData;
    }

}

