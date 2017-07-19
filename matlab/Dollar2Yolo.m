%

FolderAnnotation = '/media/sdb/15bb25bf-d153-40c8-bf47-d98e46b83822/blad_steen_schaar/blad_filter/annotations/'
FolderImages = '/media/sdb/15bb25bf-d153-40c8-bf47-d98e46b83822/blad_steen_schaar/blad_filter/images/'
image_w = 1280
image_h = 720

%get the number of annotation files
D = dir([FolderImages]);
Num = length(D(not([D.isdir])))

%loop over all the files
for count=1:57
    
    outputFilename = strcat(FolderImages,int2str(count),'.txt');    
    fileID = fopen(outputFilename,'w');
    
    str_e = sprintf('%d',count)
    s = strcat(FolderImages,str_e,'.jpg')
    
    %read image
    figure(1)
    clf;
    hold on;
    A = imread(s);
    image_w = size(A,2)
    image_h = size(A,1)    
    
    filename = strcat(FolderAnnotation,int2str(count),'.txt');    
    try
    [image, x, y, w,h ,d,d,d,d,d,d,d] = textread(filename, '%s %d %d %d %d %d %d %d %d %d %d %d');
    catch
         error = strcat('unable to open image', filename);   
         disp(error)
    end   
    
    if(size(x,1) == 1)
        %single annotation for this file
        xc_d = (x+(w/2))/image_w;
        yc_d = (y+(h/2))/image_h;
        w_d = w/image_w;
        h_d = h/image_h;       
        fprintf(fileID,'%d %f %f %f %f\n',0,xc_d,yc_d,w_d,h_d);
        %imshow(A)
        %hold on;
        %plot(xc_d,yc_d,'*b')    
              
        %pause(1)        
    else
        %multilpe annotation
        for det=1:size(x,1)
            xc_d = (x(det)+(w(det)/2))/image_w;
            yc_d = (y(det)+(h(det)/2))/image_h;
            w_d = w(det)/image_w;
            h_d = h(det)/image_h;
            fprintf(fileID,'%d %f %f %f %f\n',0,xc_d,yc_d,w_d,h_d);
        end    
    end
    fclose(fileID);
end


