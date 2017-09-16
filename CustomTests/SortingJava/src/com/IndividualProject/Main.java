package com.IndividualProject;

import java.io.File;
import java.io.FileWriter;
import java.io.Writer;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Random;

public class Main {

    private static final int ARRAYSIZE = 500000;
    private static Random random = new Random(100);
    private static Random trueRandom = new Random();
    private enum Sorting {
        INSERTION(0), MERGESORT(1), BUBBLESORT(2), QUICKSORT(3);
        private final int val;

        Sorting(int val) {
            this.val = val;
        }

        public static Sorting fromInt( int elem ) {
            switch (elem) {
                case 0:
                    return INSERTION;
                case 1:
                    return MERGESORT;
                case 2:
                    return BUBBLESORT;
                case 3:
                    return QUICKSORT;
                default:
                    return null;
            }
        }

        public String toString() {
            switch (val) {
                case 0:
                    return "INSERTION SORT";
                case 1:
                    return "MERGE SORT";
                case 2:
                    return "BUBBLE SORT";
                case 3:
                    return "QUICK SORT";
                default:
                    return "";
            }
        }
    }

    public static void main(String[] args) {
        int test1 = 0;
        int test2 = 4;
        if (args.length == 2) {
            test1 = Integer.parseInt(args[1]) - 1;
            test2 = Integer.parseInt(args[2]);

            if (test1 < 0 || test1 > 4) {
                System.err.println("first argument outside range");
                System.exit(-1);
            }
            if (test2 < 0 || test2 > 4) {
                System.err.println("second argument outside range");
                System.exit(-1);
            }
            if (test1 > test2) {
                System.err.println("invalid arguments");
                System.exit(-1);
            }
        }
        //generate Random array for test
        int[] arr = randomArrayGen();

        File dir = new File("./testDir");
        dir.mkdir();

        //delete all text files in teh directory
        deleteAllTextFiles();

        //run the tests
        for (int i = test1; i < test2; i++) {
            //for each test clone the array for fairness
            int[] test = arr.clone();
            TimeKeeper.timeKeeper.TimeKeeper.insertTimePoint("file:///home/kd913/IndividualProject/CustomTests/SortingJava/src/com/IndividualProject/Main.java", "Main.java", 88);
            runTest(Sorting.fromInt(i), test);

        }
        TimeKeeper.timeKeeper.TimeKeeper.printTimePoints();
    }



    private static void runTest(Sorting algo, int[] arr) {
        try{

            //Write the array results to disk
            String nameOfText = "Before-" + algo.toString() + ".txt";
            writeArrayToFile(arr, nameOfText);

            TimeKeeper.timeKeeper.TimeKeeper.insertTimePoint("file:///home/kd913/IndividualProject/CustomTests/SortingJava/src/com/IndividualProject/Main.java", "Main.java", 104);
            System.out.println("Writing done in :  milliseconds");
            Thread.sleep(5000);

            //Read back the contents of the array
            int[] readArr = readArrayFromFile(nameOfText);
            System.out.println("Reading done in : milliseconds");

            //Perform algortihm sorting
            System.out.println("Before " + algo.toString() + "\n");
            TimeKeeper.timeKeeper.TimeKeeper.insertTimePoint("file:///home/kd913/com.IndividualProject/SortingJava/src/com/com.IndividualProject/Main.java", "Main.java", 114);
            System.out.println("Finished\n");
            long startTime = System.nanoTime();
            switch (algo) {
                case INSERTION:
                    insertionSort(readArr);
                    break;
                case MERGESORT:
                    readArr = mergeSort(readArr);
                    break;
                case QUICKSORT:
                    quickSort(readArr, 0, readArr.length -1 );
                    break;
                case BUBBLESORT:
                    bubbleSort(readArr);
                    break;
                default:
                    break;
            }


            TimeKeeper.timeKeeper.TimeKeeper.insertTimePoint("file:///home/kd913/com.IndividualProject/SortingJava/src/com/com.IndividualProject/Main.java", "Main.java", 135);
            System.out.println("Duration " + algo.toString() + "\n");
            Thread.sleep(5000);
            //Write result of test
            String nameOfResult = "After-" + algo.toString() + ".txt";
            writeArrayToFile(readArr, nameOfResult);
            TimeKeeper.timeKeeper.TimeKeeper.insertTimePoint("file:///home/kd913/com.IndividualProject/SortingJava/src/com/com.IndividualProject/Main.java", "Main.java", 142);
            System.out.println("Writing done in : milliseconds");
            System.out.println("Finished " + algo.toString() + "\n");
        } catch (Exception e) {
            System.err.println("test " + algo.toString() + "failed");
        }
    }


