dh = 0.5;

Point(1) = { 1, 1,0, dh};
Point(2) = {-1, 1,0, dh};
Point(3) = {-1,-1,0, dh};
Point(4) = { 1,-1,0, dh};
Point(5) = { 1, 1,1, dh};
Point(6) = {-1, 1,1, dh};
Point(7) = {-1,-1,1, dh};
Point(8) = { 1,-1,1, dh};

Line(10) = {1,2};
Line(11) = {2,3};
Line(12) = {3,4};
Line(13) = {4,1};
Line(14) = {5,6};
Line(15) = {6,7};
Line(16) = {7,8};
Line(17) = {8,5};

Line(18) = {1,5};
Line(19) = {2,6};
Line(20) = {3,7};
Line(21) = {4,8};

Line Loop(30) = {10, 11, 12, 13};
Line Loop(31) = {14, 15, 16, 17};
Line Loop(32) = {10, 19,-14,-18};
Line Loop(33) = {11, 20,-15,-19};
Line Loop(34) = {12, 21,-16,-20};
Line Loop(35) = {13, 18,-17,-21};

Ruled Surface(40) = {30};
Ruled Surface(41) = {31};
Ruled Surface(42) = {32};
Ruled Surface(43) = {33};
Ruled Surface(44) = {34};
Ruled Surface(45) = {35};

Physical Surface(50) = {40};
Physical Surface(51) = {41};
Physical Surface(52) = {42,43,44,45};

Surface Loop(55) = {40,41,42,43,44,45};

Volume(60) = {55};
Physical Volume(100) = {60};
