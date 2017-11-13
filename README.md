# Pi Generator

This program generates Pi up to a certain amount of correct digits.

It was implemented in C++ and Qt 5.8 using QtCreator running on Lubuntu 17.04 (64 bits)

This is a way to exemplify how a main task can be split into smaller independent tasks that can be executed concurrently in separate threads.

Pi is generated in this case by means of [Leibnitz's Formula.](https://en.wikipedia.org/wiki/Leibniz_formula_for_%CF%80)

## Main Window Explained
![main window](screenshots/mainwin.png "Pi Generator - Main Window")

## Usage
The whole calculation is always split over 100 jobs.

The number of correct digits of the end result depends on the amount of terms used for the whole calculation.

The more terms used per job, the more accurate the end result will be, which can be adjusted using the **Terms Per Job** slider.

The amount of worker threads can be adjusted using the **Workers** spin box on the **Job Config** panel.

The elapsed time for the whole calculation is shown below the accumulated result.

On the **Job Browser** panel the partial results given by each job can be viewed using the **Current Job** slider.

The **Start/Stop** button controls the calculation.

The **Reset** button resets the calculation.

## Inner Workings
When the application starts, the workers are set up and 100 jobs are created and queued on the **jobs** queue. Then the GUI elements are also set up.

When the calculation is started, one job is assigned to each worker sequentially until all workers are busy. As each worker reports its end result, a pending job is assigned to the idle worker from the **jobs** queue until all the jobs have been processed.

When all jobs have been processed the calculation ends, and the elapsed time stops.

The calculation can be stopped and reset at any time.
