%==========================================================================
% Autonomous Vehicle Library
%
% Description: Calculates the rotation matrix from an axis defined by the
%              unit vectors X, Y, and Z to the unit vectors:
%                  X0 = [1, 0, 0]
%                  Y0 = [0, 1, 0]
%                  Z0 = [0, 0, 1]
%              This is useful for finding sensor to body frame rotation
%              matrices.
%==========================================================================

close all; clear all; clc;
addpath('functions');

%% Define IMU frame unit vetors relative to body frame

sin45 = sin(deg2rad(45));

X = [0.0; sin45; sin45];
Y = [0.0; -sin45; sin45];
Z = [1.0; 0.0; 0.0];

%% Determine rotation matrix

R = get_rotation_matrix(X, Y, Z)
