package com.IndividualProject;

public class Main {

    static volatile boolean stopFlag = false;

    public static void main(String[] args) {
        Long startTime = System.nanoTime();
        System.out.println("Test started busyWait:");
        Thread notifier = new Thread(new Runnable() {
            public void run() {
                System.out.println("notify flag set");
                try {
                    Thread.sleep(300000);
                    Main.stopFlag = true;
                } catch (java.lang.InterruptedException e) {
                }
            }
        });
        notifier.start();
        while(!stopFlag) {}
        System.out.println("Test finished, elapsed Time " + (System.nanoTime() - startTime));

        try {
            Thread.sleep(30000);

        } catch (java.lang.InterruptedException e) {

        }

        (new NotifyTest()).test();


    }


    static class NotifyTest {
        volatile  boolean stopFlag = false;

        private void test() {
            final NotifyTest instance = this;
            System.out.println("Test started wait notify:");
            long startTime = System.nanoTime();
            Thread notifier = new Thread(new Runnable() {
                public void run() {

                    System.out.println("notify flag set");
                    try {
                        Thread.sleep(300000);
                        instance.stopFlag = true;
                        synchronized (instance) {
                            instance.notify();
                        }
                    }
                    catch (java.lang.InterruptedException e) {
                    }
                }
            });
            notifier.start();
            while (!instance.stopFlag) {
                try {
                    synchronized (this) {
                        wait();
                    }
                }
                catch (InterruptedException ie) {
                }
            }
            System.out.println("Test finished, elapsed Time " + (System.nanoTime() - startTime));
        }
    }

}
