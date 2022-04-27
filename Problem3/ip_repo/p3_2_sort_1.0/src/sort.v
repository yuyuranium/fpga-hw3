/* sort.v
 *
 * The circuit that sorts 16 8-bit unsigned numbers for Problem3-2.
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
  input  [127:0] data_i,
  output [127:0] data_o
);

  integer i;
  reg [3:0] state_q, state_d;
  reg [7:0] proc_cnt_q, proc_cnt_d;  // process counter

  reg [7:0] in_buf[0:15];    // input buffer
  reg [7:0] out_buf[0:15];   // output buffer
  reg [4:0] cnt_arr[0:255];  // register to store count of each input data

  reg [7:0] pos;        // the position to write cnt_arr
  reg [4:0] cnt_arr_d;  // next input of cnt_arr

  assign busy_o  = state_q != `IDLE;
  assign valid_o = state_q == `DONE;
  assign data_o = {
    out_buf[15], out_buf[14], out_buf[13], out_buf[12],
    out_buf[11], out_buf[10], out_buf[9],  out_buf[8],
    out_buf[7],  out_buf[6],  out_buf[5],  out_buf[4],
    out_buf[3],  out_buf[2],  out_buf[1],  out_buf[0]
  };

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
        if (proc_cnt_q == 8'h0f) begin
          state_d = `PASS2;
        end else begin
          state_d = `PASS1;
        end
      end
      `PASS2: begin
        if (proc_cnt_q == 8'hff) begin
          state_d = `PASS3;
        end else begin
          state_d = `PASS2;
        end
      end
      `PASS3: begin
        if (proc_cnt_q == 8'h0f) begin
          state_d = `DONE;
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
      for (i = 0; i < 32; i = i + 1) begin
        cnt_arr[8 * i]     <= 5'd0;
        cnt_arr[8 * i + 1] <= 5'd0;
        cnt_arr[8 * i + 2] <= 5'd0;
        cnt_arr[8 * i + 3] <= 5'd0;
        cnt_arr[8 * i + 4] <= 5'd0;
        cnt_arr[8 * i + 5] <= 5'd0;
        cnt_arr[8 * i + 6] <= 5'd0;
        cnt_arr[8 * i + 7] <= 5'd0;
      end
    end else begin
      if (state_q != `DONE) begin
        cnt_arr[pos] <= cnt_arr_d;
      end else begin
        for (i = 0; i < 32; i = i + 1) begin
          cnt_arr[8 * i]     <= 5'd0;
          cnt_arr[8 * i + 1] <= 5'd0;
          cnt_arr[8 * i + 2] <= 5'd0;
          cnt_arr[8 * i + 3] <= 5'd0;
          cnt_arr[8 * i + 4] <= 5'd0;
          cnt_arr[8 * i + 5] <= 5'd0;
          cnt_arr[8 * i + 6] <= 5'd0;
          cnt_arr[8 * i + 7] <= 5'd0;
        end
      end
    end
  end

  always @(*) begin
    case (state_q)
      `PASS1: begin
        pos = in_buf[proc_cnt_q[3:0]];
      end
      `PASS2: begin
        pos = proc_cnt_q;
      end
      `PASS3: begin
        pos = in_buf[proc_cnt_q[3:0]];
      end
      default: begin
        pos = 8'd0;
      end
    endcase
  end

  always @(*) begin
    case (state_q)
      `PASS1: begin
        cnt_arr_d = cnt_arr[pos] + 5'd1;
      end
      `PASS2: begin
        if (~|pos) begin
          cnt_arr_d = cnt_arr[0];
        end else begin
          cnt_arr_d = cnt_arr[pos - 8'd1] + cnt_arr[pos];
        end
      end
      `PASS3: begin
        cnt_arr_d = cnt_arr[pos] - 5'd1;
      end
      default: begin
        cnt_arr_d = 5'd0;
      end
    endcase
  end

  /* Process counter */
  always @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      proc_cnt_q <= 8'd0;
    end else begin
      proc_cnt_q <= proc_cnt_d;
    end
  end

  always @(*) begin
    case (state_q)
      `PASS1: begin
        // 0x00 ~ 0x0f
        if (proc_cnt_q == 8'h0f) begin
          proc_cnt_d = 8'd0;
        end else begin
          proc_cnt_d = proc_cnt_q + 8'd1;
        end
      end
      `PASS2: begin
        // 0x00 ~ 0xff
        if (&proc_cnt_q) begin
          proc_cnt_d = 8'd0;
        end else begin
          proc_cnt_d = proc_cnt_q + 8'd1;
        end
      end
      `PASS3: begin
        // 0x00 ~ 0x0f
        if (proc_cnt_q == 8'h0f) begin
          proc_cnt_d = 8'd0;
        end else begin
          proc_cnt_d = proc_cnt_q + 8'd1;
        end
      end
      default: begin
        proc_cnt_d = 8'd0;
      end
    endcase
  end

  /* Write result to output buffer */
  always @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      for (i = 0; i < 16; i = i + 1) begin
        out_buf[i] <= 8'd0;
      end
    end else begin
      if (state_q == `PASS3) begin
        out_buf[cnt_arr_d[3:0]] <= pos;  // pos is in_buf[proc_cnt_q[3:0]]
      end
    end
  end

endmodule
