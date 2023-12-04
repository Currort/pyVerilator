`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/10 18:38:54
// Design Name: 
// Module Name: test_rom
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module rom( 
    output  [31:0]        data_o     ,
    input  [63:0]        data_addr_i,
    input    		    finish_i   
);

SSS64 u_SSS(
    .data_addr_i(data_addr_i),
    .finish_i(finish_i),
    .data_o(data_o)
);

endmodule
