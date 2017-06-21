location_of_annotations = '/media/sdb/15bb25bf-d153-40c8-bf47-d98e46b83822/blad_steen_schaar/blad_filter/annotations.txt';
output_directory = '/media/sdb/15bb25bf-d153-40c8-bf47-d98e46b83822/blad_steen_schaar/blad_filter/';

unix(['mkdir -p ' output_directory 'annotations']);
unix(['mkdir -p ' output_directory 'images']);

A = textread(location_of_annotations, '%s');

nr = size(A,1);

i = 1;
image_nr = 1;

widths = [];
heights = [];

while(1)
    
    data = A{i};
    
    if(strcmp(data(1:5), '/medi'))
        naam = data;
        
        nr_ann = str2num(A{i+1});
        
        % Loop over ann, get 4 values
        if(nr_ann == 0)
            ann = [];
            i = i+2;
        elseif(nr_ann ==1)
            ann = [str2num(A{i+2}) str2num(A{i+3}) str2num(A{i+4}) str2num(A{i+5})];
            
            i = i+6;
        elseif(nr_ann == 2)
            ann = [str2num(A{i+2}) str2num(A{i+3}) str2num(A{i+4}) str2num(A{i+5});...
                str2num(A{i+6}) str2num(A{i+7}) str2num(A{i+8}) str2num(A{i+9}) ];
            
            i = i+10;
        elseif(nr_ann ==3)
            ann = [str2num(A{i+2}) str2num(A{i+3}) str2num(A{i+4}) str2num(A{i+5});...
                str2num(A{i+6}) str2num(A{i+7}) str2num(A{i+8}) str2num(A{i+9});...
                str2num(A{i+10}) str2num(A{i+11}) str2num(A{i+12}) str2num(A{i+13})];
            i = i+14;
        end
        
        % Save file here
        % if ann not empty save
        if(~isempty(ann))
            
            filename = [output_directory 'annotations/' num2str(image_nr) '.txt'];
            fileID = fopen(filename,'w');
            for j=1:size(ann, 1)
                fprintf(fileID, 'blad %d %d %d %d 0 0 0 0 0 0 0\n', ann(j,1), ann(j,2), ann(j,3), ann(j,4));
            end
            fclose(fileID);
            
            % Copy image to folder            
            filename1 = [output_directory 'images/' num2str(image_nr) '.jpg'];
            filename2 = data;
            
            unix(['cp ' filename2 ' ' filename1]);
            
            w = ann(:,3);
            h = ann(:,4);
            
            widths = [widths;w];
            heights = [heights;h];            
        end
        
        image_nr = image_nr + 1;
        
    end
    
    if(i > nr)
        break;
    end
    
    
end
