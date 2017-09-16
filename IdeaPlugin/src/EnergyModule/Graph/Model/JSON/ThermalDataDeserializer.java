package EnergyModule.Graph.Model.JSON;

import com.google.gson.*;

import java.lang.reflect.Type;
import java.util.HashMap;
import java.util.Map;

    public class ThermalDataDeserializer implements JsonDeserializer<ThermalData> {

        public ThermalDataDeserializer() {
        }

        @Override
        public ThermalData deserialize(JsonElement jsonElement, Type type, JsonDeserializationContext jsonDeserializationContext) throws JsonParseException {
            if (!jsonElement.isJsonObject()) throw new JsonParseException("JSON deserializing thermal info failed");
            Map<String, Long> thermalData = new HashMap<>();
            JsonObject jsonObject = jsonElement.getAsJsonObject();
            for (Map.Entry<String, JsonElement> entry : jsonObject.entrySet()) {
                String key = entry.getKey();
                JsonElement value = entry.getValue();
                if (value.isJsonPrimitive()) {
                    thermalData.put(key, value.getAsLong());
                } else {
                    throw new JsonParseException("JSON deserializing thermal info failed");
                }

            }
            return new ThermalData(thermalData);
        }
    }
