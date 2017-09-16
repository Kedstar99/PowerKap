package EnergyModule.Graph.Model.JSON;

/**
 * Created by kd913 on 29/05/17.
 */
public interface CumulativeMeasurement<T extends Number> {

    T getAverage();

    T getMax();

    T getMin();

    T getStdDev();

    double getMaxFromStdDev(Number elem);

    double getMinFromStdDev(Number elem);

}


