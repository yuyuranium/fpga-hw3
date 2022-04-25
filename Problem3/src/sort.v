/* sort.v
 *
 * The circuit that sorts 16 8-bit data for Problem3-2.
 */
`define IDLE  4'h0
`define PASS1 4'h1
`define PASS2 4'h2
`define PASS3 4'h3
`define DONE  4'h4

module sort (
  input clk_i,
  input rst_ni,
  
  // control signal for input and output
  input  en_i,
  output busy_o,
  output valid_o,

  // data IO port
  input      [127:0] data_i,
  output reg [127:0] data_o
);

  integer i;
  reg [3:0] state_q, state_d;
  reg [7:0] in_buf[0:15];    // input buffer
  reg [7:0] out_buf[0:15];   // output buffer
  reg [3:0] cnt_arr[0:255];  // register to store count of each input data

  reg [3:0] proc_cnt_q, proc_cnt_d; // process counter
  reg [3:0] tmp;
  reg [7:0] pos;

  assign busy_o  = state_q != `IDLE;
  assign valid_o = state_q == `DONE;

  /* State machine */
  always @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      state_q <= `IDLE;
    end else begin
      state_q <= state_d;
    end
  end

  always @(*) begin
    case (state_q)
      `IDLE: begin
        if (en_i) begin
          state_d = `PASS1;
        end else begin
          state_d = `IDLE;
        end
      end
      `PASS1: begin
        if (&proc_cnt_q) begin
          state_d = `PASS2;
        end else begin
          state_d = `PASS1;
        end
      end
      `PASS2: begin
        if (en_i) begin
          state_d = `PASS1;
        end else begin
          state_d = `PASS2;
        end
      end
      `PASS3: begin
        if (en_i) begin
          state_d = `PASS1;
        end else begin
          state_d = `PASS3;
        end
      end
      `DONE: begin
        state_d = `IDLE;
      end
      default: begin
        state_d = `IDLE;
      end
    endcase
  end

  /* Input buffer */
  always @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      for (i = 0; i < 16; i = i + 1) begin
        in_buf[i] <= 8'd0;
      end
    end else begin
      if (state_q == `IDLE && en_i) begin
        { in_buf[15], in_buf[14], in_buf[13], in_buf[12],
          in_buf[11], in_buf[10], in_buf[9],  in_buf[8],
          in_buf[7],  in_buf[6],  in_buf[5],  in_buf[4],
          in_buf[3],  in_buf[2],  in_buf[1],  in_buf[0] } <= data_i;
      end
    end
  end

  /* Counter array */
  always @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      for (i = 0; i < 256; i = i + 1) begin
        cnt_arr[i] <= 4'd0;
      end
    end else begin
      if (state_q == `PASS1) begin
        cnt_arr[pos] <= cnt_arr[pos] + 1;
      end else if (state_q == `PASS2) begin
        cnt_arr[pos] <= cnt_arr[pos] + tmp;
      end
    end
  end

  /* Process counter */
  always @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      proc_cnt_q <= 4'd0;
    end else begin
      proc_cnt_d <= proc_cnt_d;
    end
  end

  always @(*) begin
    case (state_q)
      `PASS1: begin
        if (&proc_cnt_q) begin
          proc_cnt_d = 4'd0;
        end else begin
          proc_cnt_d = proc_cnt_q + 4'd1;
        end
      end
      `PASS2: begin
        if (&proc_cnt_q) begin
          proc_cnt_d = 4'd0;
        end else begin
          proc_cnt_d = proc_cnt_q + 4'd1;
        end
      end
      default: begin
        proc_cnt_d = 4'd0;
      end
    endcase
  end

  /* Position index selector */
  always @(*) begin
    case (state_q)
      `PASS1: begin
        pos = in_buf[proc_cnt_q];
      end
      `PASS2: begin
        pos = in_buf[proc_cnt_q];
      end
      default: begin
        pos = 8'd0;
      end
    endcase
  end

endmodule
