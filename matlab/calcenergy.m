clear all;
close all;


tl = 2000000;
offset =  1000000;
l=100000;

fid = fopen('~/data_for_sync/noise.dat','rb');
a = fread(fid,[2,tl],'float');
b = a(:,offset:offset+l);
noise = b(1,:) + b(2,:)*1i;
fclose(fid);

fid = fopen('~/data_for_sync/first.dat','rb');
a = fread(fid,[2,tl],'float');
b = a(:,offset:offset+l);
rxdata = b(1,:) + b(2,:)*1i;
fclose(fid);


noise_energy = mean(abs(noise).^2);
signal_energy = mean(abs(rxdata).^2);

sc_factor = 1/0.5; % from the fraction of subcarriers used
mod_factor = 1/2; % from the modulation used, QPSK

10*log10(signal_energy*sc_factor*mod_factor/noise_energy)