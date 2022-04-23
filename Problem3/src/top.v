`include "arith.v"

module top (
  input  wire        clk_i,
  input  wire        rst_ni,
  input  wire        wr_en,
  input  wire [3:0]  addr_i,
  input  wire [31:0] data_i,
  output reg  [31:0] data_o
);

  integer i;
  reg [31:0] ctrl_regs[0:15];
  wire [7:0] res_d;

  always @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      for (i = 0; i < 15; i = i + 1) begin
        ctrl_regs[i] <= 32'd0;
      end
    end else begin
      if (wr_en) begin
        ctrl_regs[addr_i] <= data_i;
      end else begin
        ctrl_regs[1]      <= {24'd0, res_d};
        ctrl_regs[addr_i] <= ctrl_regs[addr_i];
      end
    end
  end

  always @(posedge clk_i) begin
    data_o <= ctrl_regs[addr_i];
  end

  arith u_arith (
    .opd1_i(ctrl_regs[0][7:0]),
    .opd2_i(ctrl_regs[0][15:8]),
    .op_i  (ctrl_regs[0][17:16]),
    .res_o (res_d)
  );

  initial begin
    $dumpvars;
    $dumpfile("top.vcd");
  end

endmodule
