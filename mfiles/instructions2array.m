function [out, length] = instruction2array(in)


    length = numel(in);
    out = zeros(1, numel(in) * 2 + 1);
    out([1 end]) = '{}';
    out(2:2:end-1) = num2str(in,'%1d');
    out(3:2:end-2) = ',';
    out = char(out);

end