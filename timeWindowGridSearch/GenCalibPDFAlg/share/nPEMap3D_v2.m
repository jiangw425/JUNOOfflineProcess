
%The script to construct 3D nPE map using cubic spline interpolation
%powers(j)=1. for r<10000 mm ; powers(j)=3. for r>10000 mm

particles=[ "Laser0.05", "Ge68"];
toyTerm="Q";
powers=[ 1., 3. ];
for j=1:2
for i=1:2
    for phi=0:2:0
    LMu=importdata(sprintf('lmu_%s.txt', particles(i)));
    LMu=LMu';
    SMu=importdata(sprintf('smu_%s.txt', particles(i)));
    SMu=SMu';
    RT=importdata('rt.txt');

    R=RT(:, 1);
    R=R.^powers(j)
    NormF=180./(17400.^powers(j));
    R=R.*NormF

    T=RT(:, 2);

    r=[0:0.1:180.];
    theta=[0:10:180];


    [xq,yq]=meshgrid(r, theta);

    fid=fopen('gridr.txt', 'wt');
    fprintf(fid, '%g\t', xq);
    fclose(fid);
    fid=fopen('gridtheta.txt', 'wt');
    fprintf(fid, '%g\t', yq);
    fclose(fid);

    checkN = 140;
    z = griddata(R,T,LMu(:,checkN),xq,yq,'cubic');
    figure
    plot3(R,T,LMu(:,checkN),'mo')
    hold on
    mesh(xq,yq,z)
    title('cubic')
    legend('Sample Points','InterpolatedSurface','Location','NorthWest')

    z = griddata(R,T,SMu(:,checkN),xq,yq,'cubic');
    figure
    plot3(R,T,SMu(:,checkN),'mo')
    hold on
    mesh(xq,yq,z)
    title('linear')
    legend('Sample Points','InterpolatedSurface','Location','NorthWest')


    %return

    lfid=fopen(sprintf('lgridmu_%s_R%d.txt', particles(i), powers(j)), 'wt');
    sfid=fopen(sprintf('sgridmu_%s_R%d.txt', particles(i), powers(j)), 'wt');
    for i = 1:1440
        disp(sprintf('%d, %d', phi, i))
        lz = griddata(R,T,LMu(:,i),xq,yq,'cubic');
        sz = griddata(R,T,SMu(:,i),xq,yq,'cubic');
        fprintf(lfid, '%g\t', lz);
        fprintf(lfid, '\n');
        fprintf(sfid, '%g\t', sz);
        fprintf(sfid, '\n');

    end
    fclose(lfid);
    fclose(sfid);

    end
end

end
