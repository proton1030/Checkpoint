# Checkpoint 

__A Synth Sound Replicator Eurorack Module__

- Currently implemented the analysis component using JUCE, which processes live audio input, analyzes and extracts features of theinput and outputs voltage signals through multiple 4-channel DAC chips to the analog synthesizer.
- Algorithms like temporal envelope determination are first implemented using MATLAB and implemented using C++ runningon a Raspberry Pi 3 with a customized Linux system for running speed optimizations.

- GUI design with tabs for different functions using JUCE and optimized for a touch screen to boost the intuitive userexperience.