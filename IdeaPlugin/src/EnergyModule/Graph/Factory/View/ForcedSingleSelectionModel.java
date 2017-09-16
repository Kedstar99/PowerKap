package EnergyModule.Graph.Factory.View;

import javax.swing.*;

/**
 * Created by kd913 on 08/06/17.
 */
public class ForcedSingleSelectionModel extends DefaultListSelectionModel {


    public ForcedSingleSelectionModel() {
        setSelectionMode(ForcedSingleSelectionModel.SINGLE_SELECTION);
    }

    @Override
    public void clearSelection() {
    }

    @Override
    public void removeSelectionInterval(int index0, int index1) {
    }

}