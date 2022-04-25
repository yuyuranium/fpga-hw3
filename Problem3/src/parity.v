/* parity.v
 *
 * Parity generator for Problem3-3
 */

module parity (
  input [31:0] data_i,
  output       even_parity_o
);

  assign even_parity_o = ^data_i;

endmodule
