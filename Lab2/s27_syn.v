
module dff_0 ( CK, Q, D );
  input CK, D;
  output Q;


  DFFX1 Q_reg ( .D(D), .CLK(CK), .Q(Q) );
endmodule


module dff_1 ( CK, Q, D );
  input CK, D;
  output Q;


  DFFX1 Q_reg ( .D(D), .CLK(CK), .Q(Q) );
endmodule


module dff_2 ( CK, Q, D );
  input CK, D;
  output Q;


  DFFX1 Q_reg ( .D(D), .CLK(CK), .Q(Q) );
endmodule


module s27 ( CK, G0, G1, G17, G2, G3 );
  input CK, G0, G1, G2, G3;
  output G17;
  wire   G5, G10, G6, G11, G7, G13, n13, n14, n15, n16, n17;

  dff_0 DFF_0 ( .CK(CK), .Q(G5), .D(G10) );
  dff_2 DFF_1 ( .CK(CK), .Q(G6), .D(G11) );
  dff_1 DFF_2 ( .CK(CK), .Q(G7), .D(G13) );
  AND2X1 U16 ( .IN1(G17), .IN2(G0), .Q(G10) );
  NOR3X0 U17 ( .IN1(G5), .IN2(n15), .IN3(n16), .QN(G11) );
  OA22X1 U18 ( .IN1(G0), .IN2(n13), .IN3(G7), .IN4(G1), .Q(n15) );
  INVX0 U19 ( .INP(G11), .ZN(G17) );
  NOR2X0 U20 ( .IN1(n17), .IN2(G3), .QN(n16) );
  NOR2X0 U21 ( .IN1(G0), .IN2(n13), .QN(n17) );
  INVX0 U22 ( .INP(G6), .ZN(n13) );
  NOR2X0 U23 ( .IN1(G2), .IN2(n14), .QN(G13) );
  NOR2X0 U24 ( .IN1(G1), .IN2(G7), .QN(n14) );
endmodule

