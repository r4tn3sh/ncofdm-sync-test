# Synchronization test for NC-OFDM based communication

Testing the underlay to achieve the synchronization between two different NC-OFDM transmissions. In this experiment a low-power underlay PN-sequence is used as a beacon signal
to indicate the start of an NC-OFDM symbol to other subsquent NC-OFDM links. This transmission, by design, consists of a low-power underlay PN-sequence
followed by a fixed number of NC-OFDM symbols (constituting a frame) that are periodically transmitted. Now, when a second NC-OFDM transmitter
is ready to start its own transmissions it first scans the medium for the presence of
beacon signals that indicate ongoing signal transmissions. If no such beacons are found
for a specified duration then the transmitter declares a time out and starts to transmit.
If a beacon signal is detected then the timing of the beacon signal is used to determine
the start of its own transmission.

We use USRP N210 radios with SBX daughterboards having the capability to operate
from 400 MHz to 4.4 GHz while transmitting up to a maximimum bandwidth of 40 MHz. The NC-OFDM links are set to operate over a 10MHz wide bandwidth, which is
further divided into N = 64 subcarriers. Cyclic prefix is set to N cp = 16, thus making
each NC-OFDM symbol span a total of 80 samples lasting for 8μs.

### Installation
git clone https://github.com/r4tn3sh/ncofdm-sync-test.git  
cd ncofdm-sync-test  
mkdir build  
cd build  
cmake ../  
make  

### Running code
Binary files for experiment can be found in "bin" folder  
example:  
cd ncofdm-sync-test/bin  
./test_tx 0  

## Running experiment in ORBIT testbed  

### login to grid  
ssh -X username@grid.orbit-lab.org  

### load the image to nodeX-X, nodeY-Y and nodeZ-Z  
omf tell -a offh -t nodeX-X,nodeY-Y,nodeZ-Z  
omf load -i image.ndz -t nodeX-X,nodeY-Y,nodeZ-Z  
omf tell -a on -t nodeX-X,nodeY-Y,nodeZ-Z  

### login to nodeX-X (transmitter 1)  

### login to nodeY-Y (transmitter 2)  

### login to nodeZ-Z (data conllector)
