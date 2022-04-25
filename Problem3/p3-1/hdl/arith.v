module arith #(
  Add = 0,
  Sub = 1,
  Mul = 2
) (
  input  signed [7:0] opd1_i,
  input  signed [7:0] opd2_i,
  input         [1:0] op_i,
  output reg    [7:0] res_o
);

always @(*) begin
  case (op_i)
    Add: res_o = opd1_i + opd2_i;
    Sub: res_o = opd1_i - opd2_i;
    Mul: res_o = opd1_i * opd2_i;
    default: res_o = 0;
  endcase
end

endmodule