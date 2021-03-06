package TimeKeeper.timeKeeper;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by kd913 on 22/05/17.
 */
public class TimeKeeper {
    private static List<EnergyAnnotation> annotations = new ArrayList<>();
    private static int callIndex = 0;

    public static void insertTimePoint(String className, int lineNumber) {
        callIndex++;
        annotations.add(new EnergyAnnotation(new Date(), className, lineNumber, callIndex));
    }

    public static void printTimePoints() {
        try {
            Writer w = new FileWriter("elapsedtimes.txt", true);
            if (!annotations.isEmpty()) {
                for (EnergyAnnotation annotation : annotations) {
                    w.write(annotation.toString());
                }
                w.close();
                annotations.clear();
            }
        } catch (IOException e) {

        }
    }

    static class EnergyAnnotation {
        Date date;
        String className;
        int lineNumber;
        int callIndex;

        EnergyAnnotation(Date date, String className, int lineNumber, int callIndex) {
            this.date = date;
            this.className = className;
            this.lineNumber = lineNumber;
            this.callIndex = callIndex;
        }

        public String toString() {
            SimpleDateFormat dateformat = new SimpleDateFormat("yyyy:MM:dd HH:mm:ss.SSS");
            return callIndex + ", " + dateformat.format(date) + ", " + className + ", " + lineNumber + "\n";
        }
    }
}
