clear all;
close all;

seqlen = 10000;
seq = randi(2,1,seqlen)-1;

fid = fopen('pnseq.dat','w');
fwrite(fid,seq);

fclose(fid);