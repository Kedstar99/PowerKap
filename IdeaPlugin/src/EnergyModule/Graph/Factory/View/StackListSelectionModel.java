package EnergyModule.Graph.Factory.View;

import javax.swing.*;

/**
 * Created by kd913 on 24/06/17.
 */
public class StackListSelectionModel extends DefaultListSelectionModel {

    private static final long serialVersionUID = 1L;

    public StackListSelectionModel() {
        setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
    }

    @Override
    public void setSelectionInterval(int index0, int index1) {
            if (isSelectedIndex(index0)) {
                super.removeSelectionInterval(index0, index1);
            }
            else {
                super.setSelectionInterval(index0, index1);
            }
    }

    @Override
    public void clearSelection() {
        //do nothing
    }

    public void resetSelection() {
        this.removeSelectionInterval(this.getMinSelectionIndex(), this.getMaxSelectionIndex());
    }
}
