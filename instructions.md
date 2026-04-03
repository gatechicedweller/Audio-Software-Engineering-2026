The goal of this assignment is to extend the existing codebase with a Vibrato effect. This assignment focuses on the API design as well as testing with an integrated environment.

Base your code on branch assignment2-vibrato Links to an external site.. Create a vibrato class with unit / validation tests. To read up about vibrato, see 3.3.1 in [Zolzer 02] DAFX Chapter 3.pdf.

0. [10] Implement the Ringbuffer as specified in exercise 3: ringbuffer with fractional delay but move your tests to the gtest framework (see below).

1. [15] This time, please design the class interface from scratch in src/inc/Vibrato.h. Try to design a user-friendly interface (e.g., use seconds and Hz instead of samples where appropriate). Also, at the top of the code, provide a comment block with a brief explanation about your design choices.

HINT: You are welcome to use CombFilterIf.h as a reference, though you probably don't need the wrapper class for vibrato (i.e., no separate VibratoIf and Vibrato classes).

2. [15] Implement src/Vibrato/Vibrato.cpp using a test-driven approach. Write simple unit tests (e.g., setParam with various values) in src/Tests/Tests/Test_Vibrato.cpp as you develop (see Part 4).

HINT: A good workflow is to first write the test cases (expected behaviors) for each function, run the tests to see them fail, implement, then confirm the tests pass. 

SEE: Google Test PrimerLinks to an external site.(Links to an external site.)

3. [20] Vibrato requires an LFO and fractional delay (which you already have in the ringbuffer). Design and implement a wavetable LFO class in src/inc/Lfo.h. A float-only sinusoidal LFO is good enough.

HINT: A wavetable LFO will need its own RingBuffer. It needs to have parameters for amplitude and frequency, settable during processing.

4. [30] Write these validation / verification tests in Test_Vibrato.cpp:

    Ringbuffer tests as specified in exercise 2.
    Output equals delayed input when modulation amplitude is 0.
    DC input stays DC ouput regardless of parametrization.
    Varying input block size.
    Zero input signal.
    One or more additional test(s) to verify other expected behaviors.

5. [10] Finally, test several audio files by processing them through MUSI6106Exec.cpp and compare the result to the MATLAB-processed version (vibrato.m Download vibrato.m).