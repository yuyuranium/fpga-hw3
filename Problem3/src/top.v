/* top.v
 * 
 * This file is a simple wrapper to wrap 3 designs of problem3. It tries to
 * simulate the read/write of control registers. The designs under test read
 * their input from `ctrl_regs` and write the result to memory mapped registers.
 */

`include "arith.v"

module top (
  input  wire        clk_i,
  input  wire        rst_ni,
  input  wire        wr_en_i,
  input  wire  [3:0] addr_i,
  input  wire [31:0] data_i,
  output reg  [31:0] data_o
);

  integer i;
  reg  [31:0] reg_data_d;
  reg  [31:0] ctrl_regs[0:15];
  wire [7:0] res_d;

  always @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      for (i = 0; i < 16; i = i + 1) begin
        ctrl_regs[i] <= 32'd0;
      end
    end else begin
      if (wr_en_i) begin
        ctrl_regs[addr_i] <= data_i;
      end
    end
  end

  always @(posedge clk_i) begin
    data_o <= reg_data_d;
  end

  always @(*) begin
    case (addr_i)
      4'h0: reg_data_d = ctrl_regs[0];
      4'h1: reg_data_d = {24'd0, res_d};  /* Result of arith connects to here */
      /* will add more later */
      default: reg_data_d = 0;
    endcase
  end

  /* The design for Problem3-1 */
  arith u_arith (
    .opd1_i(ctrl_regs[0][7:0]),
    .opd2_i(ctrl_regs[0][15:8]),
    .op_i  (ctrl_regs[0][17:16]),
    .res_o (res_d)
  );

  initial begin
    $dumpfile("top.vcd");
    $dumpvars;
  end

endmodule
