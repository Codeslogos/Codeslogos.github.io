program curs;
var
i, n: integer;
k1, k2, k3, k4, m1, m2, m3, m4, l1, l2, l3, l4, n1, n2, n3, n4: real;
step, tk, x, y, Px, Py, t: real;
fg, f, fc, fd, fp : text;

function f1(a, b, c, d: real):real;

begin
f1:=(c/(15.36+cos((a/0.5)-b)*cos((a/0.5)-b)))-(d*cos((a/0.5)-b)/(26.112+1.7*cos((a/0.5)-b)*cos((a/0.5)-b)));
end;
function f2( a, b, c, d: real):real;
begin
f2:=(15.36*d/(44.39+2.89*cos((a/0.5)-b)*cos((a/0.5)-b)))-(c*cos((a/0.5)-b)/(26.112+1.7*cos((a/0.5)-b)*cos((a/0.5)-b)));
end;
function f3( a, b, c, d: real):real;
begin
f3:=((4.973*cos((a/0.5)-b)*c*c-(44.39+2.89*cos((a/0.5)-b)*cos((a/0.5)-b))*c*d+26.112*d*d)*3.4*sin((a/0.5)-b)/((44.39+2.89*cos((a/0.5)-b)*cos((a/0.5)-b))*(44.39+2.89*cos((a/0.5)-b)*cos((a/0.5)-b))))-0.3*a-9.81*sin(a/0.5);
end;
function f4( a, b, c, d: real):real;
 begin
f4:=-((4.973*cos((a/0.5)-b)*c*c-(44.39+2.89*cos((a/0.5)-b)*cos((a/0.5)-b))*c*d+26.112*d*d)*1.7*sin((a/0.5)-b)/((44.39+2.89*cos((a/0.5)-b)*cos((a/0.5)-b))*(44.39+2.89*cos((a/0.5)-b)*cos((a/0.5)-b))))-16.677*sin(b);
end;
function f5( a, b, c, d: real):real;

begin
f5:=(2.89*c*c-2*1.7*cos((a/0.5)-b)*c*d+15.36*d*d)/(2*(44.39+2.89*cos((a/0.5)-b)*cos((a/0.5)-b)))+((c*a*a/2)+9.81*(0.5*(1-cos(a/0.5))+1.7*(1-cos(b))))
end;
begin

assign(fp,'termech.dan');
reset(fp);

assign(fg,'tmech_1.res');
assign(f,'tmech_2.res');
assign(fc,'x(t).txt');
assign(fd,'y(t).txt');
rewrite(fg);
rewrite(f);
rewrite(fc);
rewrite(fd);

read(fp,n,tk);

step:=tk/n;
writeln(f,n);
writeln(f,tk:6:3);

x:=0.08;
y:=0.16;
Px:=1.364;
Py:=0.853;
t:=0;

writeln(fg,' t x y dx/dt dy/dt Px Py H');
writeln(fg,t:6:3,' ',x:10:6,' ',y:10:6,' ',f1(x,y,Px,Py):10:6,' ',
f2(x,y,Px,Py):10:6,' ',Px:10:6,' ',Py:10:6,' ',f5(x,y,Px,Py):14:10);
writeln(fc,t:6:3,' ',x:10:6);
writeln(fd,t:6:3,' ',y:10:6);

for i:= 0 to n-1 do
begin
k1:=f1(x,y,Px,Py)*step;
m1:=f2(x,y,Px,Py)*step;
l1:=f3(x,y,Px,Py)*step;
n1:=f4(x,y,Px,Py)*step;

k2:=f1(x+k1/2,y+m1/2,Px+l1/2,Py+n1/2)*step;
m2:=f2(x+k1/2,y+m1/2,Px+l1/2,Py+n1/2)*step;
l2:=f3(x+k1/2,y+m1/2,Px+l1/2,Py+n1/2)*step;
n2:=f4(x+k1/2,y+m1/2,Px+l1/2,Py+n1/2)*step;

k3:=f1(x+k2/2,y+m2/2,Px+l2/2,Py+n2/2)*step;
m3:=f2(x+k2/2,y+m2/2,Px+l2/2,Py+n2/2)*step;
l3:=f3(x+k2/2,y+m2/2,Px+l2/2,Py+n2/2)*step;
n3:=f4(x+k2/2,y+m2/2,Px+l2/2,Py+n2/2)*step;

k4:=f1(x+k3,y+m3,Px+l3,Py+n3)*step;
m4:=f2(x+k3,y+m3,Px+l3,Py+n3)*step;
l4:=f3(x+k3,y+m3,Px+l3,Py+n3)*step;
n4:=f4(x+k3,y+m3,Px+l3,Py+n3)*step;

x:=x+(k1+2*k2+2*k3+k4)/6;
y:=y+(m1+2*m2+2*m3+m4)/6;
Px:=Px+(l1+2*l2+2*l3+l4)/6;
Py:=Py+(n1+2*n2+2*n3+n4)/6;
t:=t+step;

writeln(fc,t:6:3,' ',x:10:6);
writeln(fd,t:6:3,' ',y:10:6);
writeln(fg,t:6:3,' ',x:10:6,' ',y:10:6,' ',f1(x,y,Px,Py):10:6,' ',
f2(x,y,Px,Py):10:6,' ',Px:10:6,' ',Py:10:6,' ',f5(x,y,Px,Py):14:10);
end;
close(fc);
close(fd);
close(fg);
close(f);
end.
