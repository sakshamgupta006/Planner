%--------------------------------------------------------------------------
% Name:        curvilinear_to_ned
% Description: Converts a lat, lon, and alt curvilinear position to an NED
%              frame in meters with the given lat, lon, and alt of the
%              origin.
% Arguments:   - p: lat, lon, and alt in deg/deg/m to be converted
%              - p0: lat, lon, and alt in deg/deg/m of the origin
% Returns:     Measurement converted to an NED frame with x, y, and z in 
%              meters from the origin.
%--------------------------------------------------------------------------
function r = curvilinear_to_ned(p, p0)

    % Convert the GPS coordinates to radians
    p(:,1:2) = deg2rad(p(:,1:2));
    p0(:,1:2) = deg2rad(p0(:,1:2));

    % Calculate the difference in radians for each GPS measurement
    for i = 1:size(p, 1)
        p(i,:) = p(i,:) - p0;
    end
    
    % Multiply the difference in radians by the radius of the earth to get 
    p(:,1:2) = p(:,1:2) * 6371000;
    
    r = p;

end