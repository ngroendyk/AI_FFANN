%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Name: Nathaniel Groendyk
% Date:  30 July 2018
%Purpose: See if I got the Math right on my Neural Network.

clear; close;
%z = [-10:0.01:10];
%Oz = 1 ./ (1 + exp(-1*z));
%Oz = exp(-z)./((1+exp(-z)).^2);
%plot (z, Oz);

yx = .8;

b = 0;

%func = 3x

w=-3;

n=.01;
Tout=[ 0 ];
Cout =[ 0];
Wout =[0];
Ozout = [0];
Bout = [ 0 ];
for i = 1: 10000

 Ip=0.01;
  
  %FrwdPass
  z= Ip .*w+b;
  Oz = 1 ./ (1+ exp(-1.*z));
  C = (yx - Oz).^2;
  
  %Backprop: Training
  
  %Calc Cost deriv
  dC = -2 .* (yx -Oz);
  dPho = exp(-1.*z)/((1+exp(-1.*z))^2);
  ErrA = dC .* dPho;
  b = b - ErrA .* n;
  w = w - n.* Ip .* ErrA;
  
  Tout = [ Tout i];
  Wout = [Wout w];
  Cout = [Cout C];
  Ozout = [Ozout Oz];
  Bout = [Bout b];
  
endfor

plot (Tout,Bout,Tout,Wout);
