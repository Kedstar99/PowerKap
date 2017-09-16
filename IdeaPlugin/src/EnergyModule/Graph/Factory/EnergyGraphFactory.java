package EnergyModule.Graph.Factory;

import EnergyModule.Graph.Factory.View.ForcedSingleSelectionModel;
import EnergyModule.Graph.Factory.View.StackListSelectionModel;
import EnergyModule.Graph.Model.*;
import EnergyModule.Graph.Model.JSON.JSONReference;
import com.intellij.openapi.editor.Editor;
import com.intellij.openapi.fileEditor.FileEditorManager;
import com.intellij.openapi.fileEditor.OpenFileDescriptor;
import com.intellij.openapi.project.Project;
import com.intellij.openapi.project.ProjectManager;
import com.intellij.openapi.vfs.VirtualFile;
import com.intellij.openapi.vfs.VirtualFileManager;
import com.intellij.openapi.wm.ToolWindow;
import com.intellij.openapi.wm.ToolWindowFactory;
import com.intellij.ui.JBColor;
import com.intellij.ui.content.Content;
import com.intellij.ui.content.ContentFactory;
import org.jetbrains.annotations.NotNull;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.plot.Marker;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.ValueMarker;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.DeviationRenderer;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.YIntervalSeriesCollection;
import org.jfree.ui.RectangleAnchor;
import org.jfree.ui.TextAnchor;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.IOException;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Vector;


public class EnergyGraphFactory implements ToolWindowFactory{

    private JPanel myToolWindowContent;
    private JTabbedPane GraphTabbedPane;
    private JButton refreshButton;
    private JCheckBox coreCheckBox;
    private JCheckBox uncoreCheckBox;
    private JCheckBox dramCheckBox;
    private JPanel EnergyGraphArea;
    private JRadioButton microVoltageRadioButton;
    private JRadioButton powerRadioButton;
    private JRadioButton microCurrentRadioButton;
    private JPanel BatteryGraphArea;
    private JRadioButton powerCounterRadioButton;
    private JRadioButton energyCounterRadioButton;
    private JCheckBox packageTotalCheckBox;
    private JList<String> stackList;
    private JPanel DiskGraphArea;
    private JPanel TemperatureGraphArea;
    private JPanel NetworkGraphArea;
    private JButton uploadButton;
    private JTable fileTable;
    private JButton clearSelectionButton;
    private JButton uploadElapsedTimesButton;
    private JLabel elapsedTimesLabel;
    private JCheckBox readBytesCheckBox;
    private JCheckBox writeBytesCheckBox;
    private JCheckBox sentCheckBox;
    private JCheckBox receivedCheckBox;
    private HashMap<Integer, EnergyAnnotation> annotationHashMap;
    private ArrayList<Integer> stackTraceIndexes = new ArrayList<>();
    private ToolWindow myToolWindow;

    private HashMap<String, JSONReference[]> fileList = new HashMap<>();

