%==========================================================================
% Autonomous Vehicle Library 
%
% Description: Estimates the location of an beacon by post-processing 
%              GPS data and range measurements using a nonlinear least 
%              squares algorithm.
%==========================================================================

close all; clear all; clc;
addpath('functions');
addpath('lbl_data');
addpath('mex');
addpath('plot_me');
disp('Running LBL beacon localizer...');

%% Load the beacon survey data

disp('Loading data...');

% Read the data files for beacon range and GPS data
range_data = read_dat_file("acoustic_ranging_node.beacon_a.dat");
gps_data = read_dat_file("gps_node.gps.dat");
t_range = range_data.t;
t_gps = gps_data.t;

t_range(10) = [];
range_data.data(10,:) = [];

% Find the GPS time that is closest to each range measurement to use at
% that range measurement's boat position
t_gps_index = zeros(length(t_range), 1);
for i = 1:length(t_range)
    [~, t_gps_index(i)] = min(abs(t_gps-t_range(i)));
end

% Get the GPS measurements closest in time to each range measurement from
% the indexes we just found
p_gps = gps_data.data(t_gps_index, 1:3);

% Convert the GPS measurements to an NED frame with origin at the first GPS
% measurement
r_gps = gps_to_ned(p_gps, p_gps(1,:));

% Get the range measurements from the range data
range_measured = range_data.data(:,5);

%% Solve the nonlinear least squares data fitting problem

disp('Running nonlinear least squares solver...');

% Nonlinear function
fun = @euclidean_range;

% Initial beacon position (initial least squares coefficients)
x0 = [-300; -300; -300];

% Data to be fit
xdata = r_gps;
ydata = range_measured;

% Run the least squares curve fit algorithm
r_beacon = lsqcurvefit(fun, x0, xdata, ydata);
p_beacon = ned_to_gps(r_beacon', p_gps(1,:));

% Calculate the range from the GPS measurements to the estimated beacon
% location for comparison
range_estimated = euclidean_range(r_beacon, xdata);

disp('LBL beacon localizer finished');
disp(' ');

%% Plot estimated vs measured range

figure(1)
hold on

% Plots
plot(t_range, range_measured, 'b.-', 'MarkerSize', 10);
plot(t_range, range_estimated, 'r.-', 'MarkerSize', 10);

% Legend
legend('Measured', 'Estimated');

% Labels
title('range vs time');
xlabel('Epoch Time');
ylabel('Range (m)');

% Font
set(gca, 'FontName', 'Times New Roman')
set(gca,'FontSize',29)

% Grid
grid on
box on
set(gca,'GridLineStyle','--')

%% Plot beacon location and GPS data

figure(2)
hold on

% Plots
plot(p_gps(:,2), p_gps(:,1), 'r.-', 'MarkerSize', 10);
plot(p_beacon(2), p_beacon(1), 'gd', 'MarkerFaceColor', 'g', 'MarkerSize', 10);
% plot_google_map('APIKey','AIzaSyDlpY42E7cy7ChKi7RkSc3A2ol0XID5huE', ...
%                 'MapType', 'satellite', ...
%                 'MapScale', 1);

% Labels
title('LBL Beacon Localization');
xlabel('Latitude (deg)');
ylabel('Longitude (deg)');

% Legend
% clickable_legend('GPS Measurements', 'Beacon Location Estimate')

% Font
set(gca, 'FontName', 'Times New Roman')
set(gca,'FontSize',29)

% Grid
grid on
box on
set(gca,'GridLineStyle','--')
