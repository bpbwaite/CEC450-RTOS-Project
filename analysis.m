%% Parse Data
close all

dataset = SDS00002;
t = dataset(:,1);
Ts = mean(diff(t));
Fs = 1/T;

CH1 = dataset(:,2);
CH2 = dataset(:,3);
CH3 = dataset(:,4);
CH4 = dataset(:,5);

vthre = 3.0
%constrain
CH1 = 5*(CH1 > vthre);
CH2 = 5*(CH2 > vthre);
CH3 = 5*(CH3 > vthre);
CH4 = 5*(CH4 > vthre);

%% SDS1
figure, hold on, grid on
plot(t,CH1+.1,t,CH2+5.2, 'LineWidth', 2)
xlabel('t (s)'), ylabel('Status')
legend('Keypad polling task', 'Servo motor pulses')
shg
% analysis for SDS1
% the keypad polling task typically runs in 
x1 = 0.0151063 - 0.0148063;
% seconds, but takes
x2 = 0.0161117 - 0.0153979;
% seconds if interrupted. thus it follows that the ISR is 
x = x2 - x1;
% seconds long. this aperiodic ISR runs constantly to control the
% servomotor ( about 0.4 ms )

%% plot for SDS2 & SDS3
figure, hold on, grid on
plot(t,CH1+.1,t,CH2+5.2,t,CH3+10.3,t,CH4+15.4, 'LineWidth', 2)
xlabel('t (s)'), ylabel('Status')
legend('Hyperperiod','Servo Task','LCD Task','Keypad polling task')
shg