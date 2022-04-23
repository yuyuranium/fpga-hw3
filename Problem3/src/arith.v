module arith #(
  Add = 0,
  Sub = 1,
  Mul = 2
) (
  input  signed [7:0] opd1,
  input  signed [7:0] opd2,
  input         [1:0] op,
  output reg    [7:0] res
);

always @(*) begin
  case (op)
    Add: res = opd1 + opd2;
    Sub: res = opd1 - opd2;
    Mul: res = opd1 * opd2;
  endcase
end

endmodule
