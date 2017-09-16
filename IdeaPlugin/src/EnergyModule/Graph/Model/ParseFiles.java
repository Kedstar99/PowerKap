package EnergyModule.Graph.Model;

import EnergyModule.Graph.Model.JSON.JSONReference;
import EnergyModule.Graph.Model.JSON.ThermalData;
import EnergyModule.Graph.Model.JSON.ThermalDataDeserializer;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.io.*;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;


public class ParseFiles {

    public static JSONReference[] readJSONFile (File file) throws IOException {
        if (file != null) {
            InputStream input = new FileInputStream(file);
            Reader reader = new InputStreamReader(input, "UTF-8");
            GsonBuilder gsonBuilder = new GsonBuilder();
            gsonBuilder.registerTypeAdapter(ThermalData.class, new ThermalDataDeserializer());
            Gson gson = gsonBuilder.create();
            return gson.fromJson(reader, JSONReference[].class);
        } else {
            throw new IOException("Error reading file");
        }
    }

    public static HashMap<Integer, EnergyAnnotation> readElapsedTimes(File file) throws IOException, ParseException {
        HashMap<Integer, EnergyAnnotation> result = new HashMap<>();
        if (file == null) {
            throw new IOException("Error reading file");
        }
        InputStream input =  new FileInputStream(file);
        BufferedReader in = new BufferedReader(new InputStreamReader(input));
        String line;
        SimpleDateFormat dateformat = new SimpleDateFormat("yyyy:MM:dd HH:mm:ss.SSS");
        Date startTime = null;
        int sampleSize = 1;
        int interval = 1;
        while((line = in.readLine()) != null) {
            if (line.startsWith("#########################RUN")) {
                startTime = null;
                sampleSize++;
            } else {
                String[] splitArray = line.split(", ");
                int callIndex = Integer.parseInt(splitArray[0]);
                Date time = dateformat.parse(splitArray[1]);
                String filePath = splitArray[2];
                String fileName = splitArray[3];
                long diffTime = 0;
                int lineNumber;
                if (startTime == null && filePath.equals("STARTPOINT")) {
                    startTime = time;
                    interval = Integer.parseInt(splitArray[5]);
                    continue;
                } else {
                    if (startTime != null) {
                        diffTime = (time.getTime() - startTime.getTime());
                    }
                    lineNumber = Integer.parseInt(splitArray[4]);
                }
                EnergyAnnotation annotation = new EnergyAnnotation(diffTime, filePath, fileName, lineNumber, callIndex, interval);
                if (result.containsKey(callIndex)) {
                    EnergyAnnotation val = result.get(callIndex);
                    val.setAvgDate(diffTime, sampleSize);
                } else {
                    result.put(callIndex, annotation);
                }

            }
        }
        return result;
    }


}