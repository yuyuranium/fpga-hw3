/* top.v
 * 
 * This file is a simple wrapper to wrap 3 designs of problem3. It tries to
 * simulate the read/write of control registers. The designs under test read
 * their input from `ctrl_regs` and write the result to memory mapped registers.
 */

`include "arith.v"
`include "parity.v"

module top (
  input  wire        clk_i,
  input  wire        rst_ni,
  input  wire        wr_en_i,
  input  wire [31:0] addr_i,
  input  wire [31:0] data_i,
  output reg  [31:0] data_o
);

  reg [31:0] reg_data_d;
  reg [31:0] slv_reg0_0, slv_reg0_1;
  reg [31:0] slv_reg1_0, slv_reg1_1, slv_reg1_2, slv_reg1_3, slv_reg1_4;
  reg [31:0] slv_reg2_0, slv_reg2_1;

  wire  [3:0] base   = addr_i[31:28];
  wire [27:0] offset = addr_i[27:0];

  /* Wires connecting to ouputs of each design */
  wire [7:0] res_d;
  wire even_parity_d;

  always @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      slv_reg0_0 <= 32'd0;
      slv_reg0_1 <= 32'd0;
      slv_reg1_0 <= 32'd0;
      slv_reg1_1 <= 32'd0;
      slv_reg1_2 <= 32'd0;
      slv_reg1_3 <= 32'd0;
      slv_reg1_4 <= 32'd0;
      slv_reg2_0 <= 32'd0;
      slv_reg2_1 <= 32'd0;
    end else begin
      if (wr_en_i) begin
        case (base)
          4'h0: begin
            case (offset)
              28'h0: slv_reg0_0 <= data_i;
              28'h1: slv_reg0_1 <= data_i;
              default: begin
                slv_reg0_0 <= slv_reg0_0;
                slv_reg0_1 <= slv_reg0_1;
              end
            endcase
          end
          4'h1: begin
            case (offset)
              28'h0: slv_reg1_0 <= data_i;
              28'h1: slv_reg1_1 <= data_i;
              28'h2: slv_reg1_2 <= data_i;
              28'h3: slv_reg1_3 <= data_i;
              28'h4: slv_reg1_4 <= data_i;
              default: begin
                slv_reg1_0 <= slv_reg1_0;
                slv_reg1_1 <= slv_reg1_1;
                slv_reg1_2 <= slv_reg1_2;
                slv_reg1_3 <= slv_reg1_3;
                slv_reg1_4 <= slv_reg1_4;
              end
            endcase
          end
          4'h2: begin
            case (offset)
              28'h0: slv_reg2_0 <= data_i;
              28'h1: slv_reg2_1 <= data_i;
              default: begin
                slv_reg2_0 <= slv_reg2_0;
                slv_reg2_1 <= slv_reg2_1;
              end
            endcase
          end
          default: begin
            slv_reg0_0 <= slv_reg0_0;
            slv_reg0_1 <= slv_reg0_1;
            slv_reg1_0 <= slv_reg1_0;
            slv_reg1_1 <= slv_reg1_1;
            slv_reg1_2 <= slv_reg1_2;
            slv_reg1_3 <= slv_reg1_3;
            slv_reg1_4 <= slv_reg1_4;
            slv_reg2_0 <= slv_reg2_0;
            slv_reg2_1 <= slv_reg2_1;
          end
        endcase
      end
    end
  end

  always @(posedge clk_i) begin
    data_o <= reg_data_d;
  end

  always @(*) begin
    case (base)
      4'h0: begin
        case (offset)
          28'h0: reg_data_d <= slv_reg0_0;
          28'h1: reg_data_d <= {{24{res_d[7]}}, res_d};
          default: begin
            reg_data_d <= 32'd0;
          end
        endcase
      end
      4'h1: begin
        case (offset)
          28'h0: reg_data_d <= slv_reg1_0;
          28'h1: reg_data_d <= slv_reg1_1;
          28'h2: reg_data_d <= slv_reg1_2;
          28'h3: reg_data_d <= slv_reg1_3;
          28'h4: reg_data_d <= slv_reg1_4;
          default: begin
            reg_data_d <= 32'd0;
          end
        endcase
      end
      4'h2: begin
        case (offset)
          28'h0: reg_data_d <= slv_reg2_0;
          28'h1: reg_data_d <= {31'd0, even_parity_d};
          default: begin
            reg_data_d <= 32'd0;
          end
        endcase
      end
      default: begin
      end
    endcase
  end

  /* The design for Problem3-1 */
  arith u_arith (
    .opd1_i(slv_reg0_0[7:0]),
    .opd2_i(slv_reg0_0[15:8]),
    .op_i  (slv_reg0_0[17:16]),
    .res_o (res_d)
  );

  parity u_parity (
    .data_i       (slv_reg2_0),
    .even_parity_o(even_parity_d)
  );

endmodule
