%--------------------------------------------------------------------------
% Name:        ned_to_curvilinear
% Description: Converts an x, y, and z NED position to a lat, lon, and alt 
%              curvilinear position given lat, lon, and alt of the origin.
% Arguments:   - r:  x, y, and z in meters to be converted
%              - p0: lat, lon, and alt in deg/deg/m of the origin
% Returns:     Measurement converted to curvilinear position with lat, lon,
%              and alt in deg/deg/m.
%--------------------------------------------------------------------------
function p = ned_to_gps(r, p0)
    
    % Divide the difference in meters by the radius of the earth to get 
    % difference in radians
    r(:,1:2) = r(:,1:2) ./ 6371000;
    
    % Convert the difference in radians to difference in degrees
    r(:,1:2) = rad2deg(r(:,1:2));
    
    % Add the origin location to the difference in degrees
    for i = 1:size(r, 1)
        r(i,:) = r(i,:) + p0;
    end
    
    p = r;

end