    private static void insertionSort(int[] insSort) {
        for (int i = 1; i < insSort.length; i++  ) {
            int key = insSort[i];
            int j = i - 1;
            while (j > -1 && insSort[j] > key) {
                insSort[j + 1] = insSort[j];
                j--;
            }
            insSort[j+1] = key;
        }
    }

    private static int[] mergeSort(int[] merSort) {
        if (merSort.length == 1) {
            return merSort;
        } else {
            int halfWay = merSort.length / 2;
            int[] left = Arrays.copyOfRange(merSort, 0, halfWay);
            int[] right = Arrays.copyOfRange(merSort, halfWay, merSort.length);
            left = mergeSort(left);
            right = mergeSort(right);
            return merge(left, right);
        }
    }

    private static int[] merge(int[] left, int[] right) {
        int resultLength = left.length + right.length;
        int leftIndex = 0;
        int rightIndex = 0;
        int i = 0;
        boolean addLeft = false;
        boolean addRight = false;
        int[] result = new int[resultLength];
        for (; i < resultLength; i++) {
            if (left[leftIndex] <= right[rightIndex]) {
                result[i] = left[leftIndex];
                leftIndex++;
                if (leftIndex >= left.length) {
                    addRight = true;
                    break;
                }
            } else if (right[rightIndex] <= left[leftIndex]){
                result[i] = right[rightIndex];
                rightIndex++;
                if (rightIndex >= right.length) {
                    addLeft = true;
                    break;
                }
            }
        }
        i++;
        int[] leftover;
        if (addRight) {
            leftover = Arrays.copyOfRange(right, rightIndex, right.length);
        } else if (addLeft) {
            leftover = Arrays.copyOfRange(left, leftIndex, left.length);
        } else {
            return result;
        }


        for (int aLeftover : leftover) {
            result[i] = aLeftover;
            i++;
        }

        return result;
    }

    private static void bubbleSort(int[] bubSort) {
        int arrayLength = bubSort.length;
        for (int i = 0; i < arrayLength - 1; i++) {
            for (int j = arrayLength - 1; j >= i + 1; j--) {
                if (bubSort[j] < bubSort[j - 1]) {
                    swap(bubSort, j, j-1);
                }
            }
        }
    }

    private static void quickSort(int[] quiSort, int low, int high) {
        if (low < high) {
            int pivot  = randomisedPartition(quiSort, low, high);
            quickSort(quiSort, low, pivot - 1);
            quickSort(quiSort, pivot + 1, high);
        }
    }

    private static int randomisedPartition(int[] quiSort, int low, int high) {
        int pivot = trueRandom.nextInt(high - low ) + low;
        swap(quiSort, high, pivot);
        return partition(quiSort, low, high);

    }

    private static int partition(int[] quiSort, int low, int high) {
        int pivot = quiSort[high];
        int seenLow = low - 1;
        for (int i = low; i < high; i++) {
            if (quiSort[i] <= pivot) {
                seenLow++;
                swap(quiSort, seenLow, i);
            }
        }
        swap(quiSort, seenLow + 1, high);
        return seenLow + 1;
    }

    private static void swap(int[] arr, int elem1, int elem2) {
        int temp = arr[elem1];
        arr[elem1] = arr[elem2];
        arr[elem2] = temp;
    }

    private static int[] randomArrayGen() {
        int[] result = new int[ARRAYSIZE];
        for (int i = 0 ; i < result.length; i++) {
            result[i] = random.nextInt();
        }
        return result;
    }

    private static void writeArrayToFile(int[] fileToWrite, String textFile) {
        try{
            Writer w = new FileWriter("./testDir/" + textFile);
            for (int aFileToWrite : fileToWrite) {
                w.write(Integer.toString(aFileToWrite) + "\n");
            }
            w.close();
        } catch (Exception e) {
            System.out.println("error when writing to disk");
        }
    }

    private static int[] readArrayFromFile(String textFile) throws java.io.IOException {
        return Files.lines(Paths.get("./testDir/" + textFile)).map(Integer::parseInt).mapToInt(i->i).toArray();
    }

    private static void deleteAllTextFiles() {
        Arrays.stream(new File("./testDir/").listFiles((f, p) -> p.endsWith(".txt"))).forEach(File::delete);

    }
}