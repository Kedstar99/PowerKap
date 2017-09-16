package EnergyModule.Graph.Model;

/**
 * Created by kd913 on 19/05/17.
 */
public class EnergyAnnotation {

    private long date;
    private String filePath;
    private String fileName;
    private int lineNumber;
    private int callIndex;
    private int interval;

    EnergyAnnotation(long date, String filePath, String fileName, int lineNumber, int callIndex, int interval) {
        this.date = date;
        this.filePath = filePath;
        this.fileName = fileName;
        this.lineNumber = lineNumber;
        this.callIndex = callIndex;
        this.interval = interval;
    }

    public int getInterval() {
        return interval;
    }

    public long getDate() {
        return date;
    }

    void setAvgDate(long date, int sampleSize) {
        this.date += (date - this.date ) / sampleSize ;
    }

    public String toString() {
        return "TimeStamp: " + date + ", File:  " + fileName + ", LineNumber: " +  lineNumber + " ";
    }

    public String getMarker() {
        return fileName + " " + lineNumber;
    }

    public String getFilePath() { return  filePath; }

    public int getLineNumber() {
        return lineNumber;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        EnergyAnnotation that = (EnergyAnnotation) o;

        return date == that.date && lineNumber == that.lineNumber && callIndex == that.callIndex && interval == that.interval && fileName.equals(that.fileName);
    }

    @Override
    public int hashCode() {
        int result = (int) (date ^ (date >>> 32));
        result = 31 * result + fileName.hashCode();
        result = 31 * result + lineNumber;
        result = 31 * result + callIndex;
        result = 31 * result + interval;
        return result;
    }
}
