# Synchronization test for NC-OFDM based communication

Testing the underlay to achieve the synchronization between two different NC-OFDM transmissions.

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
