Author: Jiang Wei
Date: 2021.7.9

Goal: 
    Make it easier to get calibration parameters for future JUNO offline versions.
Important: 
    These scripts are still under testing, a lot to debug, the goal is to release it before J21v2.
    If you need fresh message, help me test or have suggestions, please send an E-mail to me(jiangw@ihep.ac.cn).
Problem:
    How to make filter more stable and reliable?

2021.10.11:
    I think auto-run is OK now

<!-- What you need to prepare:
    6 sources' elecsim root file list to dir(./Elecsim_path)
        (AmC Ge68 Co60 Cs137 Laser0.1 Laser0.05)_elec.list + forceTrigger_elec.list -->
********** Make sure correctly modify those variables in CpAutoRunCalibration.sh ************
    
Step choose from: (01 02 03 04 1 2 3 4 11 12 13 14)
    
<!-- After running 03:
    res_xxx.root
        inteW: myh->GetMinimumBin()
        widthT:wh's local minimum's position

After running 04:
    Amp_threshold_rate.txt and `root getThreshold.C`
        get proper Amp_threshold

After running 04:(!!!important!!!)
    Update the Amp and inteW to (yourenv)$TUTORIALROOT/python/Tutorial/JUNOWaveRecModule.py
        Amp: see AmpThreshold.gif and Amp_threshold_1d.png in CheckStation
        inteW: see inteW.txt in CheckStation -->

2021.9.10
    Use C14 source again, no time to do event selection on forceTrigger, maybe later.
    <!-- So you need to generate C14 elecsim root files to do calibratioin rather than forceTrigger. -->
    

Notes for steps:
TimeOffset:
    Use Laser(μ~0.1) source to get each PMT's first hit time distribution, Gaussian fit the peak to get average timeOffset, and minus the mean value to get timeOffset parameters.
step01:
    Use simple integral method to get each PMT's charge distribution from Ge68 source, charge near 0 is noise, Gaussian fit the next peak to get SPE μ and σ.
step02:
    Use μ and σ from step01 to extract waveforms in (μ-1.5σ, μ+1.5σ), average those waveforms to get SPE spectra.
step03:
    Extract waveforms from Ge68 source(100k/file).
step04:
    Generate filter using waveforms from step03, and see relative distribution: pulse width, pulse amplitude, Gibbs effect, noise/signal versus amplitude threshold.
