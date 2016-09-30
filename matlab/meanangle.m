function [out] = meanangle(in)

sens = 1e-12;

%in = in * pi/180;

in = exp(1i*in);
mid = mean(in);
out = atan2(imag(mid),real(mid));%*180/pi;
out(abs(mid)<sens) = nan;
