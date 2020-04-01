%--------------------------------------------------------------------------
% Name:        get_rotation_matrix
% Description: Gets the rotation matrix that rotates from the reference
%              frame defined by the (X2, Y2, Z2) unit vectors to the 
%              reference frame defined by the unit vectors 
%                  X1 = (1, 0, 0)
%                  Y1 = (0, 1, 0)
%                  Z1 = (0, 0, 1)
%   http://www.meshola.com/Articles/converting-between-coordinate-systems
% Arguments:   - X2: x-axis unit vector of reference frame 2 (3x1)
%              - Y2: y-axis unit vector of reference frame 2 (3x1)
%              - Z2: z-axis unit vector of reference frame 2 (3x1)
% Returns:     Rotation matrix from the first to the second refernce frame.
%--------------------------------------------------------------------------
function R_2_1 = get_rotation_matrix(X2, Y2, Z2)

X1 = [1.0; 0.0; 0.0];
Y1 = [0.0; 1.0; 0.0];
Z1 = [0.0; 0.0; 1.0];

R_2_1(1,1) = dot(X1, X2);
R_2_1(1,2) = dot(X1, Y2);
R_2_1(1,3) = dot(X1, Z2);

R_2_1(2,1) = dot(Y1, X2);
R_2_1(2,2) = dot(Y1, Y2);
R_2_1(2,3) = dot(Y1, Z2);

R_2_1(3,1) = dot(Z1, X2);
R_2_1(3,2) = dot(Z1, Y2);
R_2_1(3,3) = dot(Z1, Z2);

end
