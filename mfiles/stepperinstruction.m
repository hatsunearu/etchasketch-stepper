% this function returns the stepper motor instructions given an image file
% and starting coordinates startx and starty.
%
% stepper motor instructions have the following code associated:
% 1 2 3
% 4 5 6
% 7 8 9
%
% where 5 corresponds to the current position.
% an instruction of 5 corresponds to essentially a noop.

function instructions = stepperinstruction(im, starty, startx)


    origim = im;
	directionmatrix = [1:3; 4:6; 7:9];
    wasdmatrix = ['qwe';'asd';'zxc'];
    

	im = mean(im, 3) == 0; 
	x = startx;
	y = starty;
	instructions = 5 * ones(1,length(find(im)) - 1);
    i = 1;
    if(~im(y,x))
        disp('Please specify a starty and startx with a valid pixel');
    end
    im(y,x) = false;


    while( any(im(:)) )


		nearbypixels = getnearbypixels(im, y, x);
		nearbypixelcount = length(find(nearbypixels)); %number of true pixels

		if (nearbypixelcount == 0 && any(im(:)))
            disp('Exited without accounting for all pixels!');
			imshow(im);
            return;
		elseif (nearbypixelcount == 1)
			currinst = directionmatrix(nearbypixels);
		else 
			%ambiguous direction
			disp('Ambiguous direction, please specify direction!');
			disp(char(nearbypixels .* wasdmatrix));
            
            subplot(2,1,1);
            imshow(origim);
            axis([x-10, x+10, y-10, y+10]);
            subplot(2,1,2);
            r = double(im);
            g = double(im);
            b = double(im);
            r(y,x) = 1;
            g(y,x) = 0;
            b(y,x) = 0;
            imshow(cat(3,r,g,b), [0, 1]);
            
            axis([x-10, x+10, y-10, y+10]);
            
			currinst = input('Direction: ','s');
            currinst = directionmatrix(currinst == wasdmatrix);
		end

		instructions(i) = currinst;
        i = i+1;
		[dy, dx] = find(directionmatrix == currinst);
		x = x + dx - 2;
		y = y + dy - 2;
        
		if (~im(y,x))
			disp('something horrible happened... current pixel is blank');
			return;
		end
        
        im(y,x) = false; %destroy current pixel


    end
	
    
end

function out = getnearbypixels(im, y, x)
        [by, bx] = size(im);
        if (y > 1 && x > 1 && y < by && x < bx)
                out = im(y-1:y+1, x-1:x+1);
        end
end
