%--------------------------------------------------------------------------
% Name:        euclidean_range
% Description: Calculates the Euclidean range between a point x and each
%              point in xdata. For use as the nonlinear equation input
%              to the lsqcurvefit function.
% Arguments:   - x: estimated x, y, and z point in an NED frame in meters
%              - xdata: x, y, and z data points point in an NED frame in
%                meters
% Returns:     Euclidean ranges between the estimated point and the data
%              points in meters.
%--------------------------------------------------------------------------
function range = euclidean_range(x, xdata)

    % Initialize the range vector
    range = zeros(size(xdata,1),1);

    % Calculate the Euclidean range for each measurement
    for i = 1:size(xdata,1)

        x1 = x(1);
        y1 = x(2);
        z1 = x(3);
        x2 = xdata(i,1);
        y2 = xdata(i,2);
        z2 = xdata(i,3);
        
        % Calculate the Euclidean range
        range(i) = sqrt((x2-x1)^2 + (y2-y1)^2 + (z2-z1)^2);
        
    end
    
end