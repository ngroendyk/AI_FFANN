%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Name: Nathaniel Groendyk
% Date:  30 July 2018
%Purpose: See if I got the Math right on my Neural Network.

clear; close;

%Set up inputs. Reps out function
A = 1;
B = 5;
C = 10;

F_out = 0.7;
G_out = 0.2;

N=.1; % Learning rate

%Initial Conditions
W_init = 3;
WL1_init = 1;

Wad = W_init;
Wae = W_init;

Wbd = W_init;
Wbe = W_init;

Wcd = W_init;
Wce = W_init;

Wdf = WL1_init;
Wdg = WL1_init;
Wef = WL1_init;
Weg = WL1_init;

D_bias = W_init;
E_bias = W_init;
F_bias = WL1_init;
G_bias = WL1_init;

D_sum = 0;
E_sum = 0;
F_sum = 0;
G_sum = 0;

D_error = 0;
E_error = 0;
F_error = 0;
G_error = 0;

    F_cost = [ 0 ];
    G_cost = [ 0 ];
    T_out = [ 0 ];
    
     WadOut = [0];

%Number of learning iterations
for i = 1:10000

  %Calc forward Pass
      %Layer 1
      D_sum = (A * Wad) + (B * Wbd) + (C * Wcd) + D_bias;
      D_axon = 1 / (1+exp(-1*D_sum));
      E_sum = (A * Wae) + (B * Wbe) + (C * Wce) + E_bias;
      E_axon = 1/(1+exp(-1*E_sum));
      
      %Layer 2
      F_sum = (D_axon * Wdf) + (E_axon * Wef) + F_bias;
      F_axon = 1/(1+exp(-1*F_sum));
      G_sum = (D_axon*Wdg)+ (E_axon*Weg) + G_bias;
      G_axon = 1/(1+exp(-1*G_sum));   
      
      %Do Learning Cycle. Start with Layer 2. (BP1)
      F_error = -2* (F_out - F_axon)*( exp(-1*F_sum)/((1+exp(-1*F_sum))^2));
      G_error = -2* (G_out - G_axon)*( exp(-1*G_sum)/((1+exp(-1*G_sum))^2));

      %Do learning Cycle, Layer 1: (BP2)
      D_error = ((F_error * Wdf) + (G_error * Wdg)) * ( exp(-1*D_sum)/((1+exp(-1*D_sum))^2));
      E_error = ((E_error * Wef) + (E_error * Weg)) * ( exp(-1*E_sum)/((1+exp(-1*E_sum))^2));
      
      %Do Weight and Bias update.
      %First Biases (BP3)
      F_bias = F_bias - N*F_error;
      G_bias = G_bias - N*G_error;
      D_bias = D_bias - N*D_error;
      E_bias = E_bias - N*E_error;
      
      %Now the weights (BP4)
     Wad = Wad - N*(A*D_error);
     Wbd = Wbd - N * (B*D_error);
     Wcd = Wcd - N * (C*D_error);
     
     Wae = Wae - N*(A*E_error);
     Wbe = Wbe - N*(B*E_error);
     Wce = Wce - N*(C*E_error);
     
     Wdf = Wdf - N*(D_axon*F_error);
     Wef = Wef - N*(E_axon*F_error);
    
     Wdg = Wdg - N*(D_axon*G_error);
     Weg = Weg - N*(E_axon*G_error);
    
    F_cost = [F_cost (F_out-F_axon)^2];
    G_cost = [G_cost (G_out-G_axon)^2];
    T_out = [T_out i];
    
    WadOut = [ WadOut (-N*A*D_error)];
endfor

plot(T_out, F_cost, 'b', T_out, G_cost,'r' );
%plot(T_out, WadOut);