    public EnergyGraphFactory() {
        refreshButton.addActionListener(e -> {
            HashMap<String, JSONReference[]> chosenFiles = getEnabledFiles();
            if (!chosenFiles.isEmpty()) {
                loadGraphs(stackList.getSelectedIndices(), chosenFiles);
                if (annotationHashMap != null) {
                    loadStackTrace();
                }
            }
        });
        microVoltageRadioButton.addActionListener(actionEvent -> {
            microCurrentRadioButton.setSelected(false);
            powerRadioButton.setSelected(false);
        });

        microCurrentRadioButton.addActionListener(actionEvent ->  {
            microVoltageRadioButton.setSelected(false);
            powerRadioButton.setSelected(false);
        });

        powerRadioButton.addActionListener(actionEvent -> {
            microCurrentRadioButton.setSelected(false);
            microVoltageRadioButton.setSelected(false);
        });
        powerCounterRadioButton.addActionListener(actionEvent -> energyCounterRadioButton.setSelected(false));
        energyCounterRadioButton.addActionListener(actionEvent -> powerCounterRadioButton.setSelected(false));

        stackList.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent mouseEvent) {
                if (mouseEvent.isPopupTrigger()) {
                    stackList.setSelectedIndex(stackList.locationToIndex(mouseEvent.getPoint()));
                    JPopupMenu menu = new JPopupMenu();
                    JMenuItem item = new JMenuItem("Go To Energy Point");
                    item.addActionListener(actionEvent -> {
                        int index = stackList.locationToIndex(mouseEvent.getPoint());
                        EnergyAnnotation annotation = annotationHashMap.get(stackTraceIndexes.get(index));
                        String filePath = annotation.getFilePath();
                        int lineNumber = annotation.getLineNumber();
                        if (ProjectManager.getInstance().getOpenProjects().length > 0) {
                            Project project = ProjectManager.getInstance().getOpenProjects()[0];
                            VirtualFile file = VirtualFileManager.getInstance().findFileByUrl(filePath);
                            if (file != null) {
                                OpenFileDescriptor descriptor = new OpenFileDescriptor(project,file );
                                FileEditorManager.getInstance(project).openTextEditor(descriptor, true);
                                Editor editor = FileEditorManager.getInstance(project).getSelectedTextEditor();
                                if (editor != null) {
                                    if (lineNumber > 1) {
                                        editor.getCaretModel().moveToOffset(editor.getDocument().getLineEndOffset(lineNumber - 1));
                                    }
                                }
                            }

                        }

                    });
                    menu.add(item);
                    menu.show(stackList, mouseEvent.getPoint().x, mouseEvent.getPoint().y);
                }
            }
        });

        uploadButton.addActionListener(actionEvent -> uploadJSONFunction());
        uploadElapsedTimesButton.addActionListener(actionEvent -> uploadElapsedTimesFunction());
        clearSelectionButton.addActionListener(actionEvent -> clearData());
    }

    private void uploadJSONFunction() {
        JFileChooser fileChooser = new JFileChooser();
        int retVal = fileChooser.showOpenDialog(uploadButton);
        if (retVal == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                JSONReference[] result = ParseFiles.readJSONFile(file);
                if (result.length > 0) {
                    if (!fileList.containsKey(file.getAbsolutePath())) {
                        fileList.put(file.getAbsolutePath(), result);
                    }
                    addToFileTable(file.getAbsolutePath());
                }

            } catch (IOException e) {
                spawnErrorDialog("Error file is not acceptable for interpreting!");
            }
        }
    }

    private void uploadElapsedTimesFunction() {
        JFileChooser fileChooser = new JFileChooser();
        int retVal = fileChooser.showOpenDialog(uploadButton);
        if (retVal == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                annotationHashMap =  ParseFiles.readElapsedTimes(file);
                elapsedTimesLabel.setText(file.getAbsolutePath());
                elapsedTimesLabel.validate();
            } catch (ParseException e) {
                spawnErrorDialog("Error file is not acceptable for interpreting");
            } catch (IOException e) {
                spawnErrorDialog("Error file can't be opened");
            }

        }
    }

    private void clearData() {
        if (fileList != null) {
            fileList.clear();
        }
        if (annotationHashMap != null) {
            annotationHashMap.clear();
        }
        if (stackTraceIndexes != null) {
            stackTraceIndexes.clear();
        }
        elapsedTimesLabel.setText("N/A");
        elapsedTimesLabel.validate();
        DefaultTableModel model = (DefaultTableModel) fileTable.getModel();
        model.setRowCount(0);
        fileTable.validate();
        stackList.setListData(new Vector<>());
        StackListSelectionModel selectionModel = (StackListSelectionModel) stackList.getSelectionModel();
        selectionModel.resetSelection();
        stackList.validate();
    }

    private HashMap<String, JSONReference[]> getEnabledFiles() {
        HashMap<String, JSONReference[]> result = new HashMap<>();
        DefaultTableModel model = (DefaultTableModel) fileTable.getModel();
        int row = model.getRowCount();
        int STRINGINDEX = 0;
        int BOOLEANINDEX = 1;
        for (int i = 0; i < row; i++) {
            if ((Boolean) model.getValueAt(i, BOOLEANINDEX)) {
                String key = (String) model.getValueAt(i, STRINGINDEX);
                result.put(key, fileList.get(key));
            }
        }
        return  result;
    }

    private JSONReference[] getProgramResult() {
        int row = fileTable.getSelectedRow();
        String key = (String) fileTable.getValueAt(row, 0);
        return fileList.get(key);
    }


    // Create the tool window content.
    public void createToolWindowContent(@NotNull Project project, @NotNull ToolWindow toolWindow) {
        myToolWindow = toolWindow;
        stackList.setBackground(myToolWindowContent.getForeground());
        stackList.setForeground(myToolWindowContent.getBackground());
        StackListSelectionModel selectionModel = new StackListSelectionModel();
        stackList.setSelectionModel(selectionModel);

        EnergyGraphArea.setLayout(new java.awt.BorderLayout());
        TemperatureGraphArea.setLayout(new java.awt.BorderLayout());
        BatteryGraphArea.setLayout(new java.awt.BorderLayout());
        DiskGraphArea.setLayout(new java.awt.BorderLayout());
        NetworkGraphArea.setLayout(new java.awt.BorderLayout());
        energyCounterRadioButton.setSelected(true);
        ContentFactory contentFactory = ContentFactory.SERVICE.getInstance();
        Content content = contentFactory.createContent(myToolWindowContent, "", false);
        toolWindow.getContentManager().addContent(content);
    }

    private void loadStackTrace() {
        Vector<String> result = new Vector<>();
        JSONReference prev = null;
        String oldStacktrace = "";
        stackTraceIndexes.clear();
        JSONReference[] programMeasurements = getProgramResult();
        for (Integer key: annotationHashMap.keySet()) {
            long time = annotationHashMap.get(key).getDate();
            int interval = annotationHashMap.get(key).getInterval();
            JSONReference program = getMeasurement(programMeasurements, time, interval);

            if (program != null) {
                if (prev == null) {
                    oldStacktrace = program.getStackTrace();
                    result.add(key + " " + annotationHashMap.get(key) + oldStacktrace);
                    stackTraceIndexes.add(key);
                    prev = program;
                } else {
                    if (prev.getTime() == program.getTime()) {
                        result.add(key + " " + annotationHashMap.get(key) + oldStacktrace);
                        stackTraceIndexes.add(key);
                    } else {
                        result.add(key + " " + annotationHashMap.get(key) + program.getStackTrace(prev));
                        stackTraceIndexes.add(key);
                        oldStacktrace = program.getStackTrace();
                        prev = program;
                    }
                }
            }

        }
        stackList.setListData(result);
        stackList.validate();
    }

    private  void addToFileTable(String name) {
        DefaultTableModel model = (DefaultTableModel) fileTable.getModel();
        model.addRow(new Object[]{name, false});
        fileTable.setRowSelectionInterval(0,0);
        fileTable.validate();
    }


    private void loadGraphs(int[] selectedIndices, HashMap<String, JSONReference[]> values) {
        loadCPUGraphs(selectedIndices, values);
        loadBatteryGraphs(selectedIndices, values);
        loadThermalGraphs(selectedIndices, values);
        loadDataGraphs(selectedIndices, values);
        loadNetworkGraphs(selectedIndices, values);
        GraphTabbedPane.validate();
    }

    private RectangleAnchor getAnchor(int i) {
        if (i % 2 == 0) {
            return RectangleAnchor.TOP;
        } else {
            return RectangleAnchor.CENTER;
        }
    }

    private void markPlot(JFreeChart xyPlot, int[] selectedIndices) {
        JSONReference[] programMeasurements = getProgramResult();
        if (annotationHashMap != null && stackTraceIndexes != null) {
            if (!annotationHashMap.isEmpty() && !stackTraceIndexes.isEmpty()) {
                for (Integer elem: selectedIndices) {
                    EnergyAnnotation annotation = annotationHashMap.get(stackTraceIndexes.get(elem));

                    JSONReference ref = getMeasurement(programMeasurements, annotation.getDate(), annotation.getInterval());
                    if (ref != null) {

                        final Marker marker = new ValueMarker(ref.getTime());
                        marker.setPaint(JBColor.GREEN);
                        marker.setStroke(new BasicStroke(5.0f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                        marker.setLabel(annotation.getMarker());
                        marker.setLabelAnchor(getAnchor(elem));
                        marker.setLabelTextAnchor(TextAnchor.CENTER);
                        xyPlot.getXYPlot().addDomainMarker(marker);
                    }
                }

            }
        }
    }

    private void loadCPUGraphs(int[] selectedIndices, HashMap<String, JSONReference[]> values) {
        BorderLayout layout = (BorderLayout) EnergyGraphArea.getLayout();
        if (layout.getLayoutComponent(BorderLayout.CENTER) != null) {
            EnergyGraphArea.remove(layout.getLayoutComponent(BorderLayout.CENTER));
        }
        String chartTitle = "Energy Counter plot";
        String xAxis = "Elapsed Time (ms)";
        String yAxis = "";
        if (powerCounterRadioButton.isSelected()) {
            yAxis = "Instantaneous Power Consumption (uW)";
        } else if (energyCounterRadioButton.isSelected()) {
            yAxis = "Change in energy per second (uJ/s)";
        }
        boolean coreMinCheck = coreCheckBox.isSelected();
        boolean uncoreMinCheck = uncoreCheckBox.isSelected();
        boolean dramMinCheck = dramCheckBox.isSelected();
        boolean powerCounterCheck = powerCounterRadioButton.isSelected();
        boolean energyCounterCheck = energyCounterRadioButton.isSelected();
        boolean packageTotalCheck = packageTotalCheckBox.isSelected();
        YIntervalSeriesCollection set = (YIntervalSeriesCollection) new ProcessCPUData(values, coreMinCheck, uncoreMinCheck, dramMinCheck, packageTotalCheck, energyCounterCheck, powerCounterCheck).produceDataset();
        JFreeChart chart = createDeviationChart(selectedIndices, chartTitle, xAxis, yAxis, set);
        ChartPanel cp = new ChartPanel(chart);
        EnergyGraphArea.add(cp, BorderLayout.CENTER);
    }

    private void loadNetworkGraphs(int[] selectedIndices, HashMap<String, JSONReference[]> values) {
        BorderLayout layout = (BorderLayout) NetworkGraphArea.getLayout();
        if (layout.getLayoutComponent(BorderLayout.CENTER) != null) {
            NetworkGraphArea.remove(layout.getLayoutComponent(BorderLayout.CENTER));
        }
        String chartTitle = "Network IO Plot";
        String xAxis = "Elapsed Time (ms)";
        String yAxis = "Bytes per second";
        boolean sentBytes = sentCheckBox.isSelected();
        boolean receivedBytes = receivedCheckBox.isSelected();
        YIntervalSeriesCollection set = (YIntervalSeriesCollection) new ProcessNetworkData(values, sentBytes, receivedBytes).produceDataset();
        JFreeChart chart = createDeviationChart(selectedIndices, chartTitle, xAxis, yAxis, set);
        ChartPanel cp = new ChartPanel(chart);
        NetworkGraphArea.add(cp, BorderLayout.CENTER);
    }

    private void loadDataGraphs(int[] selectedIndices, HashMap<String, JSONReference[]> values) {
        BorderLayout layout = (BorderLayout)DiskGraphArea.getLayout();
        if (layout.getLayoutComponent(BorderLayout.CENTER) != null) {
            DiskGraphArea.remove(layout.getLayoutComponent(BorderLayout.CENTER));
        }
        String chartTitle = "Disk IO Plot";
        String xAxis = "Elapsed Time (ms)";
        String yAxis = "Bytes per second";
        boolean readBytes = readBytesCheckBox.isSelected();
        boolean writeBytes = writeBytesCheckBox.isSelected();
        YIntervalSeriesCollection set = (YIntervalSeriesCollection) new ProcessDiskData(values, writeBytes, readBytes).produceDataset();
        JFreeChart chart = createDeviationChart(selectedIndices, chartTitle, xAxis, yAxis, set);

        ChartPanel cp = new ChartPanel(chart);
        DiskGraphArea.add(cp, BorderLayout.CENTER);
    }

    private void loadThermalGraphs( int[] selectedIndices, HashMap<String, JSONReference[]> values) {
        BorderLayout layout = (BorderLayout) TemperatureGraphArea.getLayout();
        if (layout.getLayoutComponent(BorderLayout.CENTER) != null) {
            TemperatureGraphArea.remove(layout.getLayoutComponent(BorderLayout.CENTER));
        }
        String chartTitle = "Thermal Plot";
        String xAxis = "Elapsed Time (ms)";
        String yAxis = "Thermal Temperature (mC)";
        XYDataset set = new ProcessThermalData(values).produceDataset();
        JFreeChart chart = ChartFactory.createXYLineChart(chartTitle, xAxis, yAxis, set, PlotOrientation.VERTICAL, true, true, false);
        setChartColour(chart);
        markPlot(chart, selectedIndices);
        ChartPanel cp = new ChartPanel(chart);
        TemperatureGraphArea.add(cp, BorderLayout.CENTER);
    }

    private void loadBatteryGraphs(int[] selectedIndices, HashMap<String, JSONReference[]> values) {
        BorderLayout layout = (BorderLayout) BatteryGraphArea.getLayout();
        if (layout.getLayoutComponent(BorderLayout.CENTER) != null) {
            BatteryGraphArea.remove(layout.getLayoutComponent(BorderLayout.CENTER));
        }
        String chartTitle = "Battery Plot";
        String xAxis = "Elapsed Time (ms)";
        boolean microVoltageCheck = microVoltageRadioButton.isSelected();
        boolean microCurrentCheck = microCurrentRadioButton.isSelected();
        boolean powerCheck = powerRadioButton.isSelected();
        String yAxis = "";
        if (microVoltageCheck) {
            yAxis = "Micro Voltage (uV)";
        }
        if (microCurrentCheck) {
            yAxis = "Micro Current (uV)";
        }
        if (powerCheck) {
            yAxis = "Power (W)";
        }
        YIntervalSeriesCollection set = (YIntervalSeriesCollection) new ProcessBatteryData(values, microVoltageCheck, microCurrentCheck, powerCheck).produceDataset();
        JFreeChart chart = createDeviationChart(selectedIndices, chartTitle, xAxis, yAxis, set);
        ChartPanel cp = new ChartPanel(chart);
        BatteryGraphArea.add(cp, BorderLayout.CENTER);
    }


    private JSONReference getMeasurement(JSONReference[] array, long time, int interval) {
        JSONReference result = null;
        long bucket;
        if ((time % interval) >= (interval / 2)) {
            bucket = (long) (interval * (Math.ceil(Math.abs((float) time / interval))));
        } else {
            bucket = (long) (interval * (Math.floor(Math.abs((float) time/interval))));
        }
        if (bucket == 0L) {
            spawnErrorDialog("An event occured before the profiler was able to capture. Set a smaller interval rate to capture the event at " + time + ".");
            return null;
        }
        for (JSONReference anArray : array) {
            if (anArray.getTime() == bucket) {
                result = anArray;
                break;
            }
        }
        if (result == null) {
            spawnErrorDialog("An error occured between the elapsedTimes and the profiled data. Please re-profile the code.");
        }
        return result;
    }

    private void spawnErrorDialog(String error) {
        String[] args = {error};
        ErrorDialog.main(args);

    }

    private void setChartColour(JFreeChart chart) {
        chart.setBorderPaint(myToolWindowContent.getBackground());
        chart.setBackgroundPaint(myToolWindowContent.getBackground());
        chart.getTitle().setPaint(myToolWindowContent.getForeground());
        chart.getTitle().setFont(myToolWindowContent.getFont());
        chart.getXYPlot().getDomainAxis().setLabelFont(myToolWindowContent.getFont());
        chart.getXYPlot().getDomainAxis().setLabelPaint(myToolWindowContent.getForeground());
        chart.getXYPlot().getDomainAxis().setTickLabelPaint(myToolWindowContent.getForeground());
        chart.getXYPlot().getDomainAxis().setTickLabelFont(myToolWindowContent.getFont());
        chart.getXYPlot().getRangeAxis().setLabelFont(myToolWindowContent.getFont());
        chart.getXYPlot().getRangeAxis().setLabelPaint(myToolWindowContent.getForeground());
        chart.getXYPlot().getRangeAxis().setTickLabelPaint(myToolWindowContent.getForeground());
        chart.getXYPlot().getRangeAxis().setTickLabelFont(myToolWindowContent.getFont());
        chart.getLegend().setBackgroundPaint(myToolWindowContent.getBackground());
        chart.getLegend().setItemPaint(myToolWindowContent.getForeground());
    }

    @NotNull
    private JFreeChart createDeviationChart(int[] selectedIndices, String chartTitle, String xAxis, String yAxis, YIntervalSeriesCollection set) {
        JFreeChart chart = ChartFactory.createXYLineChart(chartTitle, xAxis, yAxis, set, PlotOrientation.VERTICAL, true, true, false);
        setChartColour(chart);
        DeviationRenderer renderer = new DeviationRenderer(true, true);
        XYPlot plot = chart.getXYPlot();
        plot.setRenderer(renderer);
        for (int i = 0; i < set.getSeriesCount(); i++) {

            renderer.setSeriesStroke(i, new BasicStroke(1.0f));
            setColor(i, renderer);

        }
        plot.setRenderer(renderer);

        // change the auto tick unit selection to integer units only...
        NumberAxis yyAxis = (NumberAxis) plot.getRangeAxis();
        yyAxis.setAutoRangeIncludesZero(false);
        yyAxis.setStandardTickUnits(NumberAxis.createIntegerTickUnits());
        markPlot(chart, selectedIndices);
        return chart;
    }




    private void setColor(int i, DeviationRenderer renderer) {
        switch (i % 17) {
            case(0):
                //red
                renderer.setSeriesPaint(i, new Color(153, 0, 0));
                renderer.setSeriesFillPaint(i, new Color(255, 102, 102));
                break;
            case(1):
                //green
                renderer.setSeriesPaint(i, new Color(76, 153, 0));
                renderer.setSeriesFillPaint(i, new Color(229, 255, 204));
                break;

            case(2):
                //blue
                renderer.setSeriesPaint(i, new Color(0, 102, 204));
                renderer.setSeriesFillPaint(i, new Color(153, 204, 255));
                break;

            case(3):
                //orange
                renderer.setSeriesPaint(i, new Color(204, 102, 0));
                renderer.setSeriesFillPaint(i, new Color(255, 204, 153));
                break;

            case(4):
                //purple
                renderer.setSeriesPaint(i, new Color(102, 0, 204));
                renderer.setSeriesFillPaint(i, new Color(204, 153, 255));
                break;

            case(5):
                //magenta
                renderer.setSeriesPaint(i, new Color(204, 0, 102));
                renderer.setSeriesFillPaint(i, new Color(255, 153, 204));
                break;

            case(6):
                //brown
                renderer.setSeriesPaint(i, new Color(110, 63, 25));
                renderer.setSeriesFillPaint(i, new Color(171, 149, 132));
                break;

            case(7):
                //dark grey
                renderer.setSeriesPaint(i, new Color(160, 160, 160));
                renderer.setSeriesFillPaint(i, new Color(224, 224, 224));
                break;

            case(8):
                //darkGreen
                renderer.setSeriesPaint(i, new Color(0, 51, 0));
                renderer.setSeriesFillPaint(i, new Color(0, 128, 0));
                break;

            case(9):
                //sickly yellow
                renderer.setSeriesPaint(i, new Color(51, 51, 0));
                renderer.setSeriesFillPaint(i, new Color(255, 255, 153));
                break;
            case(10):
                //Teal
                renderer.setSeriesPaint(i, new Color(0, 128, 128));
                renderer.setSeriesFillPaint(i, new Color(153, 255, 255));
                break;
            case(11):
                //black
                renderer.setSeriesPaint(i, new Color(0, 0, 0));
                renderer.setSeriesFillPaint(i, new Color(64, 64, 64));
                break;
            case(12):
                //dark red
                renderer.setSeriesPaint(i, new Color(102, 0, 0));
                renderer.setSeriesFillPaint(i, new Color(255, 102, 102));
                break;
            case(13):
                renderer.setSeriesPaint(i, new Color(50, 205, 50));
                renderer.setSeriesFillPaint(i, new Color(144, 238, 144));
                break;
            case(14):
                renderer.setSeriesPaint(i, new Color(49, 79, 79));
                renderer.setSeriesFillPaint(i, new Color(32, 178, 170));
                break;
            case(15):
                renderer.setSeriesPaint(i, new Color(255, 20, 147));
                renderer.setSeriesFillPaint(i, new Color(255, 192,180));
                break;
            case(16):
                renderer.setSeriesPaint(i, new Color(75, 0, 130));
                renderer.setSeriesFillPaint(i, new Color(221, 160,221));
                break;
            default:

        }
    }


    //Create the Table of JSONReference
    private void createUIComponents() {
        DefaultTableModel model = new DefaultTableModel();

        model.addColumn("File Name");
        model.addColumn("Enabled");
        fileTable = new JTable(model) {

            private static final long serialVersionUID = 1L;

            @Override
            public Class getColumnClass(int column) {
                switch (column) {
                    case 0:
                        return String.class;
                    case 1:
                        return Boolean.class;
                    default:
                        return String.class;
                }
            }

            public boolean isCellEditable(int row, int column) {
                return !(column == 0);
            }
        };
        ForcedSingleSelectionModel selectionModel = new ForcedSingleSelectionModel();
        fileTable.setSelectionModel(selectionModel);

    }
}
